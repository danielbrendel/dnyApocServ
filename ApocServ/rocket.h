#ifndef _ROCKET_H
#define _ROCKET_H

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: rocket.h
	Purpose: Rocket Launcher interface
*/

#include "sdkinclude.h"

//======================================================================
#define SZ_ROCKETMODEL "models/rpgrocket.mdl" //The model file of the rocket.
#define ROCKET_VELOCITY 75 //The velocity of the rocket.
#define DAMAGE_RADIUS 70 //The radius which gets checked of entities.
#define DAMAGE_VALUE 87 //The damage value.
#define JUMP_VALUE 500 //The jump value for jump direction.
#define ALIVE_VALUE 250 //The alive time of one rocket.
//======================================================================

//======================================================================
struct rocket_s {
	edict_s *pEntity;
	edict_s *pShooter;
	unsigned int aliveCount;
};
//======================================================================

//======================================================================
void RCK_Precache(void);
void RCK_Create(edict_s *pShooter);
void RCK_Detonate(unsigned int id);
void RCK_Think(void);
//======================================================================

#endif