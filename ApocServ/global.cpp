#include "global.h"
#ifdef __linux__
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#elif _WIN32
#include <windows.h>
#endif

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: global.cpp
	Purpose: Global variables and function implementations
*/

#include <stdarg.h>
#include <time.h> 
#include <fstream>
#include <iostream>

//======================================================================
char g_AdminPass[250];

void* g_hOrigModule;

enginefuncs_t* g_pEngFuncs;
enginefuncs_s g_EngBackup;
DLL_FUNCTIONS* g_pDllFuncs;
DLL_FUNCTIONS g_DllBackup;
globalvars_t* g_pGlobalVars;

bool g_bServerStarted = false;
//======================================================================

//======================================================================
bool ReadPatchLen(const char* szCfg, int* poplen)
{
	//Get Byte amount to patch from config

	#ifdef _WIN32
	#define GPA_OPLEN_IDENTIFIER "gpaoplen_win"
	#elif __linux__
	#define GPA_OPLEN_IDENTIFIER "gpaoplen_linux"
	#endif
	
	if ((!szCfg) || (!poplen)) return false;

	bool result = false;
	
	std::ifstream hFile;
	hFile.open(szCfg, std::ifstream::in); //Open config file for reading
	if (hFile.good()) { //If opened file
		char szLine[250];
		char *lptr = NULL;
		
		while (!hFile.eof()) { //Walk trough each line
			hFile.getline(szLine, sizeof(szLine), '\n');

			if (szLine[0] == '#') continue; //Filter comments

			if (strstr(szLine, GPA_OPLEN_IDENTIFIER)) { //If identifier found
				lptr = strstr(szLine, "="); lptr++; //Point to "value"
				*poplen = atoi(lptr); //Write value to pointed integer

				result = true; //Job done

				break;
			}
		}

		hFile.close(); //Close file
	}

	return result;
}
//======================================================================

//======================================================================
void GlobalEmitSound(const char *szSoundFile)
{
	//Globally emit a normal sound to each player
	for (unsigned char i = 1; i < 33; i++) {
		playerinfo_s *pInfo = gPlayers.GetPlayerById(i);
		if ((pInfo) && (pInfo->bConnected)) 
			g_pEngFuncs->pfnEmitSound(pInfo->pEnt, 3, szSoundFile, 1.0f, 0.8f, 0, 101);
	}
}
//======================================================================

//======================================================================
void GlobalEmitAmbientSound(float* pos, const char *szSoundFile)
{
	//Globally emit a 3D sound to each player
	for (unsigned char i = 1; i < 33; i++) {
		playerinfo_s *pInfo = gPlayers.GetPlayerById(i);
		if ((pInfo) && (pInfo->bConnected)) 
			g_pEngFuncs->pfnEmitAmbientSound(pInfo->pEnt, pos, szSoundFile, 1.0f, 0.8f, 0, 101);
	}
}
//======================================================================

//======================================================================
void CreateExplosion(vec3_t pOrigin)
{
	//Send explosion message (temporary entity)
	g_pEngFuncs->pfnMessageBegin(MSG_BROADCAST, 23, NULL, NULL);
	g_pEngFuncs->pfnWriteByte(4);
	g_pEngFuncs->pfnWriteCoord(pOrigin[0]);
	g_pEngFuncs->pfnWriteCoord(pOrigin[1]);
	g_pEngFuncs->pfnWriteCoord(pOrigin[2]);
	g_pEngFuncs->pfnMessageEnd();
}
//======================================================================

//======================================================================
void PrintSayText(edict_s*pEnt, const char*msg, const char*arg1, const char*arg2, const char*arg3)
{
	//Send message to print a chat text message
	g_pEngFuncs->pfnMessageBegin(MSG_ONE, 76, NULL, pEnt);
	g_pEngFuncs->pfnWriteByte(0);
	g_pEngFuncs->pfnWriteString(msg);
	g_pEngFuncs->pfnWriteString(arg1);
	g_pEngFuncs->pfnWriteString(arg2);
	g_pEngFuncs->pfnWriteString(arg3);
	g_pEngFuncs->pfnMessageEnd();
}
//======================================================================

//======================================================================
bool isValidPlayer(playerinfo_s *pInfo)
{
	//Return if player is valid and playing
	return ((pInfo != NULL) && (pInfo->bConnected) && (pInfo->pEnt != NULL) && (!(pInfo->pEnt->v.flags & FL_SPECTATOR)));
}
//======================================================================

//======================================================================
void SendMotdMsg(const char* szMotdMsg, edict_s *pEdict)
{
	//Send a MOTD to a client
	if ((szMotdMsg) && (pEdict)) {
		g_pEngFuncs->pfnMessageBegin(MSG_ONE, 89, NULL, pEdict);
		g_pEngFuncs->pfnWriteByte(1);
		g_pEngFuncs->pfnWriteString(szMotdMsg);
		g_pEngFuncs->pfnMessageEnd();
	}
}
//======================================================================

//======================================================================
void PlayStepNearPlayer(edict_s* pEdict)
{
	//Play a step sound near a player
	if (pEdict) {
		const int MAX_X_POS = 500; const int MAX_Y_POS = 500;

		char buf[250];
		float pos[3];
			
		//Calculate random step sound
		srand(time(NULL));
		int irand = rand()%5;
		if (irand == 0) irand = 1;
		sprintf(buf, "player/pl_step%d.wav", irand);
			
		//Set the origins
		pos[3] = pEdict->v.origin[3];
		pos[2] = pEdict->v.origin[2];
		pos[1] = pEdict->v.origin[1];

		//Calculate random distances related to players origin (x, y)
		int irxm = rand()%1;
		int irym = rand()%1;
		int ipx = rand()%MAX_X_POS;
		int ipy = rand()%MAX_Y_POS;

		if (!irxm) ipx *= (-1);
		if (!irym) ipy *= (-1);

		pos[2] += ipy;
		pos[1] += ipx;

		//Play sound
		g_pEngFuncs->pfnEmitAmbientSound(pEdict, &pos[0], buf, 1.0f, 0.8f, 0, 101);
	}
}
//======================================================================

//======================================================================
void SlapPlayer(edict_s* pEdict, float fHealthToReduce)
{
	//Slap a player

	if (pEdict) {
		//Reduce health
		if (pEdict->v.health > fHealthToReduce)
			pEdict->v.health -= fHealthToReduce;
			
		//Let him jump into random directions with high z value
		pEdict->v.velocity[2] += 400.0f;
		pEdict->v.velocity[0] += -100 + rand()%200;
		pEdict->v.velocity[1] += -100 + rand()%200;
				
		//Send message to shake his screen
		g_pEngFuncs->pfnMessageBegin(MSG_ONE, 97, NULL, pEdict);
		g_pEngFuncs->pfnWriteShort(1<<12);
		g_pEngFuncs->pfnWriteShort(1<<12);
		g_pEngFuncs->pfnWriteShort(1<<12);
		g_pEngFuncs->pfnMessageEnd();
				
		//Emit a pain sound
		GlobalEmitAmbientSound((float*)&pEdict->v.origin[0], "player/pl_pain2.wav");
	}
}
//======================================================================

//======================================================================
void BlindPlayer(edict_s* pEdict)
{
	//Temporarily blind a player

	if (pEdict) {
		//Send blind message
		g_pEngFuncs->pfnMessageBegin(MSG_ONE, 98, NULL, pEdict);
		g_pEngFuncs->pfnWriteShort(10<<12);
		g_pEngFuncs->pfnWriteShort(10<<12);				
		g_pEngFuncs->pfnWriteShort(0x0000);
		g_pEngFuncs->pfnWriteByte(211);
		g_pEngFuncs->pfnWriteByte(211);
		g_pEngFuncs->pfnWriteByte(211);
		g_pEngFuncs->pfnWriteByte(255);
		g_pEngFuncs->pfnMessageEnd();
	}
}
//======================================================================

//======================================================================
bool SpawnItemToPlayer(edict_s* pEdict, const char* szItemName)
{
	//Spawn an item to player(weapons, bomb, etc)

	bool bResult = false;

	if ((pEdict) && (szItemName)) {
		int ialloc = g_pEngFuncs->pfnAllocString(szItemName); //Allocate item string
		if (ialloc) {
			edict_s* pItem = g_pEngFuncs->pfnCreateNamedEntity(ialloc); //Create the item entity by name
			if (pItem) {
				g_pDllFuncs->pfnSpawn(pItem); //Spawn item to world
				g_pDllFuncs->pfnTouch(pItem, pEdict); //Connect item with player

				bResult = true;
			}
		}
	}

	return bResult;
}
//======================================================================

//======================================================================
bool SpawnZone(const char* szZoneName, float* pos)
{
	//Spawn a zone entity

	bool bResult = false;

	if ((szZoneName) && (pos)) {
		int ialloc = g_pEngFuncs->pfnAllocString(szZoneName); //Allocate entity string
		if (ialloc) {
			edict_s* pZone = g_pEngFuncs->pfnCreateNamedEntity(ialloc); //Create the zone entity by name
			if (pZone) {
				g_pDllFuncs->pfnSpawn(pZone); //Spawn item to world
				g_pEngFuncs->pfnSetOrigin(pZone, pos); //Set desired position

				bResult = true;
			}
		}
	}

	return bResult;
}
//======================================================================

//======================================================================
void SetPlayerGlow(edict_s* pEdict, float ramt, float fr, float fg, float fb)
{
	//Create a glow around a player
	if (pEdict) {
		pEdict->v.renderfx = kRenderFxGlowShell; //Glow constant
		pEdict->v.renderamt = ramt; //The glows thickness
		pEdict->v.rendercolor[0] = fr; //Color red
		pEdict->v.rendercolor[1] = fg; //Color green
		pEdict->v.rendercolor[2] = fb; //Color blue
	}
}
//======================================================================