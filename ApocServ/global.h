#ifndef _GLOBAL_H
#define _GLOBAL_H

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: global.h
	Purpose: Global interface
*/

#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <dlfcn.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "sdkinclude.h"

#include "player.h"

//======================================================================
extern void* g_hOrigModule; //Handle to original game mod library
extern char g_AdminPass[250]; //Buffer containing the authentication password

extern enginefuncs_s* g_pEngFuncs;
extern enginefuncs_s g_EngBackup;
extern DLL_FUNCTIONS* g_pDllFuncs;
extern DLL_FUNCTIONS g_DllBackup;
extern globalvars_t* g_pGlobalVars;

extern bool g_bServerStarted;
//======================================================================

//======================================================================
bool ReadPatchLen(const char* szCfg, int* poplen);
void GlobalEmitSound(const char *szSoundFile);
void GlobalEmitAmbientSound(float* pos, const char *szSoundFile);
void CreateExplosion(vec3_t pOrigin);
void PrintSayText(edict_s*pEnt, const char*msg, const char*arg1, const char*arg2, const char*arg3);
bool isValidPlayer(playerinfo_s *pInfo);
void SendMotdMsg(const char* szMotdMsg, edict_s* pEdict);
void PlayStepNearPlayer(edict_s* pEdict);
void SlapPlayer(edict_s* pEdict, float fHealthToReduce);
void BlindPlayer(edict_s* pEdict);
bool SpawnItemToPlayer(edict_s* pEdict, const char* szItemName);
bool SpawnZone(const char* szZoneName, float* pos);
edict_s* SpawnEntity(const char* szClassName, float* pos);
void SetPlayerGlow(edict_s* pEdict, float ramt, float fr, float fg, float fb);
//======================================================================

#endif

