#ifndef _INCOMING_H
#define _INCOMING_H

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See README.md file for more details !

	File: incoming.h
	Purpose: Mortar interface
*/

#include "sdkinclude.h"
#include "global.h"

#ifdef __WIN32__
#include <windows.h>
#endif

//======================================================================
#define MAX_XRAND 100 //The difference of the x position (+, -)
#define MAX_ZRAND 100 //The difference of the z position (+, -)
#define MAX_ROWS 10 //How often shall the mortar be active?
#define MAX_HEALTH_REDUCE 7 //How much health shall be reduced
#define MAX_JUMP_NUM 500 //The value of the velocity z value which shall be set

#define MS_MORTAR_COUNT 2.000f //Please let it stay 2.000f seconds, because this is the sounds length!

#define SZ_SND_MORTAR "weapons/mortar.wav" //The sound.
#define SZ_INFO "** You are now target of the mortar **" //The information string.
//======================================================================

//======================================================================
void INC_SetTarget(edict_s *p);
edict_s *INC_GetTarget(void);
bool INC_IsStarted(void);

void INC_Think(void);

void INC_Precache(void);
void INC_Start(void);
void INC_Abort(void);
//======================================================================

#endif
