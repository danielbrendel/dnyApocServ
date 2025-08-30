#ifndef _SDKINCLUDE_H
#define _SDKINCLUDE_H

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: sdkinclude.h
	Purpose: All important includes
*/

//======================================================================
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "sdk/mathlib.h"
#include "sdk/eiface.h"
#include "sdk/progdefs.h"
#include "sdk/edict.h"
#include "sdk/usercmd.h"
#include "sdk/vector.h"

#ifndef qboolean
#define qboolean int
#endif

#ifndef qfalse
#define qfalse 0
#endif

#ifndef qtrue
#define qtrue 1
#endif

#ifndef STRING
#define STRING(offset) (const char *)(g_pGlobalVars->pStringBase + (int)offset)
#endif

#ifndef MAKE_STRING
#define MAKE_STRING(str) ((int)str - (int)STRING(0))
#endif

#ifndef FStrEq
inline qboolean FStrEq(const char* sz1, const char* sz2)
{
	return (strcmp(sz1, sz2) == 0);
}
#endif

#ifndef FStrCont
inline const char* FStrCont(const char* sz1, const char* sz2)
{
	return (strstr(sz1, sz2));
}
#endif

#ifndef FClassnameIs
#define FClassnameIs(pent, szClassname) FStrEq(STRING(pent->v.classname), szClassname)
#endif

#ifndef FClassnameContains
#define FClassnameContains(pent, szClassname) FStrCont(STRING(pent->v.classname), szClassname) != NULL
#endif

#define HITGROUP_GENERIC   0
#define HITGROUP_HEAD      1
#define HITGROUP_CHEST     2
#define HITGROUP_STOMACH   3
#define HITGROUP_LEFTARM   4
#define HITGROUP_RIGHTARM  5
#define HITGROUP_LEFTLEG   6
#define HITGROUP_RIGHTLEG  7
#define HITGROUP_SHIELD    8
//======================================================================

#endif

