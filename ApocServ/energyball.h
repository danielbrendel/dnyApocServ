#ifndef _ENERGY_BALL
#define _ENERGY_BALL

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: energyball.h
	Purpose: Energy Ball Interface
*/

#include "sdkinclude.h"

//======================================================================
#define EB_VELOCITY 5.0f //The balls' velocity
#define EB_BALLSIZE 4.0f //Size of the ball sprite
#define EB_ELECTROSIZE 40.0f //Width size of the lighting
#define EB_TARGETRADIUS 40.0f //Target radius to search for targets
#define EB_ELECTRORADIUS 400.0f //Radius to check for entities to use for lighting end positon
#define EB_ORIGINOFFSET 20.0f //Add offset to not let the end pos be on ground
#define EB_MAXENTITIES 5 //Maximum allowed lightings
#define EB_CLASSNAME "weapon_energyball" //Use to identify the ball entity
#define EB_ACTIVATESOUND "weapons/mine_activate.wav"
#define EB_EXPLOSIONSOUND "weapons/electro4.wav"
#define EB_BALLSPRITE "sprites/xspark4.spr"
#define EB_ELECTROSPRITE "sprites/lgtning.spr"
#define EB_CIRCLESPRITE "sprites/shockwave.spr"
#define EB_CHECKALIVETIME 10.0f //In this case 10 seconds
//======================================================================

//======================================================================
class CEnergyBall {
private:
	edict_s* pBall; //Pointer to the energy ball entity
	edict_s* pShooter; //Pointer to the shooter
	edict_s* pElectro; //Pointer to the lighting entity

	int iSprLighting; //Handle to lighting sprite
	int iSprCircle; //Handle to energy circle

#ifdef EB_CHECKALIVETIME
	float flTimeCur; //Save current time value
	float flTimeLast; //Save start time value
#endif

	qboolean ValidTargetEnt(edict_s* pEntity);
	edict_s* GetRandomEntity(void);
	void EmitEnergyCircle(void);
	void EmitGlobalAmbSound(vec3_t *origin, const char *szSoundFile);
	qboolean CheckTarget(void);
	qboolean CheckForwardWall(void);
	void BallExplode(void);
	void ClearStuff(void);
public:
	CEnergyBall() { pBall = pShooter = pElectro = NULL; iSprLighting = iSprCircle = 0; }
	~CEnergyBall() { ClearStuff(); }

	qboolean Precache(void);
	qboolean Spawn(edict_s* pShooter);
	void Think(void);
};
//======================================================================

//======================================================================
extern CEnergyBall gEnergyBall;
//======================================================================

#endif