#include "hooks.h"
#include "global.h"

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See README.md file for more details !

	File: hooks.cpp
	Purpose: Hooked system function callback implementations
*/

#ifdef __linux__
//======================================================================
T_dlsym orig_dlsym;
//======================================================================

//======================================================================
void* new_dlsym(void* hModule, char* lpProcName)
{
	void* pRet;

	pRet = orig_dlsym(hModule, lpProcName);
	if (!pRet) pRet = orig_dlsym(hOrigModule, lpProcName);

	return pRet;
}
//======================================================================

#else

//======================================================================
T_GetProcAddress orig_GetProcAddress;
//======================================================================

//======================================================================
void* WINAPI new_GetProcAddress(HMODULE hModule, char *lpProcName)
{
	void* pRet;

	DWORD p;
	VirtualProtect(orig_GetProcAddress, 10, PAGE_EXECUTE_READWRITE, &p);
	pRet = orig_GetProcAddress(hModule, lpProcName);
	if (!pRet) pRet = orig_GetProcAddress((HMODULE)g_hOrigModule, lpProcName);
	VirtualProtect(orig_GetProcAddress, 10, p, &p);

	return pRet;
}
//======================================================================

#endif