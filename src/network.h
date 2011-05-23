#ifndef __NETWORK_H_
#define __NETWORK_H_

#include "common/CLevel.h"

enum TNetworkMessage
{
	NETWORK_END=0,
    NETWORK_SERVERSEARCH,
    NETWORK_TEXTMESSAGE,
    NETWORK_SERVERANSWER,
    NETWORK_SERVERSHUTDOWN,
    NETWORK_JOINREQUEST,
    NETWORK_JOINACCEPT,
    NETWORK_SENDLEVELINFO,
    NETWORK_SENDLEVEL,
    NETWORK_LEVELINFO,
    NETWORK_TRANSMITLEVEL,
    NETWORK_GAMEDATA,
    NETWORK_IMJOININGNOW,
    NETWORK_PLYXY,
    NETWORK_PLYRANGLE,
    NETWORK_ALIVE,
    NETWORK_PLYANIM,
    NETWORK_SENDCRATES,
    NETWORK_UPDCRATE,
    NETWORK_UPDKILLS,
    NETWORK_NEWBULLET,
    NETWORK_BURNING,
    NETWORK_MESSAGE,
    NETWORK_SOUND,
    NETWORK_STEAMPACK,
    NETWORK_LIGHTPACK,
    NETWORK_SENDLEVEFFS,
    NETWORK_NEWEFFECT,
    NETWORK_SENDBULLETS,
    NETWORK_BULLET,
    NETWORK_NEWPLAYERDATA,
    NETWORK_PLAYERLEFTGAME,
    NETWORK_CAMETOGAME,
	NETWORK_NOMESSAGE
};

const int NETWORK_SOCKET=0x8691;

const int NETWORK_MESSAGE_LENGTH[NETWORK_NOMESSAGE] = 
{
	1,  // END
	1,  // SERVERSEARCH
	20, // TEXTMESSAGE (text)
	11, // SERVERANSWER (name)
	1,  // SERVERSHUTDOWN
	1,  // JOINREQUEST
	2,  // JOINACCEPT (1/0)
	1,  // SENDLEVELINFO
	3,  // SENDLEVEL (short packetno)
	4+sizeof(CLevel::TGeneralLevelInfo),  // LEVELINFO
	0, // TRANSMITLEVEL
	0, // GAMEDATA
	0, // IMJOININGNOW
	10,// PLYXY
	5, // PLYRANGLE
	1,  // ALIVE
	5, // PLYANIM
	2, // SENDCRATES
	0, // UPDCRATE
	0, // UPDKILLS
	0, //  NEWBULLET
	0, // NETWORK_BURNING
	0, // NETWORK_MESSAGE
	0, // NETWORK_SOUND
	0, // NETWORK_STEAMPACK
	0, // NETWORK_LIGHTPACK
	0, // NETWORK_SENDLEVEFFS
	0, // NETWORK_NEWEFFECT
	0, // NETWORK_SENDBULLETS
	0, // NETWORK_BULLET
	0, // NETWORK_NEWPLAYERDATA
	0, // NETWORK_PLAYERLEFTGAME
	0, // NETWORK_CAMETOGAME
};

class CNetworkAddress
{
public:
	CNetworkAddress& operator =(const CNetworkAddress* aAddr);
	const char* GetHostname() const;
	const char* GetAddress() const;
};

class CNetworkDevice
{
public:
	CNetworkDevice();
	~CNetworkDevice();
	void Send(int dest, int type, ... );
	void SendNow(const CNetworkAddress* dest, int type, ... );
	int Available();
	const CNetworkAddress* BroadCastAddress();
	const CNetworkAddress* LocalAddress();

	char* ReadHTTP(char *host,char *page,int port, int* error);
	char* ReadProxyHTTP(char *proxyaddress,char *host, char *page, int proxyport, int* error);

	CNetworkAddress* iServerAddress;
	CNetworkAddress* iLocalAddress;
	CNetworkAddress* iBroadCastAddress;
};

#endif


