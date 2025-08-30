#ifndef _DETOUR_H
#define _DETOUR_H

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: detour.h
	Purpose: Detour hooking interface
*/

//======================================================================
#ifdef __linux__
void* detour_func(unsigned char* src, const unsigned char* dst, const int len);
#else
void* JMPDetour(void*OrigAddr, void*NewAddr, unsigned int uiSize);
#endif
//======================================================================

#endif
