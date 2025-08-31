#include "incoming.h"

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: incoming.cpp
	Purpose: Mortar implementation
*/

//======================================================================
unsigned int dwCurSnd; //Current sound timer amount.
unsigned int dwLastSnd; //Last sound timer count.
unsigned char bRowAmount = 0; //How often has a sound been played / explosion created ?
		
edict_s *pTargetEnt = NULL;
bool bStarted = false; //Is the timer started?
//======================================================================

//======================================================================
void INC_UTIL_Explosion(edict_s *pTarget)
{
	//Creates an explosion near the entity.

	if (!pTarget) return;

	vec3_t pos;
	memcpy(&pos, &pTarget->v.origin, sizeof(vec3_t));

	unsigned char iminus1 = rand()%2;
	unsigned char iminus2 = rand()%2;

	//Calculate position
	if (iminus1 == 0)
 		pos[0] -= rand()%MAX_XRAND;
	else if (iminus1 == 1) 
		pos[0] += rand()%MAX_XRAND;

	if (iminus2 == 0)
 		pos[1] -= rand()%MAX_ZRAND;
	else if (iminus2 == 1) 
		pos[1] += rand()%MAX_ZRAND;

	//Broadcast explosion information
	g_EngBackup.pfnMessageBegin(MSG_BROADCAST, 23, NULL, NULL);
	g_EngBackup.pfnWriteByte(TE_EXPLOSION);
	g_EngBackup.pfnWriteCoord(pos[0]);
	g_EngBackup.pfnWriteCoord(pos[1]);
	g_EngBackup.pfnWriteCoord(pos[2]);
	g_EngBackup.pfnWriteShort(100);
	g_EngBackup.pfnWriteByte(50);
	g_EngBackup.pfnWriteByte(15);
	g_EngBackup.pfnWriteByte(TE_EXPLFLAG_NONE);
	g_EngBackup.pfnMessageEnd();
}
//======================================================================

//======================================================================
void INC_UTIL_PrintString(edict_s *pEdict, const char* sz)
{
	//Prints a SayText string to a client

	g_EngBackup.pfnMessageBegin(MSG_ONE, 76, NULL, pEdict);
	g_EngBackup.pfnWriteByte(1);
	g_EngBackup.pfnWriteString(sz);
	g_EngBackup.pfnWriteString(NULL);
	g_EngBackup.pfnWriteString(NULL);
	g_EngBackup.pfnWriteString(NULL);
	g_EngBackup.pfnMessageEnd();
}
//======================================================================

//======================================================================
void INC_UTIL_EmitGlobalSound(vec3_t *pos, const char *szSoundFile)
{
	//Globally emits a sound to all clients (1-32 are the player entity indices).

	if (!szSoundFile) return;

	for (unsigned char i = 1; i <= g_pGlobalVars->maxClients; i++) {
		edict_s *pTarget = g_EngBackup.pfnPEntityOfEntIndex(i);
		if (pTarget) {
			g_EngBackup.pfnEmitAmbientSound(pTarget, (float*)pos, szSoundFile, 1.0f, 0.8f, 0, 101);
		}
	}
}
//======================================================================

//======================================================================
void INC_UTIL_Slap(edict_s *pTarget)
{
	//Slap the player entity.

	if (!pTarget) return;

	//Set velocity y value (for jump effect).
	pTarget->v.velocity[2] += MAX_JUMP_NUM;
	pTarget->v.velocity[0] += -100 + rand()%200;
	pTarget->v.velocity[1] += -100 + rand()%200;

	//Reduce clients health.
	if (pTarget->v.health > MAX_HEALTH_REDUCE)
		pTarget->v.health -= MAX_HEALTH_REDUCE;
}
//======================================================================

//======================================================================
void INC_SetTarget(edict_s *p)
{
	//Set the target entity address.

	pTargetEnt = p;
}
//======================================================================

//======================================================================
edict_s *INC_GetTarget(void)
{
	//Return the target entity address.

	return pTargetEnt;
}
//======================================================================

//======================================================================
bool INC_IsStarted(void)
{
	//Return the start status.

	return bStarted;
}
//======================================================================

//======================================================================
void INC_Think(void)
{
	//Should always be called in pfnStartFrame. Implementation of the mortar.
	if ((!bStarted) || (!pTargetEnt)) return;

	if (bRowAmount == 0) { //The timer has been started...
		//Initializing...

		//Do visual stuff.
		INC_UTIL_PrintString(pTargetEnt, SZ_INFO);
		INC_UTIL_EmitGlobalSound(&pTargetEnt->v.origin, SZ_SND_MORTAR);

		//Reset timers.
		dwCurSnd = g_pGlobalVars->time;
		dwLastSnd = g_pGlobalVars->time;

		//Increment row counter.
		bRowAmount = 1;
	}

	//The timer calculation.
	dwCurSnd = g_pGlobalVars->time;
	if (dwCurSnd > dwLastSnd + MS_MORTAR_COUNT) {
		//Is the maximal row amount reached?
		if (bRowAmount >= MAX_ROWS) {
			//Create the last explosion.
			INC_UTIL_Explosion(pTargetEnt);			
			//Kill the player.
			g_DllBackup.pfnClientKill(pTargetEnt);

			//Disable timer.
			bStarted = false;

			return; //Don't continue with the code below.
		}

		INC_UTIL_Explosion(pTargetEnt); //Explosion.
		INC_UTIL_EmitGlobalSound(&pTargetEnt->v.origin, SZ_SND_MORTAR); //Sound.
		INC_UTIL_Slap(pTargetEnt); //Slap.
		
		//Increment counter.
		bRowAmount++;

		dwLastSnd = g_pGlobalVars->time;
	}
}
//======================================================================

//======================================================================
void INC_Precache(void)
{
	//Precaches all needed sound(s).

	g_EngBackup.pfnPrecacheSound((char*)SZ_SND_MORTAR);
}
//======================================================================

//======================================================================
void INC_Start(void)
{
	//Starts the incoming mortar.
	
	if (!pTargetEnt) return;

	bRowAmount = 0;
	bStarted = true;
}
//======================================================================

//======================================================================
void INC_Abort(void)
{
	//Aborts the timer of the incoming mortar.

	bRowAmount = 0;
	pTargetEnt = NULL;
	bStarted = false;
}
//======================================================================
