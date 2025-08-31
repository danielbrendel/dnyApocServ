#ifndef _HOOKS_H
#define _HOOKS_H

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See README.md file for more details !

	File: hooks.h
	Purpose: Hook Callback interface
*/

#include "global.h"

#ifdef __linux__

//======================================================================
typedef void* (*T_dlsym)(void* hModule, char* lpProcName);
//======================================================================

//======================================================================
extern T_dlsym orig_dlsym;
//======================================================================

//======================================================================
void* new_dlsym(void* hModule, char* lpProcName);
//======================================================================

#elif WIN32

#include <windows.h>

//======================================================================
typedef void* (WINAPI *T_GetProcAddress)(HMODULE hModule, char *lpProcName);
//======================================================================

//======================================================================
extern T_GetProcAddress orig_GetProcAddress;
//======================================================================

//======================================================================
void* WINAPI new_GetProcAddress(HMODULE hModule, char *lpProcName);
//======================================================================

#endif

#endif
