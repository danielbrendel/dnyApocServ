#include "player.h"
#include "global.h"

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: player.cpp
	Purpose: Player class implementation
*/

//======================================================================
//Check for defined cheat variable exists in current line as "name"
//If found copy "value" to member as defined (either int(CVT_INTEGER), float(CVT_FLOAT) or char[](CVT_STRING))
#define CHECK_CVAR(n, m, t) if (strstr(szLine, n)) { if (t == CVT_INTEGER) pInfo->cvars.##m = atoi(lPtr); else if (t == CVT_FLOAT) pInfo->cvars.##m = atof(lPtr); else if (t == CVT_STRING) strncpy((char*)pInfo->cvars.##m, lPtr, strlen(lPtr)); continue; } 
//======================================================================

//======================================================================
playerinfo_s* CPlayers::GetPlayerByEdict(edict_s* pEntity)
{
	//Returns a player pointer by edict pointer
	if (!pEntity) return NULL;

	for (int i = 1; i < 33; i++) { //Walk trough entries
		if (gPlayer[i-1].pEnt == pEntity) //If found then return address of entry
			return &gPlayer[i-1];
	}

	return NULL;
}
//======================================================================

//======================================================================
playerinfo_s* CPlayers::GetPlayerById(int id)
{
	//Return a player pointer by unique ID
	if (!id) return NULL;

	for (int i = 1; i < 33; i++) { //Walk trough entries
		if ((i-1 == id-1) && (gPlayer[i-1].bConnected)) //If found and player is connected return address of entry
			return &gPlayer[i-1];
	}

	return NULL;
} 
//======================================================================

//======================================================================
playerinfo_s* CPlayers::GetPlayerBySteamID(const char *szSteamID)
{
	//Return a player pointer by SteamID
	if (!szSteamID) return NULL;

	for (int i = 1; i < 33; i++) { //Walk trough entries
		if (strcmp(gPlayer[i-1].SteamID, szSteamID)==0) //If found return address of entry
			return &gPlayer[i-1];
	}

	return NULL;
}
//======================================================================

//======================================================================
playerinfo_s* CPlayers::GetPlayerByName(char *szName)
{
	//Return a player pointer by Name
	if (!szName) return NULL;

	for (int i = 1; i < 33; i++) { //Walk trough entries
		if (strcmp(gPlayer[i-1].Name, szName)==0) //If found return address of entry
			return &gPlayer[i-1];
	}

	return NULL;
}
//======================================================================

//======================================================================
playerinfo_s* CPlayers::GetPlayerByAddress(char *szAddress)
{
	//Return a player pointer by Address
	if (!szAddress) return NULL;

	for (int i = 1; i < 33; i++) { //Walk trough entries
		if (strcmp(gPlayer[i-1].Address, szAddress)==0) //If found return address of entry
			return &gPlayer[i-1];
	}

	return NULL;
}
//======================================================================

//======================================================================
void CPlayers::Initialize(edict_s* pEdictList)
{
	//Initialize all player entries
	for (int i = 1; i <= g_pGlobalVars->maxClients; i++) { //Walk trough entries
		memset(gPlayer[i-1].SteamID, 0x00, 250);
		memset(gPlayer[i-1].Address, 0x00, 250);
		memset(gPlayer[i-1].Name, 0x00, 250);
		memset(&gPlayer[i-1].cvars, 0x00, sizeof(cvar_s));
		gPlayer[i-1].pEnt = &pEdictList[i]; //Save edict pointer
		gPlayer[i-1].bConnected = false;
		gPlayer[i-1].bAuthed = false;
	}
}
//======================================================================

//======================================================================
void CPlayers::Connect(edict_s* pEntity, const char* szName, const char* szAddress, char szRejectReason[128])
{
	//Called if a client connects to the server
	playerinfo_s* pInfo = GetPlayerByEdict(pEntity);
	if (pInfo) {
		//Store data
		strcpy(pInfo->Address, szAddress);
		strcpy(pInfo->SteamID, g_pEngFuncs->pfnGetPlayerAuthId(pEntity));
		strcpy(pInfo->Name, szName);
		memset(&pInfo->cvars, 0x00, sizeof(cvar_s));
		pInfo->bConnected = true;
		pInfo->bAuthed = false;
	}
}
//======================================================================

//======================================================================
void CPlayers::PutInServer(edict_s* pEntity)
{
	//Called if a client connects to the server
	playerinfo_s* pInfo = GetPlayerByEdict(pEntity);
	if ((!pInfo) && (pEntity->v.flags & FL_FAKECLIENT)) {
		this->Connect(pEntity, "Bot", "127.0.0.1", (char*)"");
	}
}
//======================================================================

//======================================================================
void CPlayers::Disconnect(edict_s* pEntity)
{
	//Called if a client disconnects from server
	playerinfo_s* pInfo = GetPlayerByEdict(pEntity);
	if (pInfo) {
		//Clear all data
		memset(pInfo->SteamID, 0x00, 250);
		memset(pInfo->Address, 0x00, 250);
		memset(pInfo->Name, 0x00, 250);
		memset(&pInfo->cvars, 0x00, sizeof(cvar_s));
		pInfo->bConnected = false;
		pInfo->bAuthed = false;
	}
}
//======================================================================

//======================================================================
void CPlayers::RefreshName(edict_s* pEntity, char* szInfoBuffer)
{
	//Refresh player name when name has changed
	if ((pEntity) && (szInfoBuffer)) {
		char *namestr = strstr(szInfoBuffer, "\\name\\"); //Get to name info section
		if (namestr) {
			namestr += 6; //Points now to the new name

			playerinfo_s *pInfo = GetPlayerByEdict(pEntity);
			if ((pInfo) && (!strstr(namestr, pInfo->Name))){ //If name is not equal to old name
				//Set new name
				memset(pInfo->Name, 0x00, 250);
				strcpy(pInfo->Name, namestr);
			}
		}
	}
}
//======================================================================

//======================================================================
void CPlayers::RefreshTeam(edict_s* pEntity, char* szInfoBuffer)
{
	//Refresh player team  when team has changed
	if ((pEntity) && (szInfoBuffer)) {
		char* modelstr = strstr(szInfoBuffer, "\\model\\"); //Get to model info section
		if (modelstr) {
			modelstr += 7; //Points now to the new model

			player_team_e team = PLAYER_TEAM_SPEC;
			if ((strstr(modelstr, "arctic")) || (strstr(modelstr, "guerilla")) || (strstr(modelstr, "leet")) || (strstr(modelstr, "militia")) || (strstr(modelstr, "terror"))) {
				team = PLAYER_TEAM_T;
			} else if ((strstr(modelstr, "gign")) || (strstr(modelstr, "gsg9")) || (strstr(modelstr, "sas")) || (strstr(modelstr, "spetsnaz")) || (strstr(modelstr, "urban"))) {
				team = PLAYER_TEAM_CT;
			}

			playerinfo_s* pInfo = GetPlayerByEdict(pEntity);
			if ((pInfo) && (pInfo->Team != team)) { //If team is not equal to old team
				//Set new team
				pInfo->Team = team;
			}
		}
	}
}
//======================================================================

//======================================================================
bool CPlayers::ExecForSteamID(const char *szSteamID, char *szConfig)
{
	//Execute settings for client by a config file
	bool bResult = false;

	if ((szSteamID) && (szConfig)) {
		playerinfo_s* pInfo = GetPlayerBySteamID(szSteamID);
		if (pInfo) {
			std::ifstream hFile;
			hFile.open(szConfig, std::ifstream::in); //Open file
			if (hFile.good()) { //If openend
				char szLine[250];
				char* lPtr = NULL;

				while (!hFile.eof()) { //Enumerate trough all lines
					hFile.getline(szLine, sizeof(szLine), '\n');
					
					if ((szLine[0] == '#') || (szLine[0] == 0)) continue; //Filter comments and empty lines

					//Line format:
					//name=value
					lPtr = strstr(szLine, "="); lPtr++; //Point to "value"

					//Check all cvars here.
					//Add new cvars if you want
					//CVar types:
					//	If your var is an integer then use CVT_INTEGER
					//	If your var is a float then use CVT_FLOAT
					//	If your var is a string buffer then use CVT_STRING
					CHECK_CVAR("ssh_bunnyhop", bunnyhop, CVT_INTEGER);
				}

				hFile.close(); //Close file
				bResult = true; //Success
			}
		}
	}

	return bResult;
}
//======================================================================

//======================================================================
bool CPlayers::ConfigReadyForPlayer(const char* szSteamID)
{
	//Check if a config is registered for this SteamID
	bool bResult = false;

	if ((szSteamID) && (strlen(szSteamID) < MAX_STEAMID_STRING)) {
		for (unsigned int i = 0; i < vConfigs.size(); i++) { //Walk trough all entries
			if (strcmp(vConfigs[i].szSteamID, szSteamID) == 0) { //If SteamID matches we have succeeded
				bResult = true;
				break;
			}
		}
	}

	return bResult;
}
//======================================================================

//======================================================================
char* CPlayers::GetConfigNameForSteamID(const char* szSteamID)
{
	//Get config name for SteamID if exists
	if ((!szSteamID) || (strlen(szSteamID) > MAX_STEAMID_STRING)) return NULL;

	for (unsigned int i = 0; i < vConfigs.size(); i++) { //Walk trough entries
		if (strcmp(vConfigs[i].szSteamID, szSteamID) == 0) //If SteamID matches return config name string address
			return vConfigs[i].szConfig;
	}

	return NULL;
}
//======================================================================

//======================================================================
bool CPlayers::LoadAllSteamIDConfigs(const char* szLoadFromFile)
{
	//Load all SteamID profiles into a list
	bool bResult = false;

	if ((szLoadFromFile) && (strlen(szLoadFromFile) < MAX_STEAMID_STRING)) {
		std::ifstream hFile;
		//Open file for reading
		hFile.open(szLoadFromFile, std::ifstream::in);
		if (hFile.good()) { //If opened
			char szLine[MAX_STEAMID_STRING];
			char* lPtr = NULL;

			while (!hFile.eof()) { //Goto each line
				hFile.getline(szLine, sizeof(szLine), '\n'); //Read current line

				if ((szLine[0] == 0) || (szLine[0] == '#')) continue; //Filter comments or empty lines

				char *lPtr = strstr(szLine, "="); //Point to the '=' char in line
				//Add to list
				steamid_cfg_s cfg;
				memset(&cfg, 0x00, sizeof(steamid_cfg_s));
				strncpy(cfg.szSteamID, szLine, (lPtr - &szLine[0])); //Copy SteamID name (chars until '=' reached)
				strcpy(cfg.szConfig, lPtr + 1); //Copy config file name (chars from '=' + 1
				vConfigs.push_back(cfg);
			}

			hFile.close(); //Close file
			bResult = true;
		}
	}

	return bResult;
}
//======================================================================

//======================================================================
CPlayers gPlayers;
//======================================================================
