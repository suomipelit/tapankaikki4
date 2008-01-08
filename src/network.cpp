#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <SDL_net.h>

#include "common/error.h"
#include "network.h"

const int HTTPBUFFER=1024;

CNetworkAddress& CNetworkAddress::operator =(const CNetworkAddress* aAddr)
{
	return *this;
}
const char* CNetworkAddress::GetHostname() const
{
	return "";
}
const char* CNetworkAddress::GetAddress() const
{
	return "";
}

void CNetworkDevice::Send(int dest, int type, ... )
{
}

void CNetworkDevice::SendNow(const CNetworkAddress* dest, int type, ... )
{
}

const CNetworkAddress* CNetworkDevice::BroadCastAddress()
{
	return iBroadCastAddress;
}

const CNetworkAddress* CNetworkDevice::LocalAddress()
{
	return iLocalAddress;
}

int CNetworkDevice::Available()
{
	return 0;
}


CNetworkDevice::CNetworkDevice()
{
	if(SDLNet_Init()==-1) 
	{
		error("SDLNet_Init: %s\n", SDLNet_GetError());
	}
}

CNetworkDevice::~CNetworkDevice()
{
	SDLNet_Quit();
}
/*
void sendipxnow(struct nodeaddr *node,int type,...) 
{
	int a=0;
// 	TEMP VARIABLES 
	int b,c,d;
	char *str;
	char *p;

// 	TEMP VARIABLES END
	auto va_list ap;
	va_start(ap,type);
		
	if(send->ecb.inuse) while(send->ecb.inuse);

	switch(type) 
	{
		case NETWORK_STEAMPACK:
							send->data[a++]=(char)type;
							send->data[a++]=(char)Steam_amount;
							send->data[a++]=(char)(c=va_arg(ap,int));
							d=c*(MAX_NETWORKDATA_SIZE-100);
							b=d+(MAX_NETWORKDATA_SIZE-100);
							b=min(b,Steam_amount*sizeof(Steam));

							for(;d<b;d++)
							 send->data[a++]=((char*)(steam))[d];
							break;

		case NETWORK_LIGHTPACK:
							send->data[a++]=(char)type;
							send->data[a++]=(char)Spot_amount;
							send->data[a++]=(char)(c=va_arg(ap,int));
							d=c*(MAX_NETWORKDATA_SIZE-100);
					
					  
						b=d+(MAX_NETWORKDATA_SIZE-100);
							b=min(b,Spot_amount*sizeof(Spot));

							for(;d<b;d++)
							 send->data[a++]=((char*)(spot_light))[d];
							break;
	 	 
	    case NETWORK_GAMEDATA:	  send->data[a++]=(char) type;
                              b=va_arg(ap,int);
							  send->data[a++]=(char)(player_amount()+1);
							  send->data[a++]=(char) b;
							  str=(char *) &DEATH_MATCH_SPEED;
							  for(c=0;c<sizeof(float);c++) send->data[a++]=*(str++);
							  for(d=0;d<MAX_PLAYERS;d++)
  							  if((player[d].enabled) ||(d==b))
							  {
							  send->data[a++]=(char) d;
							  str=(char *) &player[d].x;
							  for(c=0;c<sizeof(float);c++) send->data[a++]=*(str++);
							  str=(char *) &player[d].y;
							  for(c=0;c<sizeof(float);c++) send->data[a++]=*(str++);
							  str=(char *) &player[d].rangle;
							  for(c=0;c<sizeof(int);c++) send->data[a++]=*(str++);
							  send->data[a++]=(char) player[d].color;
							  str=(char *) &player[d].burning;
							  for(c=0;c<sizeof(int);c++) send->data[a++]=*(str++);
							  str=(char *) &player[d].ipxnode;
							  for(c=0;c<sizeof(struct nodeaddr);c++) send->data[a++]=*(str++);
							  for(c=0;c<10;c++) send->data[a++]=player[d].name[c];
							  str=(char *) &player[d].ANIM;
							  for(c=0;c<sizeof(int);c++) send->data[a++]=*(str++);
							  str=(char *) &player[d].player_kills;
							  for(c=0;c<sizeof(int);c++) send->data[a++]=*(str++);
							  str=(char *) &player[d].DEAD;
							  for(c=0;c<sizeof(int);c++) send->data[a++]=*(str++);
							  }
							  MBoard.add_message("Gamedata sent");
							  break;
		case NETWORK_NEWPLAYERDATA:send->data[a++]=(char) type;
							   c=va_arg(ap,char);
							   send->data[a++]=(char) c;
							   for(d=0;d<10;d++)
							    send->data[a++]=player[c].name[d];
							   str=(char*) &(player[c].ipxnode);
							   for(d=0;d<sizeof(struct nodeaddr);d++)
							    send->data[a++]=*(str++);
							   str=(char*) &(player[c].x);
							   for(d=0;d<sizeof(float);d++)
							    send->data[a++]=*(str++);
							   str=(char*) &(player[c].y);
							   for(d=0;d<sizeof(float);d++)
							    send->data[a++]=*(str++);
							  break;
		case NETWORK_IMJOININGNOW:send->data[a++]=(char) type;
						   	  str=va_arg(ap,char *);
							  for(b=0;b<10;b++)
							   send->data[a++]=*(str++);
							  break;
		case NETWORK_SERVERSEARCH:send->data[a++]=(char) type;
							  break;
		case NETWORK_SERVERANSWER:send->data[a++]=(char) type;
							  str=va_arg(ap,char *);
							  for(b=0; b<10; b++) 
							  send->data[a++]=str[b];
							  break;
		case NETWORK_SERVERSHUTDOWN:send->data[a++]=(char) type;
							    break;
		case NETWORK_JOINREQUEST:send->data[a++]=(char) type;
							 break;
		case NETWORK_SENDLEVELINFO:send->data[a++]=(char) type;
							   break;
		case NETWORK_SENDLEVEL:send->data[a++]=(char) type;
						   b=va_arg(ap,int);
						   send->data[a++]=(char)(b & 255);
						   send->data[a++]=(char)(b >> 8);
						   break;
		case NETWORK_TRANSMITLEVEL:
						   send->data[a++]=(char) type;
						   b=va_arg(ap,int);
						   send->data[a++]=(char)(b & 255);
						   send->data[a++]=(char)(b >> 8);
						   str=(char*)leveldata.level;
						   for(c=b*(MAX_NETWORKDATA_SIZE-24);(c<(b+1)*(MAX_NETWORKDATA_SIZE-24))&&(c<(sizeof(struct BLOCK)*leveldata.x_size*leveldata.y_size));c++)
						    send->data[a++]=str[c];

			 			   break;
		case NETWORK_JOINACCEPT:send->data[a++]=(char) type;
							b=va_arg(ap,int);
							send->data[a++]=(char) b;
							if(b) MBoard.add_message("Accepting player...");
							  else MBoard.add_message("Player NOT accepted...");

							break;
		case NETWORK_LEVELINFO: send->data[a++]=(char)type;
							send->data[a++]=(char)(leveldata.x_size&255);
							send->data[a++]=(char)(leveldata.x_size>>8);
							send->data[a++]=(char)(leveldata.y_size&255);
							send->data[a++]=(char)(leveldata.y_size>>8);
							p=(char *)&general_level_info;
							for(b=0;b<sizeof(General_level_info);b++)
							 send->data[a++]=p[b];
							break;
		default:error("sendipxnow: message %d not yet implemented\n",type);
	}
	send->data[a++]=NETWORK_END;
	sendpacket(sendseg,*node,send,(unsigned short)(a+1));
	va_end(ap);
}

void sendipx(int dest,int type,...) 
{	int c,b,a;
	char *str;
	char d;

	float x,y;

	va_list ap;
	va_start(ap,type);

	switch(type) 
	{
	 	case NETWORK_NEWEFFECT:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			d=va_arg(ap,char);
			ipxstream[dest][ipxofs[dest]++]=d;
			d=va_arg(ap,char);
			ipxstream[dest][ipxofs[dest]++]=d;
			b= va_arg(ap,int);
			str=(char *) &b;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			b= va_arg(ap,int);
			str=(char *) &b;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			b= va_arg(ap,int);
			str=(char *) &b;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			b= va_arg(ap,int);
			str=(char *) &b;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
		break;
	 	
	 	case NETWORK_SOUND:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			d=va_arg(ap,char);
			ipxstream[dest][ipxofs[dest]++]=d;
			d=va_arg(ap,char);
			ipxstream[dest][ipxofs[dest]++]=d;
			b= va_arg(ap,int);
			str=(char *) &b;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			b= va_arg(ap,int);
			str=(char *) &b;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			b= va_arg(ap,int);
			str=(char *) &b;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
		break;

	    case NETWORK_BULLET:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			d=(char)va_arg(ap,int);
			ipxstream[dest][ipxofs[dest]++]=d;
			x=bullet[d].x;
			y=bullet[d].y;
			str=(char *) &x;
			for(c=0;c<sizeof(float);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &y;
			for(c=0;c<sizeof(float);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &bullet[d].bullet_type_index;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &bullet[d].index;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &bullet[d].angle;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &bullet[d].time;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &bullet[d].count;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &bullet[d].IN_USE;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
		break;

	 
	    case NETWORK_NEWBULLET:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			d=va_arg(ap,char);
			ipxstream[dest][ipxofs[dest]++]=d;
			b= va_arg(ap,int);
			str=(char *) &b;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			b=va_arg(ap,int);
			x=bullet[b].x;
			y=bullet[b].y;
			str=(char *) &x;
			for(c=0;c<sizeof(float);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &y;
			for(c=0;c<sizeof(float);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &bullet[b].angle;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &bullet[b].time;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
		break;
		
		case NETWORK_UPDCRATE:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			d= va_arg(ap,char);
			ipxstream[dest][ipxofs[dest]++]=d;
			a=va_arg(ap,char);
		
			str=(char *) &a;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &crate[a].type;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &crate[a].weapon;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &crate[a].bullet;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &crate[a].energy;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &crate[a].x;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &crate[a].y;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &crate[a].IN_USE;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *) &crate[a].text_count;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
		break;

		case NETWORK_ALIVE:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			ipxstream[dest][ipxofs[dest]++]=(char)aplayer[0]->tindex;
		break;

		case NETWORK_CAMETOGAME:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			ipxstream[dest][ipxofs[dest]++]=(char)aplayer[0]->tindex;
		break;

		case NETWORK_PLAYERLEFTGAME:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
	 		ipxstream[dest][ipxofs[dest]++]=(char)va_arg(ap,char);
		break;

		case NETWORK_MESSAGE:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			str=va_arg(ap,char *);
			for(c=0;c<MAX_MESSAGE_LENGTH;c++)
			 ipxstream[dest][ipxofs[dest]++]=*(str++);
		break;

		case NETWORK_BURNING:
			ipxstream[dest][ipxofs[dest]++]=(char) type;
			d=va_arg(ap,char);
			c=va_arg(ap,int);
			ipxstream[dest][ipxofs[dest]++]=(char) d;
			ipxstream[dest][ipxofs[dest]++]=(char)(c!=0? 1:0);
		break;

		case NETWORK_UPDKILLS:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			d=va_arg(ap,char);ipxstream[dest][ipxofs[dest]++]=d;
			d=va_arg(ap,char);ipxstream[dest][ipxofs[dest]++]=d;
			str=(char *) &player[d].player_kills;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
		break;

		case NETWORK_SENDCRATES:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			c= va_arg(ap,char);
			ipxstream[dest][ipxofs[dest]++]=(char) c;
		break;

		case NETWORK_SENDLEVEFFS:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			c= va_arg(ap,char);
			ipxstream[dest][ipxofs[dest]++]=(char) c;
		break;

		case NETWORK_SENDBULLETS:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			c= va_arg(ap,int);
			ipxstream[dest][ipxofs[dest]++]=(char) c;
		break;
				 
		case NETWORK_PLYXY:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			d=va_arg(ap,char);
			ipxstream[dest][ipxofs[dest]++]=(char)d;
			x=player[d].x;
			y=player[d].y;

			str=(char *)&x;
			for(c=0;c<sizeof(float);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
			str=(char *)&y;
			for(c=0;c<sizeof(float);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
    	break;

		case NETWORK_PLYRANGLE:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			d=va_arg(ap,char);
			ipxstream[dest][ipxofs[dest]++]=d;
			str=(char *)&player[d].rangle;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
    	break;

		case NETWORK_PLYANIM:
			ipxstream[dest][ipxofs[dest]++]=(char)type;
			d=va_arg(ap,char);
			ipxstream[dest][ipxofs[dest]++]=d;
			str=(char *)&player[d].ANIM;
			for(c=0;c<sizeof(int);c++) ipxstream[dest][ipxofs[dest]++]=*(str++);
    	break;

		default:error("sendipx: Command not implemented yet!\n");
	}
	docheckflush();
	va_end(ap);
void flushipx() 
{
	int a,b;
//	char jee[100];
	for(a=0; a<MAX_PLAYERS; a++) 
	if(player[a].enabled)
	{
//	sprintf(jee,"streami[%d][%d]",a,ipxofs[a]);
//	MBoard.add_message(jee);
	 if(ipxofs[a]!=0)
	  {
		ipxstream[a][ ipxofs[a]++]=NETWORK_END;
		for(b=0;b<ipxofs[a];b++)
		 send->data[b]=ipxstream[a][b];
	    while(send->ecb.inuse);
		sendpacket(sendseg,player[a].ipxnode,send,(unsigned short)(ipxofs[a]+1));
 // 		RESET
		ipxofs[a]=0;
	  }
	}
//	MBoard.add_message("flushed...");
void docheckflush()
{
	int a,b;
//	char jee[100];

	for(a=0; a<MAX_PLAYERS; a++) 
	if(player[a].enabled)
	{
	if(ipxofs[a]>(MAX_NETWORKDATA_SIZE-100))
	{
//		sprintf(jee,"streami[%d][%d]",a,ipxofs[a]);
//		MBoard.add_message(jee);
		ipxstream[a][ ipxofs[a]++]=NETWORK_END;
		for(b=0;b<ipxofs[a];b++)
		 send->data[b]=ipxstream[a][b];
	    while(send->ecb.inuse);
		sendpacket(sendseg,player[a].ipxnode,send,(unsigned short)(ipxofs[a]+1));
 // 		RESET
		ipxofs[a]=0;
	}
	}
//	MBoard.add_message("flushed2...");
}

}

}


void setup_ipx() 
{
	int a;
	if(opensocket(0,NETWORK_SOCKET)) 
		error("setup_ipx: Couldn't open socket(%Xh)\n",NETWORK_SOCKET);
	for(a=0; a<MAX_PLAYERS; a++) ipxofs[a]=0;
	if(NETWORK_MODE==ENetworkModeServer)
	{
		strcpy(servername,name1);
		for(a=0;a<6;a++)
		 serveraddr.node[a]=localadd->node.node[a];
	}
	initsendpacket(send,sendseg);
	for(a=0;a<NETWORK_RECEIVERS;a++)
	 {
		 initreceivepacket(rec[a],recseg[a]);
		 listenforpacket(recseg[a]);
	 }
	for(a=0;a<MAX_PLAYERS;a++)
	 ipxofs[a]=0;
}

void desetup_ipx() 
{	int a;

	flushipx();
	if((GAME_MODE==NETWORK)&&(NETWORK_MODE==ENetworkModeServer))
	 for(a=0;a<MAX_PLAYERS;a++)
	  if(player[a].enabled)
	   sendipxnow(&player[a].ipxnode,NETWORK_SERVERSHUTDOWN);
	for(a=0;a<NETWORK_RECEIVERS;a++)
	 if(rec[a]->ecb.inuse)
	  cancelpacket(recseg[a]);
	closesocket((unsigned short) socketid);
}

*/

char* CNetworkDevice::ReadProxyHTTP(char *proxyaddress,char *host, char *page, int proxyport, int* errval)
{
	IPaddress remote;
	TCPsocket sock;
	char buf[HTTPBUFFER];
	int result;

	*errval=0;

	if (SDLNet_ResolveHost(&remote, proxyaddress,proxyport)==-1)
	{
		*errval=1;
		ASSERT(EF_SNPRINTF(buf,HTTPBUFFER,"CNetworkDevice::ReadProxyHTTP: Couldn't resolve host! (%s)",host)>0);
		return strdup(buf);
	}

	sock = SDLNet_TCP_Open(&remote);

	if ( sock == NULL )
	{
		*errval=2;
		ASSERT(EF_SNPRINTF(buf,HTTPBUFFER,"CNetworkDevice::ReadProxyHTTP: Unable to open socket: %s\n", SDLNet_GetError())>0);
		return strdup(buf);
	}

	sprintf(buf,"GET http://%s/%s HTTP/1.0%c%c",host,page,0xa,0xa);
	result=SDLNet_TCP_Send(sock,buf,strlen(buf)+1);

	if(result<strlen(buf)+1) 
	{
		SDLNet_TCP_Close(sock);
		*errval=3;
		ASSERT(EF_SNPRINTF(buf,HTTPBUFFER,"CNetworkDevice::ReadProxyHTTP: SDLNet_TCP_Send: %s\n", SDLNet_GetError())>0);
		return strdup(buf);
	}

	result=SDLNet_TCP_Recv(sock,buf,HTTPBUFFER);

	if(result<=0) 
	{
		SDLNet_TCP_Close(sock);
		*errval=4;
		ASSERT(EF_SNPRINTF(buf,HTTPBUFFER,"CNetworkDevice::ReadProxyHTTP: SDLNet_TCP_Recv: %s\n", SDLNet_GetError())>0);
		return strdup(buf);
	}

	SDLNet_TCP_Close(sock);
	return strdup(buf);
}

char* CNetworkDevice::ReadHTTP(char *host,char *page,int port, int* errval)
{
	IPaddress remote;
	TCPsocket sock;
	char buf[HTTPBUFFER];
	int result;

	*errval=0;

	if (SDLNet_ResolveHost(&remote, host,port)==-1)
	{
		*errval=1;
		ASSERT(EF_SNPRINTF(buf,HTTPBUFFER,"CNetworkDevice::ReadHTTP: Couldn't resolve host! (%s)",host)>0);
		return strdup(buf);
	}

	sock = SDLNet_TCP_Open(&remote);

	if ( sock == NULL )
	{
		*errval=2;
		ASSERT(EF_SNPRINTF(buf,HTTPBUFFER,"CNetworkDevice::ReadHTTP: Unable to open socket: %s\n", SDLNet_GetError())>0);
		return strdup(buf);
	}

	sprintf(buf,"GET %s HTTP/1.1\nHost: %s\n\n",page,host);
	result=SDLNet_TCP_Send(sock,buf,strlen(buf)+1);

	if(result<strlen(buf)+1) 
	{
		SDLNet_TCP_Close(sock);
		*errval=3;
		ASSERT(EF_SNPRINTF(buf,HTTPBUFFER,"CNetworkDevice::ReadHTTP: SDLNet_TCP_Send: %s\n", SDLNet_GetError())>0);
		return strdup(buf);
	}

	result=SDLNet_TCP_Recv(sock,buf,HTTPBUFFER);

	if(result<=0) 
	{
		SDLNet_TCP_Close(sock);
		*errval=4;
		ASSERT(EF_SNPRINTF(buf,HTTPBUFFER,"CNetworkDevice::ReadHTTP: SDLNet_TCP_Recv: %s\n", SDLNet_GetError())>0);
		return strdup(buf);
	}

	SDLNet_TCP_Close(sock);
	return strdup(buf);
}
