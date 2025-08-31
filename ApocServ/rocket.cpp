#include "rocket.h"

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See README.md file for more details !

	File: rocket.cpp
	Purpose: Rocket Launcher implementation
*/ 

#include <vector>
#include <math.h>
#include <time.h>
#include "global.h"

//======================================================================
std::vector<rocket_s*>vRockets;
vec3_t forigin; //Rockets origin
int iSmoke; //Smoke sprite index.
float fCurTime; //Current timer count.
float fLastTime; //Last timer count.
//======================================================================

//======================================================================
void RCK_AddRocket(edict_s *pEnt, edict_s *pShooter, unsigned int aliveCount)
{
	//Add a rocket to memory.

	rocket_s *r = new rocket_s;
	r->aliveCount = aliveCount;
	r->pEntity = pEnt;
	r->pShooter = pShooter;

	vRockets.push_back(r);
}
//======================================================================

//======================================================================
void RCK_DelRocket(unsigned int id)
{
	//Remove a rocket from memory.


	if (id < vRockets.size()) {
		delete vRockets[id];
		vRockets.erase(vRockets.begin() + id);
	}
}
//======================================================================

//======================================================================
rocket_s *RCK_GetRocketByEdict(edict_s *pRocket)
{
	//Get a rocket by its edict address.

	for (unsigned int i = 0; i < vRockets.size(); i++) {
		if (vRockets[i]->pEntity == pRocket)
			return vRockets[i];
	}

	return NULL;
}
//======================================================================

//======================================================================
rocket_s *RCK_GetRocketByShooter(edict_s *pShooter)
{
	//Get a rocket by its shooter address.

	for (unsigned int i = 0; i < vRockets.size(); i++) {
		if (vRockets[i]->pShooter == pShooter)
			return vRockets[i];
	}

	return NULL;
}
//======================================================================

//======================================================================
rocket_s *RCK_GetRocketById(unsigned int id)
{
	//Get a rocket by its ID.

	if (id < vRockets.size())
		return vRockets[id];

	return NULL;
}
//======================================================================

//======================================================================
bool RCK_isInRadius(vec3_t start, vec3_t end)
{
	//Calculate the distance.

	vec3_t v;

	v[0] = start[0] - end[0];
	v[1] = start[1] - end[1];
	v[2] = start[2] - end[2];

	float dist = (sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]));

	return dist <= DAMAGE_RADIUS; //Is the distance between the rocket origin and the other origin less or equal then the damage radius?
}
//======================================================================

//======================================================================
void RCK_UTIL_Explosion(vec3_t v)
{
	//Creates an explosion.

	g_EngBackup.pfnMessageBegin(MSG_BROADCAST, 23, NULL, NULL);
	g_EngBackup.pfnWriteByte(TE_EXPLOSION);
	g_EngBackup.pfnWriteCoord(v[0]);
	g_EngBackup.pfnWriteCoord(v[1]);
	g_EngBackup.pfnWriteCoord(v[2]);
	g_EngBackup.pfnWriteShort(100);
	g_EngBackup.pfnWriteByte(50);
	g_EngBackup.pfnWriteByte(15);
	g_EngBackup.pfnWriteByte(TE_EXPLFLAG_NONE);
	g_EngBackup.pfnMessageEnd();
}
//======================================================================

//======================================================================
void RCK_UTIL_GlobalSound(vec3_t *v, const char *szSoundFile)
{
	//Emits a sound to each client.

	for (unsigned char i = 0; i <= g_pGlobalVars->maxClients; i++) {
		edict_s *pPlayer = g_EngBackup.pfnPEntityOfEntIndex(i);
		if (pPlayer) {
			g_EngBackup.pfnEmitAmbientSound(pPlayer, (float*)v, szSoundFile, 1.0f, 0.8f, 0, 101);
		}
	}
}
//======================================================================

//======================================================================
void RCK_Precache(void)
{
	//Precache models and initialize stuff.

	srand(time(NULL));

	g_EngBackup.pfnPrecacheSound((char*)"player/pl_pain2.wav");

	g_EngBackup.pfnPrecacheModel((char*)SZ_ROCKETMODEL);
	iSmoke = g_EngBackup.pfnPrecacheModel((char*)"sprites/smoke.spr");

	//Clear rockets if exist.
	for (unsigned int i = 0; i < vRockets.size(); i++)
		delete vRockets[i];

	vRockets.clear();
}
//======================================================================

//======================================================================
void RCK_Create(edict_s *pShooter)
{
	//Initializes the rocket.

	if (!pShooter) return;

	//Initialize the entity.
	edict_s *pRocketEnt = NULL;
	pRocketEnt = g_EngBackup.pfnCreateEntity();
	pRocketEnt->v.origin[0] = pShooter->v.origin[0];
	pRocketEnt->v.origin[1] = pShooter->v.origin[1];
	pRocketEnt->v.origin[2] = pShooter->v.origin[2];
	pRocketEnt->v.angles[0] = pShooter->v.v_angle[0];
	pRocketEnt->v.angles[1] = pShooter->v.v_angle[1];
	pRocketEnt->v.angles[2] = pShooter->v.v_angle[2];
	pRocketEnt->v.movetype = MOVETYPE_FLY; //The entity shall fly.

	//Set the entity model.
	g_EngBackup.pfnSetModel(pRocketEnt, SZ_ROCKETMODEL);

	//Create the following beam.
	g_EngBackup.pfnMessageBegin(MSG_BROADCAST, 23, NULL, NULL);
	g_EngBackup.pfnWriteByte(22);
	g_EngBackup.pfnWriteShort(g_EngBackup.pfnIndexOfEdict(pRocketEnt));
	g_EngBackup.pfnWriteShort(iSmoke);
	g_EngBackup.pfnWriteByte(40);
	g_EngBackup.pfnWriteByte(5);
	g_EngBackup.pfnWriteByte(211);
	g_EngBackup.pfnWriteByte(211);
	g_EngBackup.pfnWriteByte(211);
	g_EngBackup.pfnWriteByte(250);
	g_EngBackup.pfnMessageEnd();

	//Add rocket to memory.
	RCK_AddRocket(pRocketEnt, pShooter, 0);
}
//======================================================================

//======================================================================
void RCK_Detonate(unsigned int id)
{
	//The rocket should now be destroyed.

	rocket_s*pRocketEnt = RCK_GetRocketById(id);
	if (!pRocketEnt) return;

	//Create an explosion.
	RCK_UTIL_Explosion(pRocketEnt->pEntity->v.origin);

	//Remove the entity.
	g_EngBackup.pfnRemoveEntity(pRocketEnt->pEntity);
	RCK_DelRocket(id);
}
//======================================================================

//======================================================================
void RCK_CalcNextVec(vec3_t origins, vec3_t angles, vec3_t& out, float fVelocity)
{
	//Calculate next vector position
	
	vec3_t fmove;
	g_EngBackup.pfnAngleVectors(angles, fmove, NULL, NULL);

	fmove[2] = -fmove[2];
				
	out[0] = origins[0] + fmove[0] * fVelocity;
	out[1] = origins[1] + fmove[1] * fVelocity;
	out[2] = origins[2] + fmove[2] * fVelocity;
}
//======================================================================

//======================================================================
void RCK_Think(void)
{
	//Here we calculate the movement and the collision of each rocket.

	static bool bThinkOnce = false;
	if (!bThinkOnce) {
		//First call.

		fCurTime = g_pGlobalVars->time;
		fLastTime = g_pGlobalVars->time;

		bThinkOnce = true;
	}

	fCurTime = g_pGlobalVars->time; //Update current timer value.
	if (fCurTime > fLastTime + 0.100f) {
		for (unsigned int k = 0; k < vRockets.size(); k++) {
				//Calculate forward move depending of the view of the shooter.
				vec3_t fmove;
				g_EngBackup.pfnAngleVectors(vRockets[k]->pShooter->v.v_angle, fmove, NULL, NULL);

				vRockets[k]->pEntity->v.origin[0] += fmove[0] * ROCKET_VELOCITY;
				vRockets[k]->pEntity->v.origin[1] += fmove[1] * ROCKET_VELOCITY;
				vRockets[k]->pEntity->v.origin[2] += fmove[2] * ROCKET_VELOCITY;

	   			for (unsigned char i = 1; i <= g_pGlobalVars->maxClients; i++) { //Check each entity for radius.
					edict_s *pPlayer = g_EngBackup.pfnPEntityOfEntIndex(i);
					if (pPlayer) {
						if ((vRockets[k]->pShooter != pPlayer) && (!(pPlayer->v.flags & FL_SPECTATOR)) && (pPlayer->v.deadflag == DEAD_NO) && (RCK_isInRadius(pPlayer->v.origin, vRockets[k]->pEntity->v.origin))) {
							if (pPlayer->v.health > DAMAGE_VALUE) { //Check damage value.
								pPlayer->v.health -= DAMAGE_VALUE; //Reduce health.
							
								pPlayer->v.velocity[2] += JUMP_VALUE; //Jump high.

								//Jump direction.
								pPlayer->v.velocity[0] += -100 + rand()%200;
								pPlayer->v.velocity[1] += -100 + rand()%200;

							 	//Play pain sound.
								RCK_UTIL_GlobalSound(&pPlayer->v.origin, "player/pl_pain2.wav");
							} else {
								g_DllBackup.pfnClientKill(pPlayer);
							}

							RCK_Detonate(k);
							return;
						}
					}
				}

				//Check for walls
				vec3_t Target;
				TraceResult tr;

				RCK_CalcNextVec(vRockets[k]->pEntity->v.origin, vRockets[k]->pEntity->v.v_angle, Target, DAMAGE_RADIUS-2);
				g_EngBackup.pfnTraceLine(vRockets[k]->pEntity->v.origin, Target, 0, vRockets[k]->pEntity, &tr);

				if ((tr.flFraction != 1.0f) || (vRockets[k]->aliveCount > ALIVE_VALUE)) {//Did we reach the alive amount?
					RCK_Detonate(k); //If so, detonate the rocket

					continue;
				}

				vRockets[k]->aliveCount++; //Increment value.
			}

		
		fLastTime = g_pGlobalVars->time; //Update last timer value.
	}
}
//======================================================================
