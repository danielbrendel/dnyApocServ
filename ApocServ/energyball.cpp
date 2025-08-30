#include "energyball.h"
#include "global.h"

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: energyball.cpp
	Purpose: Energy Ball implementation
*/

//======================================================================
qboolean CEnergyBall::Precache(void)
{
	//Precache resources

	//Precache sounds
	g_EngBackup.pfnPrecacheSound((char*)EB_ACTIVATESOUND);
	g_EngBackup.pfnPrecacheSound((char*)EB_EXPLOSIONSOUND);

	//Precache Sprites
	g_EngBackup.pfnPrecacheModel((char*)EB_BALLSPRITE);

	iSprLighting = g_EngBackup.pfnPrecacheModel((char*)EB_ELECTROSPRITE);
	if (!iSprLighting)
		return qfalse;

	iSprCircle = g_EngBackup.pfnPrecacheModel((char*)EB_CIRCLESPRITE);
	if (!iSprCircle)
		return qfalse;

	return qtrue;
}
//======================================================================

//======================================================================
qboolean CEnergyBall::Spawn(edict_s* pShooter)
{
	//Spawn a new Energy Ball

	if (!pShooter)
		return qfalse;

	if (pBall)
		return qfalse;

	//Create Energy Ball entity
	pBall = g_EngBackup.pfnCreateEntity();
	if(!pBall)
		return qfalse;

	//Create Energy Balls' lighting sprite
	pElectro = g_EngBackup.pfnCreateEntity();
	if (!pElectro)
		return qfalse;

	//Set shooter
	this->pShooter = pShooter;

	//Set ball entity vars
	pBall->v.classname = MAKE_STRING(EB_CLASSNAME); //Set a class name to identify
	pBall->v.movetype = MOVETYPE_FLY; //Ball shall fly
	pBall->v.solid = SOLID_NOT; //Shall not be solid
	
	pBall->v.scale = EB_BALLSIZE; //Adjust the sprite size
	
	//Set balls' view angles
	pBall->v.angles[0] = pShooter->v.v_angle[0];
	pBall->v.angles[1] = pShooter->v.v_angle[1];
	pBall->v.angles[2] = pShooter->v.v_angle[2];

	pBall->v.effects |= EF_BRIGHTFIELD; //Add a spark effect around the ball
	pBall->v.renderamt = 255; //Alpha rendering value
	pBall->v.rendermode = kRenderTransAdd; //Add transparency to remove the black mask
	//Set a green ball
	pBall->v.rendercolor[0] = 0.0f;
	pBall->v.rendercolor[1] = 255.0f; 
	pBall->v.rendercolor[2] = 0.0f;

	g_EngBackup.pfnSetModel(pBall, EB_BALLSPRITE); //Merge the sprite with the entity
	g_EngBackup.pfnSetOrigin(pBall, pShooter->v.origin); //Set the balls world position

	//Set lighting entity vars
	pElectro->v.solid = SOLID_NOT; //Shall not be solid
	pElectro->v.scale = EB_ELECTROSIZE; //Adjust lighting size
	pElectro->v.flags |= FL_CUSTOMENTITY; //It's a custom entity
	pElectro->v.rendermode = kRenderNormal; //No special rendering mode
	pElectro->v.renderamt = 255;
	//Set a green light
	pElectro->v.rendercolor[0] = 0.0f;
	pElectro->v.rendercolor[1] = 100.0f;
	pElectro->v.rendercolor[2] = 0.0f;
	pElectro->v.frame = 0; //Start frame
	pElectro->v.modelindex = iSprLighting; //Set the sprite file handle
	
	g_EngBackup.pfnSetModel(pElectro, EB_ELECTROSPRITE); //Set the lightings' model file

	EmitGlobalAmbSound(&pBall->v.origin, EB_ACTIVATESOUND); //Play a spawn sound

#ifdef EB_CHECKALIVETIME
	//Initialize alive timers
	flTimeCur = g_pGlobalVars->time;
	flTimeLast = g_pGlobalVars->time;
#endif

	return qtrue;
}
//======================================================================

//======================================================================
qboolean CEnergyBall::CheckTarget(void)
{
	//Check for a target of the ball

	if ((!pBall) || (!pShooter))
		return qfalse;

	//Find an entity in the balls' target radius
	edict_s* pTarget = NULL;
	while ((pTarget = g_EngBackup.pfnFindEntityInSphere(pTarget, pBall->v.origin, EB_TARGETRADIUS)) != NULL) {
		if (FClassnameIs(pTarget, "worldspawn")) //Break out if worldspawn entity is found to prevent infinite loop (it's the last entity and seems always to be listed)
			break;
		
		if (ValidTargetEnt(pTarget)) {
			pTarget->v.health = 1.0f; //Set health to 1 percent
			pTarget->v.armorvalue = 0.0f; //Set armor to 0

			//Toss the target
			pTarget->v.velocity[2] += 500;
			pTarget->v.velocity[0] += -100 + rand() % 200;
			pTarget->v.velocity[1] += -100 + rand() % 200;

			BallExplode(); //Detonate the ball

			return qtrue;
		}
	}
	
	return qfalse;
}
//======================================================================

//======================================================================
void CEnergyBall::BallExplode(void)
{
	//Detonate the ball

	if ((!pBall) || (!pElectro))
		return;

	EmitEnergyCircle(); //Emit an energy cycle as an explosion

	EmitGlobalAmbSound(&pBall->v.origin, EB_EXPLOSIONSOUND); //Emit an explosion sound

	ClearStuff(); //Remove entities
}
//======================================================================

//======================================================================
qboolean CEnergyBall::CheckForwardWall(void)
{
	//Check for a wall in front of the ball

	if (!pBall)
		return qfalse;

	vec3_t endPos, nextPos;
	g_EngBackup.pfnAngleVectors(pBall->v.origin, nextPos, NULL, NULL); //Calculate next origin depending on the view
	
	//Copy world positon
	endPos[0] = pBall->v.origin[0];
	endPos[1] = pBall->v.origin[1];
	endPos[2] = pBall->v.origin[2];

	//Calculate a forward origin
	endPos[0] += nextPos[0] * 2.0f;
	endPos[1] += nextPos[1] * 2.0f;
	endPos[2] += nextPos[2] * 2.0f;

	//Check if collided to something
	TraceResult tr;
	g_EngBackup.pfnTraceLine(pBall->v.origin, endPos, qtrue, pBall, &tr);

	return tr.flFraction < 1.0f;
}
//======================================================================

//======================================================================
void CEnergyBall::Think(void)
{
	//Thinking stuff

	if ((!pBall) || (!pShooter) || (!pElectro))
		return;

	//Calculate next origin to move the ball forward
	vec3_t forw;
	g_EngBackup.pfnAngleVectors(pBall->v.angles, forw, NULL, NULL);

	pBall->v.origin[0] += forw[0] * EB_VELOCITY;
	pBall->v.origin[1] += forw[1] * EB_VELOCITY;
	pBall->v.origin[2] += forw[2] * EB_VELOCITY;

	//Modify frame for animation
	pBall->v.frame = ((int)pBall->v.frame + 1) % 10;

	edict_s* pRandomElt = GetRandomEntity(); //Get random entity near the ball for hitting with lighting
	if (pRandomElt) {
		g_pEngFuncs->pfnSetOrigin(pElectro, pBall->v.origin); //Set lighting start position

		//Copy end positon of found entity to angles
		pElectro->v.angles[0] = pRandomElt->v.origin[0];
		pElectro->v.angles[1] = pRandomElt->v.origin[1];
		pElectro->v.angles[2] = pRandomElt->v.origin[2] + EB_ORIGINOFFSET;

		//Modify frame for animation
		pElectro->v.frame = ((int)pElectro->v.frame + 1) % 10;
	}

	if ((CheckTarget()) || (CheckForwardWall())) { //Detonate ball if collided with monster entities or walls
		BallExplode();
	}

#ifdef EB_CHECKALIVETIME
	//Calculate for detonating if the ball has survived n seconds
	flTimeCur = g_pGlobalVars->time;
	if (flTimeCur > flTimeLast + EB_CHECKALIVETIME) {
		BallExplode();
	}
#endif
}
//======================================================================

//======================================================================
inline qboolean CEnergyBall::ValidTargetEnt(edict_s* pEntity)
{
	//Check for target entity is valid to be used
	//Check for:
	//	NULL-Pointer, Equal Entity, Lighting, Shooter, a Player or a monster
	return ((pEntity != NULL) && (pEntity != pBall) && (pEntity != pElectro) && (pEntity != pShooter) && (pEntity->v.deadflag == DEAD_NO) && ((FClassnameIs(pEntity, "player")) || (FClassnameContains(pEntity, "monster"))));
}
//======================================================================

//======================================================================
edict_s* CEnergyBall::GetRandomEntity(void)
{
	//Calculate a random entity to use as lighting end position

	static edict_s* pRndEntities[EB_MAXENTITIES]; //Can be modified easily
	edict_s* pTemp = NULL;
	unsigned char ucCounter = 0;
	unsigned char ucIndex;

	memset(pRndEntities, 0x00, sizeof(pRndEntities));

	while ((pTemp = g_EngBackup.pfnFindEntityInSphere(pTemp, pBall->v.origin, EB_ELECTRORADIUS)) != NULL) { //Enumerate trough entities in the lighting radius
		if (FClassnameIs(pTemp, "worldspawn")) //Break out if worldspawn entity is found to prevent infinite loop (it's the last entity and seems always to be listed)
			break;

		//Check if entity is valid to be used as end positon
		if ((pTemp != pBall) && (pTemp != pElectro) && ((FClassnameIs(pTemp, "player")) || (FClassnameContains(pTemp, "monster"))) || (pTemp->v.flags & FL_FAKECLIENT)) {
			pRndEntities[ucCounter] = pTemp; //Add entity pointer to array
			
			ucCounter++; //Increment to go to next array entry

			if (ucCounter >= sizeof(pRndEntities) / sizeof(edict_s*)) //Break out if array limit is reached
				break;
		}
	}
	
	if (!pRndEntities[0]) //If no valid entity has been found in the radius
		return NULL;

	unsigned int uiIndex = 0;
	for (unsigned int i = 0; i < sizeof(pRndEntities) / sizeof(edict_s*); i++) { //Increment a counter to get the amount of found entities
		if (pRndEntities[i])
			uiIndex++;
	}

	//Calculate a random value to select an entity randomly
	ucIndex = g_EngBackup.pfnRandomLong(1, sizeof(pRndEntities) / sizeof(edict_s*));
	ucIndex--;
	
	return pRndEntities[ucIndex];
}
//======================================================================

//======================================================================
void CEnergyBall::EmitEnergyCircle(void)
{
	//Force client to display an energy circle
	if ((!pBall) || (!iSprCircle))
		return;

	g_EngBackup.pfnMessageBegin(MSG_BROADCAST, 23, NULL,NULL);
	g_EngBackup.pfnWriteByte(TE_BEAMCYLINDER);
	g_EngBackup.pfnWriteCoord(pBall->v.origin[0]);
	g_EngBackup.pfnWriteCoord(pBall->v.origin[1]);
	g_EngBackup.pfnWriteCoord(pBall->v.origin[2] + 16);
	g_EngBackup.pfnWriteCoord(pBall->v.origin[0]);
	g_EngBackup.pfnWriteCoord(pBall->v.origin[1]);
	g_EngBackup.pfnWriteCoord(pBall->v.origin[2] + 500); 
	g_EngBackup.pfnWriteShort(iSprCircle);
	g_EngBackup.pfnWriteByte(0); // startframe
	g_EngBackup.pfnWriteByte(0); // framerate
	g_EngBackup.pfnWriteByte(2); // life
	g_EngBackup.pfnWriteByte(16);  // width
	g_EngBackup.pfnWriteByte(0);   // noise
	g_EngBackup.pfnWriteByte(0);//r
	g_EngBackup.pfnWriteByte(0);//g
	g_EngBackup.pfnWriteByte(250);//b
	g_EngBackup.pfnWriteByte(255); //brightness
	g_EngBackup.pfnWriteByte(0);	// speed
	g_EngBackup.pfnMessageEnd();
}
//======================================================================

//======================================================================
void CEnergyBall::EmitGlobalAmbSound(vec3_t *origin, const char *szSoundFile)
{
	//Emits a sound to each client.

	for (unsigned char i = 1; i <= g_pGlobalVars->maxClients; i++) {
		edict_s *pPlayer = g_EngBackup.pfnPEntityOfEntIndex(i);
		if (pPlayer) {
			g_EngBackup.pfnEmitAmbientSound(pPlayer, (float*)origin, szSoundFile, 1.0f, 0.8f, 0, 101);
		}
	}
}
//======================================================================

//======================================================================
void CEnergyBall::ClearStuff(void)
{
	//Clean up entity stuff

	if ((!pBall) || (!pElectro))
		return;

	//Remove the entity
	g_EngBackup.pfnRemoveEntity(pBall);
	g_EngBackup.pfnRemoveEntity(pElectro);

	//Clear pointers
	pBall = NULL;
	pElectro = NULL;
}
//======================================================================

//======================================================================
CEnergyBall gEnergyBall;
//======================================================================
