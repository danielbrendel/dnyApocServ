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

#define CSVER_CZERO

#ifdef __linux__
#define OPERATINGSYSTEM "Linux"
#define EXPORT_LIBRARY extern "C"
#define FNPAPI
#define ENTAPI
#define NEWAPI
#define INTERNAPI
#if defined(CSVER_CSTRIKE)
#define SSH_CONFIG "cstrike/dlls/ApocServ.cfg"
#define LMODULE "cstrike/dlls/cs_i386.so"
#elif defined(CSVER_CZERO)
#define SSH_CONFIG "czero/dlls/ApocServ.cfg"
#define LMODULE "czero/dlls/cs_i386.so"
#endif
#elif _WIN32
#define OPERATINGSYSTEM "Win32"
#define EXPORT_LIBRARY
#define FNPAPI __stdcall
#define ENTAPI __cdecl
#define NEWAPI __cdecl
#define INTERNAPI __cdecl
#if defined(CSVER_CSTRIKE)
#define SSH_CONFIG "cstrike\\dlls\\ApocServ.cfg"
#define LMODULE "cstrike\\dlls\\mp.dll"
#define ZMODULE "cstrike\\dlls\\zbotcz.dll"
#elif defined(CSVER_CZERO)
#define SSH_CONFIG "czero\\dlls\\ApocServ.cfg"
#define LMODULE "czero\\dlls\\mp.dll"
#define ZMODULE "czero\\dlls\\zbotcz.dll"
#endif
#endif

#if defined(CSVER_CSTRIKE)
#define HLMOD "cstrike"
#elif defined(CSVER_CZERO)
#define HLMOD "czero"
#else
#error "You need to define either CSVER_CSTRIKE or CSVER_CZERO"
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
