#ifndef _PLAYER_H
#define _PLAYER_H

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: player.h
	Purpose: Player interface
*/

#include "sdkinclude.h"

//======================================================================
//If not already defined. Maximum amount of players for engine placed here
#ifndef MAX_CLIENTS
#define MAX_CLIENTS 32
#endif

#define MAX_STEAMID_STRING 250
//======================================================================

//======================================================================
enum cvar_type_e {
	CVT_INTEGER,
	CVT_FLOAT,
	CVT_STRING
};

enum player_team_e {
	PLAYER_TEAM_SPEC = 0,
	PLAYER_TEAM_T,
	PLAYER_TEAM_CT
};
//======================================================================

//======================================================================
struct cheatvars_s {
	//Add new cheat variables here.
	//Supported object types: int(CVT_INTEGER), float(CVT_FLOAT) and char[](CVT_STRING)
	int bunnyhop;
	int noclip;
	int invincibility;
	int transparency;
	int healthcare;
	int maxhealth;
	float gravity;
	int traceaim;
	int selplayer;
	vec3_t savedpos;
};

struct playerinfo_s {
	char SteamID[250]; //SteamID of client
	char Address[250]; //IP-Address of client
	char Name[250]; //Name of client
	player_team_e Team; //Player team
	edict_s* pEnt; //Edict pointer
	bool bConnected; //Is client connected?
	bool bAuthed; //Is client authed by console command?
	cheatvars_s cvars; //All cheat variables
};

struct steamid_cfg_s {
	char szSteamID[MAX_STEAMID_STRING];
	char szConfig[MAX_STEAMID_STRING];
};
//======================================================================

//======================================================================
class CPlayers {
private:
	playerinfo_s gPlayer[MAX_CLIENTS];
	std::vector<steamid_cfg_s> vConfigs;
public:
	~CPlayers() { vConfigs.clear(); }
	playerinfo_s* GetPlayerByEdict(edict_s* pEnt);
	playerinfo_s* GetPlayerById(int id);
	playerinfo_s* GetPlayerBySteamID(const char *szSteamID);
	playerinfo_s* GetPlayerByName(char *szName);
	playerinfo_s* GetPlayerByAddress(char *szAddress);
	void Initialize(edict_s* pEdictList);
	void Connect(edict_s* pEntity, const char* szName, const char* szAddress, char szRejectReason[128]);
	void PutInServer(edict_s* pEntity);
	void Disconnect(edict_s* pEntity);
	void RefreshName(edict_s* pEntity, char* szInfoBuffer);
	void RefreshTeam(edict_s* pEntity, char* szInfoBuffer);
	bool ExecForSteamID(const char *szSteamID, char *szConfig);
	bool LoadAllSteamIDConfigs(const char* szLoadFromFile);
	bool ConfigReadyForPlayer(const char* szSteamID);
	char* GetConfigNameForSteamID(const char* szSteamID);
};
//======================================================================

//======================================================================
extern CPlayers gPlayers;
//======================================================================

#endif
