#include "CGraphicsBuffer.h"

#include <SDL_image.h>
#include "CPalette.h"
#include "files.h"

namespace
{
	#define PCX_RLE 1

	const char KSCITag[4] = {'R','I','X','3'};
	const char KEFPTag[6] = {'E','F',' ','p','i','c'};

	typedef struct TPCXheader  
	{
                char  manufacturer;   // always 10
                char  version;        // should be 5
                char  encoding;       // 1 for RLE
                char  bits_per_pixel; // usually 8, for 256-color
                short xmin, ymin;     // the width  is *usually*  (xmax-xmin+1)
                short xmax, ymax;     // the height is *usually*  (ymax-ymin+1)
                short horz_res, vert_res;     // DPI for printing
                char  ega_palette[48];        // junk  :)
                char  reserved;       
                char  num_color_planes;       // usually 1 (3 for 24-bit color)
                short bytes_per_line;         // MUST BE an EVEN number
                short palette_type;           // should be 1
                char  padding[58];            // junk
	} TPCXHeader;
}

#define swappaa(a,b) { a+=b;b=a-b;a-=b; }
#define KFixedPointBits 10

CGraphicsBuffer::CGraphicsBuffer(const CRect<int>& aRect) 
{
	DEBUG_ASSERT(aRect.Width()>5);
	DEBUG_ASSERT(aRect.Height()>5);

	iHeight=aRect.Height();
	iWidth=aRect.Width();
	iBuf=(unsigned char*)malloc(iWidth*iHeight);
	ASSERT(iBuf!=NULL);
}

CGraphicsBuffer::CGraphicsBuffer(int aWidth,int aHeight): iWidth(aWidth), iHeight(aHeight)
{
	ASSERT(iHeight!=0);
	ASSERT(iWidth!=0);
	iBuf=(unsigned char*)malloc(iWidth*iHeight);
	ASSERT(iBuf!=NULL);
}

CGraphicsBuffer::CGraphicsBuffer(CGraphicsBuffer& aBuf)
{
	iHeight=aBuf.iHeight;
	iWidth=aBuf.iWidth;
	ASSERT(iHeight!=0);
	ASSERT(iWidth!=0);

	iBuf=(unsigned char*)malloc(iWidth*iHeight);
	ASSERT(iBuf!=NULL);
	memcpy(iBuf,aBuf.iBuf,iWidth*iHeight);
}

void CGraphicsBuffer::Load(std::string aFilename, CPalette* aPalette)
{
 	ASSERT(aFilename.length()>0);

	// Translate the filename to have DATADIR before
	// calling any of the real loading functions.
	aFilename = getdatapath(aFilename);

	const char* ext = aFilename.c_str() + aFilename.find_last_of('.');

	if (strcasestr(ext,".efp2"))
	{
		LoadEFP2(aFilename,aPalette);
		return;
	}

	if (strcasestr(ext,".efp"))
	{
		LoadEFP(aFilename,aPalette);
		return;
	}

	if (strcasestr(ext,".sci"))
	{
		LoadSCI(aFilename,aPalette);
		return;
	}
	
	if (strcasestr(ext,".pcx"))
	{
		LoadPCX(aFilename,aPalette);
		return;
	}

	SDL_Surface* surf = IMG_Load(aFilename.c_str());

	if ( surf )
	{
		if (CopyFromSurface(surf ,aPalette)==0)
		{
			SDL_FreeSurface( surf );
			return;
		}
                else
                        error("CGraphicsBuffer::Load: Only 8-bit surfaces allowed (%s)!",aFilename.c_str());
	}
	
	error("CGraphicsBuffer::Load: File format not detected (%s)!",aFilename.c_str());
}

void CGraphicsBuffer::Save(const std::string& aFilename, const CPalette* aPalette) const
{
	ASSERT(aFilename.length()>0);
	const char* ext = aFilename.c_str() + aFilename.find_last_of('.');

	if (strcasestr(ext,".efp2"))
	{
		SaveEFP2(aFilename,aPalette);
		return;
	}
	if (strcasestr(ext,".efp"))
	{
		SaveEFP(aFilename,aPalette);
		return;
	}
	
	if (strcasestr(ext,".sci"))
	{
		SaveSCI(aFilename,aPalette);
		return;
	}
	
	if (strcasestr(ext,".pcx"))
	{
		printf("CGraphicsBuffer::Save: pcx image found\n");
		SavePCX(aFilename,aPalette);
		return;
	}

	if (strcasestr(ext,".bmp"))
	{
                SaveBMP(aFilename,aPalette);
		return;
	}
	error("CGraphicsBuffer::Save: File format not detected (%s)!",aFilename.c_str());
}


void CGraphicsBuffer::LoadEFP(const std::string& aFilename, CPalette* aPalette)
{
	ASSERT(aFilename.length()>0);

	unsigned int p;
	unsigned short w,h;

	int tavu,tavu2;
	int length = filelen(aFilename.c_str()), cnt=0;

	ASSERT(length<10*1024*1024); // less than 10 megabytes
	FILE *efp=fopen(aFilename.c_str(),"rb");
	if (!efp) error("CGraphicsBuffer::LoadEFP: File %s couldn't be opened!\n",aFilename.c_str());

	unsigned char *filebuffer= new unsigned char[length];
	fread(filebuffer,length,1,efp);
	fclose(efp);
	
	if (strncmp((char*)filebuffer,KEFPTag,6) ) 
		error("CGraphicsBuffer::LoadEFP: File %s is not an EFP file!\n",aFilename.c_str());

	cnt+=6;
	w = *(short*)(filebuffer+cnt);cnt+=2;
	h = *(short*)(filebuffer+cnt);cnt+=2;

	Resize(w,h);

	for(p=0;p<(unsigned int)w*h;)
	{
		tavu=filebuffer[cnt++];
		if (tavu>192)
		{
			tavu2=filebuffer[cnt++];
			memset(iBuf+p,tavu2,tavu-192);
			p+=tavu-192;
		} 
		else 
			iBuf[p++]=(char)tavu;
	} 

	if (aPalette)
	{
		aPalette->CopyPalette(filebuffer+cnt);
		aPalette->FourMultiply();
	}

	delete[] filebuffer;
}

void CGraphicsBuffer::LoadPCX(const std::string& aFilename, CPalette* aPalette)
{
	ASSERT(aFilename.length()>0);

	unsigned int p;
	unsigned short w,h;
	unsigned char tavu,tavu2;

	FILE *pcx=fopen(aFilename.c_str(),"rb");

	if (!pcx)
	{
		error("CGraphicsBuffer::LoacPCX: Unable to load %s",aFilename.c_str());
	}

	fseek(pcx,8,SEEK_SET);
	w=fgetc(pcx)+1;
	w+=fgetc(pcx)<<8;
	h=fgetc(pcx)+1;
	h+=fgetc(pcx)<<8;

	Resize(w,h);

	fseek(pcx,128,SEEK_SET);

	for(p=0;p<(unsigned int)w*h;)
	{
		tavu=fgetc(pcx);
		if (tavu>192) 
		{ 
			tavu2=fgetc(pcx);
			for(;tavu>192;tavu--) 
				iBuf[p++]=tavu2;
		} 
		else 
			iBuf[p++]=tavu;
	} 

	if (aPalette)
	{
		fseek(pcx,-256*3,SEEK_END);
		aPalette->ReadPalette(pcx);
	}

	fclose(pcx);
}

void CGraphicsBuffer::LoadEFP2(const std::string& aFilename, CPalette* aPalette)
{
	ASSERT(aFilename.length()>0);

	FILE *efp=fopen(aFilename.c_str(),"rb");
	unsigned char buf[4];

	if (!efp) error("CGraphicsBuffer::LoadEFP: File %s couldn't be opened!\n",aFilename.c_str());

	fread(buf,4,1,efp);
	if (memcmp(buf,"EFP2",4)!=0)
		error("CGraphicsBuffer::LoadEFP2: File malformed (%s)!",aFilename.c_str());
	
	fread(&iWidth,4,1,efp);	// WIDTH 
	fread(&iHeight,4,1,efp);	// HEIGHT

	Resize(iWidth,iHeight);
	// BITS (8 bittiä => palette seuraavaksi)
	if (fgetc(efp)!=8) error("CGraphicsBuffer::LoadEFP2: Only 8 bits EFP2 files are supported (%s)",aFilename.c_str());
	
	if (aPalette)
		aPalette->ReadPalette(efp);
	else
		fseek(efp,768,SEEK_CUR);

	aPalette->FourMultiply();

	fread(iBuf,iWidth*iHeight,1,efp);
	fclose(efp);
}

void CGraphicsBuffer::LoadSCI(const std::string& aFilename, CPalette* aPalette)
{
	ASSERT(aFilename.length()>0);

	FILE *sci=fopen(aFilename.c_str(),"rb");
	unsigned short width=iWidth, height=iHeight;
	const short Sumthing_Important=0x00AF;
	char tag[4];

	if (fread(tag,1,4,sci)!=4) error("CGraphicsBuffer::LoadSCI: fread failed (1)\n");
	if (memcmp(tag,KSCITag,4)!=0)
		 error("CGraphicsBuffer::LoadSCI: SCI tag mismatch\n");

	if (fread(&width,1,2,sci)!=2) error("CGraphicsBuffer::LoadSCI: fread failed (2)\n");
	if (fread(&height,1,2,sci)!=2) error("CGraphicsBuffer::LoadSCI: fread failed (3)\n");

	Resize(width,height);

	fseek(sci,10,SEEK_SET);
	
	if (aPalette)
	{
		aPalette->ReadPalette(sci);
		aPalette->FourMultiply();
	}

	if (fread(iBuf,1,iWidth*iHeight,sci)!=(iWidth*iHeight)) error("CGraphicsBuffer::LoadSCI: fread failed()\n");
	fclose(sci);
}



void CGraphicsBuffer::SaveSCI(const std::string& aFilename, const CPalette* aPalette) const
{
	ASSERT(aFilename.length()>0);

	FILE *sci=fopen(getsavepath(aFilename).c_str(),"wb");
	int a;
	unsigned short width=iWidth, height=iHeight;
	const short Sumthing_Important=0x00AF;

	ASSERT(iWidth<=0xffff);
	ASSERT(iHeight<=0xffff);

	if (fwrite(KSCITag,1,4,sci)!=4) error("CGraphicsBuffer::SaveSCI: fwrite failed (1)\n");
	if (fwrite(&width,1,2,sci)!=2) error("CGraphicsBuffer::SaveSCI: fwrite failed (2)\n");
	if (fwrite(&height,1,2,sci)!=2) error("CGraphicsBuffer::SaveSCI: fwrite failed (3)\n");
	if (fwrite(&Sumthing_Important,1,2,sci)!=2) error("CGraphicsBuffer::SaveSCI: fwrite failed (4)\n");

	if (aPalette)
		aPalette->AppendPaletteDiv4(sci);
	else 
		for (a=0;a<768;a++)
			fputc(0,sci);

	if (fwrite(iBuf,1,iWidth*iHeight,sci)!=(width*height)) error("CGraphicsBuffer::SaveSCI: fwrite failed (5)\n");
	fclose(sci);

}


// FIXED 9.3.2003 by Allu
void CGraphicsBuffer::SaveEFP(const std::string& aFilename, const CPalette* aPalette) const
{
	ASSERT(aFilename.length()>0);

	unsigned short width=iWidth, height=iHeight;
	int a,samecol,size;
	int pixel;
	FILE *efp;

  	ASSERT(iWidth<=0xffff);
	ASSERT(iHeight<=0xffff);

	size=height*width;
	efp=fopen(getsavepath(aFilename).c_str(),"wb");

	if (efp==NULL) error("CGraphicsBuffer::SaveEFP: Couldn't open file for writing (%s)", aFilename.c_str());
	if (fwrite(KEFPTag,1,6,efp)!=6) error("CGraphicsBuffer::SaveEFP: fwrite failed (1)");
	if (fwrite(&width,1,2,efp)!=2) error("CGraphicsBuffer::SaveEFP: fwrite failed (2)");
	if (fwrite(&height,1,2,efp)!=2) error("CGraphicsBuffer::SaveEFP: fwrite failed (3)");
 
	 for (a=0;a<size;a++)
	 {
		samecol=0;
		if (a<size-1)
			while(iBuf[a]==iBuf[a+samecol+1]&& 
				samecol<(255-193)&& 
				(a+samecol)<(size-1))			
			samecol++; // kuinka paljon samaa väriä

		if (samecol==0)
		{ 
			if ((pixel=(unsigned char)iBuf[a])<193) 
				fputc(pixel,efp);
			else 
			{ 
				fputc(193,efp);
				fputc(pixel,efp);
			}
		}
		if (samecol>0) 
		{ 
			fputc(193+samecol,efp); 
			fputc((int)(unsigned char)iBuf[a],efp);
			a+=samecol; 
		} 
    }
	if (aPalette)
		aPalette->AppendPaletteDiv4(efp);
	else 
		for (a=0;a<768;a++)
			fputc(0,efp);
	fclose(efp);
}

void CGraphicsBuffer::SavePCX(const std::string& aFilename, const CPalette* aPalette) const
{
	ASSERT(aFilename.length()>0);

	int count=0;
	TPCXHeader p;
	FILE *pcx;

	p.manufacturer=10;
	p.version=5;
	p.encoding=PCX_RLE;
	p.bits_per_pixel=8;
	p.xmin=0;
	p.ymin=0;
	p.xmax=(short)(iWidth-1);
	p.ymax=(short)(iHeight-1);
	p.horz_res=(short)75;
	p.vert_res=(short)75;
	memset(p.ega_palette,0,48);
	p.reserved=0;
	p.num_color_planes=1; 
	p.bytes_per_line=(short)iWidth; 
	p.palette_type=1; // color
	memset(p.padding,0,58);

	pcx=fopen(getsavepath(aFilename).c_str(),"wb");
	if (pcx==NULL) 
		error("CGraphicsBuffer::SavePCX: Couldn't open file for writing (%s)", aFilename.c_str());

    if (fwrite(&p,1,sizeof(TPCXHeader),pcx)!=sizeof(TPCXHeader)) 
		error("CGraphicsBuffer::SavePCX: fwrite failed");

	while (count<iHeight*iWidth)
	{

		int runcount = 0;       // reset some variables...
		int key = iBuf[count];
		int key2 = key;

		// check to see how long this "run"
		// in the image is...
		while(key == key2 && runcount < 63 && count<iHeight*iWidth) 
       	{
			runcount++;
			count++;
			key2=iBuf[count];
			if(count % iWidth == 0) break;  // "runs" cannot go from one
		}                                   //  row to another.
		if(runcount == 1)					// if it isn't a "run"
		{
			if(key >= 0xC0)					// save either a "run" of length 1...
			{
				putc(193, pcx);
				putc(key, pcx);
			}
			else
				putc(key, pcx);				// ... or put a raw pixel
		}
		else								// if it is a "run", save the "runlength" and then
		{									//  save the color index number
			putc(runcount + 192, pcx);
			putc(key, pcx);
		}

	}

	// end tag, I guess..
	putc(0x0c,pcx);

	if (aPalette)
		aPalette->AppendPalette(pcx);
	else 
		for (int a=0;a<768;a++)
			fputc(0,pcx);
	fclose(pcx);
}

void CGraphicsBuffer::SaveEFP2(const std::string& aFilename, const CPalette* aPalette) const
{
	ASSERT(aFilename.length()>0);

	FILE *efp=fopen(getsavepath(aFilename).c_str(),"wb");

	if (!efp) error("CGraphicsBuffer::SaveEFP2: File %s couldn't be opened!\n",aFilename.c_str());

	fwrite("EFP2",4,1,efp);		// MAGIC
	fwrite(&iWidth,4,1,efp);	// WIDTH 
	fwrite(&iHeight,4,1,efp);	// HEIGHT
	fputc(8,efp);				// BITS (8 bittiä => palette seuraavaksi)

	if (aPalette)
		aPalette->AppendPalette(efp);
	else
		for (int a=0;a<768;a++)
			fputc(0,efp);

	fwrite(iBuf,iWidth*iHeight,1,efp);
	fclose(efp);
}

CRect<int> CGraphicsBuffer::TransCopy(int aDX,int aDY, const CGraphicsBuffer* buf,int aSX, int aSY,int aWidth, int aHeight, int aColorKey)
{
	ASSERT(iBuf!=NULL);
	ASSERT(buf!=NULL);
	DEBUG_ASSERT(aDX>=0);
	DEBUG_ASSERT(aDY>=0);
	DEBUG_ASSERT(aSX>=0);
	DEBUG_ASSERT(aSY>=0);
	DEBUG_ASSERT(aDX+aWidth<=iWidth);
	DEBUG_ASSERT(aDY+aHeight<=iHeight);
	DEBUG_ASSERT(aSX+aWidth<=buf->iWidth);
	DEBUG_ASSERT(aSY+aHeight<=buf->iHeight);

	unsigned char c;
	int w=buf->Width();
	unsigned char *srcptr=buf->iBuf+w*aSY+aSX;
	unsigned char *dstptr=iBuf+aDY*iWidth+aDX;
	

	for (int y=0;y<aHeight;y++,srcptr+=w,dstptr+=iWidth)
	 for (int x=0;x<aWidth;x++)
	 {
		c=srcptr[x];
		if (c)
			dstptr[x]=c;
	 }

	return CRect<int>(aDX,aDY,aDX+aWidth,aDY+aHeight);
}

CRect<int> CGraphicsBuffer::Copy(int aDX,int aDY, const CGraphicsBuffer* buf,int aSX, int aSY,int aWidth, int aHeight)
{
	ASSERT(iBuf!=NULL);
	ASSERT(buf!=NULL);
	DEBUG_ASSERT(aDX>=0);
	DEBUG_ASSERT(aDY>=0);
	DEBUG_ASSERT(aSX>=0);
	DEBUG_ASSERT(aSY>=0);
	DEBUG_ASSERT(aDX+aWidth<=iWidth);
	DEBUG_ASSERT(aDY+aHeight<=iHeight);
	DEBUG_ASSERT(aSX+aWidth<=buf->iWidth);
	DEBUG_ASSERT(aSY+aHeight<=buf->iHeight);

	for (int a=0;a<aHeight;a++)
		memcpy(iBuf+(a+aDY)*iWidth+aDX,buf->Ptr()+aSX+(a+aSY)*buf->Width(),aWidth);

	return CRect<int>(aDX,aDY,aDX+aWidth,aDY+aHeight);
}

CRect<int> CGraphicsBuffer::Copy(const CGraphicsBuffer* buf,int aSX, int aSY,int aWidth, int aHeight)
{
	DEBUG_ASSERT(aSX>=0);
	DEBUG_ASSERT(aSY>=0);
	DEBUG_ASSERT(aWidth<=iWidth);
	DEBUG_ASSERT(aHeight<=iHeight);
	DEBUG_ASSERT(aSX+aWidth<=buf->iWidth);
	DEBUG_ASSERT(aSY+aHeight<=buf->iHeight);
	return Copy(0,0,buf,aSX,aSY,aWidth,aHeight);
}

CRect<int> CGraphicsBuffer::Copy(const CGraphicsBuffer* src)
{
	ASSERT(iBuf!=NULL);
	ASSERT(src!=NULL);
	DEBUG_ASSERT(iWidth>=src->iWidth);
	DEBUG_ASSERT(iHeight>=src->iHeight);

	return Copy(src->Ptr(),src->Width(),src->Height());
}

inline CRect<int> CGraphicsBuffer::Copy(const unsigned char *buf,int aWidth, int aHeight)
{	
	int a;
	DEBUG_ASSERT(buf);
	DEBUG_ASSERT(iWidth>=aWidth);
	DEBUG_ASSERT(iHeight>=aHeight);

	ASSERT(iBuf!=NULL);

	for (a=0;a<aHeight;a++)
	 memcpy(iBuf+(a)*iWidth, buf+a*aWidth, aWidth);

	return CRect<int>(0,0,aWidth,aHeight);
}

CRect<int> CGraphicsBuffer::Copy(int aDX, int aDY,const CGraphicsBuffer* buf)
{	
	int a=0;
	DEBUG_ASSERT(buf);
	DEBUG_ASSERT(aDX+buf->iWidth<=iWidth);
	DEBUG_ASSERT(aDY+buf->iHeight<=iHeight);

	return Copy(aDX,aDY,buf,0,0,buf->Width(),buf->Height());
}

inline CRect<int> CGraphicsBuffer::Copy(int aDX, int aDY,const unsigned char *buf,int aWidth, int aHeight)
{	
	int a;
	ASSERT(iBuf!=NULL);
	DEBUG_ASSERT(aDX+aWidth<=iWidth);
	DEBUG_ASSERT(aDY+aHeight<=iHeight);

	for (a=0;a<aHeight;a++)
		memcpy(iBuf+aDX+(a+aDY)*iWidth, buf+a*aWidth, aWidth);

	return CRect<int>(aDX,aDY,aDX+aWidth,aDY+aHeight); 
}

inline CRect<int> CGraphicsBuffer::Copy(const unsigned char *buf,int aSX, int aSY,int aWidth, int aHeight)
{	
	int a;

	ASSERT(iBuf!=NULL);
	DEBUG_ASSERT(buf);
	DEBUG_ASSERT(aWidth<=iWidth);
	DEBUG_ASSERT(aHeight<=iHeight);

	for (a=0;a<aHeight;a++)
		memcpy(iBuf+(a)*iWidth, buf+aSX+(aSY+a)*aWidth, aWidth);

	return CRect<int>(0,0,aWidth,aHeight);
}

CGraphicsBuffer::CGraphicsBuffer(): iWidth(0), iHeight(0), iBuf(NULL)
{
}

void CGraphicsBuffer::Resize(int aWidth,int aHeight)
{
	if (aWidth == iWidth && aHeight == iHeight)
		return;
	unsigned char* newbuf=(unsigned char*)malloc(aWidth*aHeight);
	DEBUG_ASSERT(aWidth>0);
	DEBUG_ASSERT(aHeight>0);
	DEBUG_ASSERT(aWidth<0xffff);
	DEBUG_ASSERT(aHeight<0xffff);

	for (int y=0;y<aHeight&&y<iHeight;y++)
		for (int x=0;x<aWidth&&x<iWidth;x++)
		{
			newbuf[y*aWidth+x]=iBuf[y*iWidth+x];
		}

	iWidth=aWidth;
	iHeight=aHeight;
	free(iBuf);
	iBuf=newbuf;
	ASSERT(iBuf!=NULL);
}

void CGraphicsBuffer::Stretch(int aNewWidth,int aNewHeight)
{	int x,y;
	unsigned char* newbuf=(unsigned char*)malloc(aNewWidth*aNewHeight);

	ASSERT(newbuf);
	DEBUG_ASSERT(aNewWidth>0);
	DEBUG_ASSERT(aNewHeight>0);
	DEBUG_ASSERT(aNewWidth<0xffff);
	DEBUG_ASSERT(aNewHeight<0xffff);
	
	for (y=0;y<aNewHeight;y++)
		for (x=0;x<aNewWidth;x++)
		{
			newbuf[y*aNewWidth+x]=iBuf[y*iHeight/aNewHeight*iWidth+x*iWidth/aNewWidth];
		}

	free(iBuf);
	iBuf=newbuf;
	iWidth=aNewWidth;
	iHeight=aNewHeight;
}

void CGraphicsBuffer::SaveBMP(const std::string& aFilename,const CPalette* aPalette) const
{
	ASSERT(aPalette);
	ASSERT(aFilename.length()>0);
	SDL_Surface* s;

	s=CopyToSurface(aPalette);
	if (s==NULL) error("CGraphicsBuffer::CopyToSurface failed. Saving to BMP possible.");
	SDL_SaveBMP(s,aFilename.c_str());
	DeleteSurface(s);
}

SDL_Surface* CGraphicsBuffer::CopyToSurface(const CPalette* aPalette) const
{
	ASSERT(aPalette);
	SDL_Surface* surface=SDL_CreateRGBSurfaceFrom(iBuf,iWidth,iHeight,8,iWidth,8,8,8,8);

	if (surface==NULL) 
		return NULL;

	ASSERT( surface->format->palette->ncolors == 256 );
	memcpy(surface->format->palette->colors, aPalette->ColorData(), 256*sizeof(SDL_Color));

	return surface;
}

int CGraphicsBuffer::CopyFromSurface(SDL_Surface* aSurface, CPalette* aPalette)
{
	ASSERT(aSurface);
	// few checks 
	if (aSurface->w==0||
		aSurface->h==0) return 1;

	if (aSurface->format->BitsPerPixel!=8||
		aSurface->format->BytesPerPixel!=1) 
                return 1;

	Resize(aSurface->w,aSurface->h);

	for (int y=0;y<iHeight;y++)
		memcpy(iBuf+y*iWidth,(char*)aSurface->pixels+y*aSurface->pitch,iWidth);

	if (aPalette)
	{
		memcpy( aPalette->ColorData(),aSurface->format->palette->colors, aSurface->format->palette->ncolors*sizeof(SDL_Color) );
	}

	return 0;
}

void CGraphicsBuffer::DeleteSurface(SDL_Surface* aSurface) const
{
	ASSERT(aSurface);
	SDL_FreeSurface(aSurface);
}

void CGraphicsBuffer::Clear()
{
	if (!iBuf) return;
	memset(iBuf,0,iWidth*iHeight);
}

CGraphicsBuffer::~CGraphicsBuffer()
{
	free(iBuf);
}

void CGraphicsBuffer::Reset()
{
	free(iBuf);
	iBuf = NULL;
	iWidth = 0;
	iHeight = 0;
}

CRect<int> CGraphicsBuffer::DrawBox( const CRect<int>& aRect,int aAdd) 
{
	int a, b, c, offs=aRect.iTop*iWidth+aRect.iLeft;
	for (a=0; a<aRect.iBottom-aRect.iTop; a++, offs+=iWidth-(aRect.iRight-aRect.iLeft)) 
		for (b=0; b<aRect.iRight-aRect.iLeft; b++, offs++) 
		{
			if (a==0||
				a==aRect.iBottom-aRect.iTop-1||
				b==0||
				b==aRect.iRight-aRect.iLeft-1) 
				c=iBuf[offs]+1;
			else 
				c=iBuf[offs]-aAdd;
			if (c<0 ) c=0;
			if (c>63) c=63;
			iBuf[offs]=(unsigned char) c;
		}

	return aRect;
}


CRect<int> CGraphicsBuffer::Rectangle(int aX1,int aY1,int aX2,int aY2,unsigned char aCol,int aOffset)
{
	DEBUG_ASSERT(aX1>=0);
	DEBUG_ASSERT(aY1>=0);
	DEBUG_ASSERT(aX2>=0);
	DEBUG_ASSERT(aY2>=0);
	DEBUG_ASSERT(aX1<iWidth);
	DEBUG_ASSERT(aY1<iHeight);
	DEBUG_ASSERT(aX2<iWidth);
	DEBUG_ASSERT(aY2<iHeight);
	int a, offs, offs1, offs2, offs3;
	
	offs1=aOffset+aY1*iWidth+aX1;
	offs2=aOffset+aY1*iWidth+aX2;
	offs3=aOffset+aY2*iWidth+aX1;

	memset(iBuf+offs1, aCol, (aX2-aX1) +1);
	memset(iBuf+offs3, aCol, (aX2-aX1) +1);
	for(a=0,offs=offs1+iWidth;a<(aY2-aY1)-1;a++,offs+=iWidth) 
		iBuf[offs]=aCol;

	for(a=0,offs=offs2+iWidth;a<(aY2-aY1)-1;a++,offs+=iWidth) 
		iBuf[offs]=aCol;

	return CRect<int>(aX1,aY1,aX1,aY2);
}

CRect<int> CGraphicsBuffer::FillRectangle( const CRect<int>& aRect, unsigned char aColor )
{
	int a;
	unsigned char* ptr;
	const int h = aRect.Height();
	const int w = aRect.Width();

	ptr = iBuf + aRect.iTop * iWidth + aRect.iLeft;

	for (a=0;a<h;a++,ptr+=iWidth)
		memset( ptr, aColor, w );

	return aRect;

}


inline void CGraphicsBuffer::DrawHorizLineNoClip(unsigned char* aLinePtr, int aCol,int aX1,int aX2)
{
	DEBUG_ASSERT(aLinePtr);
	memset(aLinePtr+aX1,aCol,aX2-aX1+1);
}

inline void CGraphicsBuffer::DrawHorizLineClip(unsigned char* aLinePtr, int aCol,int aX1,int aX2)
{
	DEBUG_ASSERT(aLinePtr);
	if (aX1 < 0) aX1=0;
	if (aX2 >= iWidth) aX2=iWidth-1;
	if (aX1 > aX2) return;
	memset(aLinePtr+aX1,aCol,aX2-aX1+1);
}

inline void CGraphicsBuffer::DrawPolyTri(int aK1,int aK2,int aX,int aY,int aHeight, int aCol,int aClip)
{
	int adder,deltaX1,deltaX2,row,StartY=0;
	unsigned char* linePtr;
	//unsigned char *yg,*yg2;
	
	if (aClip){
		if (aHeight < 0)
		{ 
			if (aY<0 || aY+aHeight >= iHeight) return;
			if (aHeight+aY < 0) aHeight-=aHeight+aY;
			if (aY > iHeight-1) StartY=aY-(iHeight-1);
		} else
		{ 
			if (aY>=iHeight || aY+aHeight<=0 ) return;
			if (aY+aHeight > iHeight-1) aHeight-=aHeight+aY-(iHeight-1);
			if (aY < 0) StartY=abs(aY); 
		}
	}
	
	adder=iWidth;

	linePtr=iBuf + aY*adder;//y ofs in screen
	
	// Draw from down to up
	if (aHeight<0)
	{
		adder=-adder;
		aHeight=-aHeight;
	}
	
	deltaX1=deltaX2=aX<<KFixedPointBits; //deltaX1 = left side, deltaX2 = right side

	// aK1 should be always bigger that aK2 (because memset doesn't go reverse :))
	if (aK1>aK2) swappaa(aK1,aK2);

	if (! aClip)
	{
		for (row=0 ;row <= aHeight;row++)
		{
			DrawHorizLineNoClip(linePtr,aCol,deltaX1>>KFixedPointBits,deltaX2>>KFixedPointBits);
			linePtr+=adder;
			deltaX1+=aK1;
			deltaX2+=aK2;
		}
	}
	else
	{
		deltaX1+=aK1*StartY;
		deltaX2+=aK2*StartY;
		linePtr+=StartY*adder;

		for (row=StartY;row <= aHeight;row++)
		{
			DrawHorizLineClip(linePtr,aCol,deltaX1>>KFixedPointBits,deltaX2>>KFixedPointBits);
			linePtr+=adder;
			deltaX1+=aK1;
			deltaX2+=aK2;
		}
	}
}

void CGraphicsBuffer::DrawPolygon(int aX1,int aY1,int aX2,int aY2,int aX3,int aY3,int aCol,int aClip)
{
	int k1,k2,h,h2;

	if (aY1>aY2) { swappaa(aX1,aX2);swappaa(aY1,aY2); };
	if (aY1>aY3) { swappaa(aX1,aX3);swappaa(aY1,aY3); };
	if (aY2>aY3) { swappaa(aX2,aX3);swappaa(aY2,aY3); };

	if (aY1==aY2)
	{
		h=aY1-aY3;//p3 based DrawPolygon
		if (h==0) return;
		k1=((aX3-aX1)<<KFixedPointBits)/h;
		k2=((aX3-aX2)<<KFixedPointBits)/h;
		DrawPolyTri(k1,k2,aX3,aY3,h,aCol,aClip);
		return;
	}

	if (aY2==aY3)
	{
		h=aY3-aY1;//p1 based DrawPolygon
		if (h==0) return;
		k1=((aX2-aX1)<<KFixedPointBits)/h;
		k2=((aX3-aX1)<<KFixedPointBits)/h;
		DrawPolyTri(k1,k2,aX1,aY1,h,aCol,aClip);
		return;
	}
	
	h=aY2-aY1;
	h2=aY3-aY1;
	k1=((aX2-aX1)<<KFixedPointBits)/h;
	k2=((aX3-aX1)<<KFixedPointBits)/h2;
	DrawPolyTri(k1,k2,aX1,aY1,h,aCol,aClip);
	
	h2-=h;
	k1=((aX2-aX3)<<KFixedPointBits)/h2;
	DrawPolyTri(k1,-k2,aX3,aY3,-h2,aCol,aClip);
}	
