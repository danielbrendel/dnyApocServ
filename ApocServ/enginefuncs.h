#ifndef _ENGINEFUNCS_H
#define _ENGINEFUNCS_H

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: enginefuncs.h
	Purpose: Engine function interface
*/

#include "sdkinclude.h"
#include "global.h"

#ifdef __linux__
#define OPERATINGSYSTEM "Linux"
#define SSH_CONFIG "cstrike/dlls/dnyApocServ.cfg"
#define EXPORT_LIBRARY extern "C"
#define FNPAPI
#define ENTAPI
#define NEWAPI
#define INTERNAPI
#define LMODULE "cstrike/dlls/cs_i386.so"
#elif _WIN32
#define OPERATINGSYSTEM "Win32"
#define SSH_CONFIG "cstrike\\dlls\\dnyApocServ.cfg"
#define EXPORT_LIBRARY
#define FNPAPI __stdcall
#define ENTAPI __cdecl
#define NEWAPI __cdecl
#define INTERNAPI __cdecl
#define LMODULE "cstrike\\dlls\\mp.dll"
#define ZMODULE "cstrike\\dlls\\zbotcz.dll"
#endif

//======================================================================
typedef int (ENTAPI *T_GetEntityAPI)(DLL_FUNCTIONS* pFunctionTable, int interfaceVersion);
typedef int (NEWAPI *T_GetNewDLLFunctions)(NEW_DLL_FUNCTIONS* pNewFunctionTable, int* pinterfaceVersion);
typedef void (FNPAPI *T_GiveFnptrsToDll)(enginefuncs_s* pengfuncsFromEngine, globalvars_t* pGlobals);
typedef const char* (INTERNAPI *TGetPlayerAuthId)(edict_s* ed);
//======================================================================

//======================================================================
extern T_GetEntityAPI orig_GetEntityAPI;
extern T_GetNewDLLFunctions orig_GetNewDLLFunctions;
extern T_GiveFnptrsToDll orig_GiveFnptrsToDll;
//======================================================================

#endif
