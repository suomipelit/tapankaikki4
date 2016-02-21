#include "CColorTable.h"
#include "CPalette.h"
#include "files.h"

CColorTable::CColorTable()
{
	Init();
}

void CColorTable::GenerateTransparencyTable(CPalette& aPalette)
{
	int a,b,r1,g1,b1,r2,g2,b2;

	for (a=0;a<256;a++)
	{
		r1=aPalette.Color(a).r;
		g1=aPalette.Color(a).g;
		b1=aPalette.Color(a).b;
	
		for (b=0;b<256;b++)
		{
			r2=aPalette.Color(b).r;
			g2=aPalette.Color(b).g;
			b2=aPalette.Color(b).b;
			TransparencyTable[a][b]=CLightTable::ClosestInPalette((r1+r2)>>1,(g1+g2)>>1,(b1+b2)>>1,aPalette);
		}
	}
}

void CColorTable::GenerateShadowTable(CPalette& aPalette)
{
	int a,z;
	float r,g,b,h,s,l,e;

	for (a=0;a<256;a++)
	{
		r=aPalette.Color(a).r;
		g=aPalette.Color(a).g;
		b=aPalette.Color(a).b;
		CPalette::rgb2hsl(r,g,b,h,s,l);
		e=l/16;

		for (z=0;z<16;z++)
		{
			l-=e/2.5f;
			if (l<0) l=0;
			CPalette::hsl2rgb(h,s,l,r,g,b);
			ShadowTable[a][z]=CLightTable::ClosestInPalette((int)r,(int)g,(int)b,aPalette);
		}
	}
}

void CColorTable::WriteTables(const char *aFilename)
{
	FILE *tab=fopen(getsavepath(std::string(aFilename)).c_str(),"wb");
	if (!tab) error("Error: Couldn't create %s!",aFilename);

	fwrite(TransparencyTable,sizeof(TransparencyTable),1,tab);
	fwrite(ShadowTable,sizeof(ShadowTable),1,tab);
	fwrite(iNormalLight.iData,sizeof(iNormalLight.iData),1,tab);
	fwrite(iRedLight.iData,sizeof(iRedLight.iData),1,tab);
	fwrite(iYellowLight.iData,sizeof(iYellowLight.iData),1,tab);
	fwrite(iExplosionLight.iData,sizeof(iExplosionLight.iData),1,tab);
	fclose(tab);
}

int CColorTable::ReadTables(const char *aFilename) 
{
	FILE *tab= fopen(getsavepath(std::string(aFilename)).c_str(),"rb");

	if (!tab) return 1;
	fread(TransparencyTable,sizeof(TransparencyTable),1,tab);
	fread(ShadowTable,sizeof(ShadowTable),1,tab);
	fread(iNormalLight.iData,sizeof(iNormalLight.iData),1,tab);
	fread(iRedLight.iData,sizeof(iRedLight.iData),1,tab);
	fread(iYellowLight.iData,sizeof(iYellowLight.iData),1,tab);
	fread(iExplosionLight.iData,sizeof(iExplosionLight.iData),1,tab);
	fclose(tab);

	return 0;
}

void CColorTable::Init()
{
	int err=1;
	if (filetimestamp("palette.tab")>filetimestamp("efps/walls1.efp"))
	{
		err = ReadTables("palette.tab");
	}
	if (err)
	{
		CreateTables("efps/walls1.efp");
		WriteTables("palette.tab");
	}

	for(int a=0; a<KLightAmount; a++) 
		iLightMap[a].Make(15+a*5);
}

void CColorTable::CreateTables(const char *aPaletteFilename)
{
	CPalette pal;

	pal.LoadEFPPal(aPaletteFilename);

	GenerateTransparencyTable(pal);
	GenerateShadowTable(pal);
	iNormalLight.Make(0,0,0,pal);
	iRedLight.Make(5,0,0,pal);
	iYellowLight.Make(5,5,0,pal);
	iExplosionLight.Make2(pal);
}

const CLightMap& CColorTable::LightMap(int aLightMap) const
{
	ASSERT(aLightMap>=0);
	ASSERT(aLightMap<KLightAmount);
	return iLightMap[aLightMap];
}

const CLightTable& CColorTable::NormalLight() const
{
	return iNormalLight;
}

const CLightTable& CColorTable::RedLight() const
{
	return iRedLight;
}

const CLightTable& CColorTable::YellowLight() const
{
	return iYellowLight;
}

const CLightTable& CColorTable::ExplosionLight() const
{
	return iExplosionLight;
}

