#include "enginefuncs.h"
#include "global.h"
#include "detour.h"
#include "hooks.h"
#include "player.h"
#include "incoming.h"
#include "rocket.h"
#include "energyball.h"
#include "log.h"
#include "clientcmd.h"
#include "tablehook.h"
#include "usermsgs.h"
#include "menu.h"

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: enginefuncs.cpp
	Purpose: Engine function callback implementations
*/

#include <time.h>

//======================================================================
T_GetEntityAPI orig_GetEntityAPI;
T_GetNewDLLFunctions orig_GetNewDLLFunctions;
T_GiveFnptrsToDll orig_GiveFnptrsToDll;
//======================================================================

//======================================================================
const char* paEngineFuncs[] = {
	"pfnPrecacheModel",
	"pfnPrecacheSound",
	"pfnSetModel",
	"pfnModelIndex",
	"pfnModelFrames",
	"pfnSetSize",
	"pfnChangeLevel",
	"pfnGetSpawnParms",
	"pfnSaveSpawnParms",
	"pfnVecToYaw",
	"pfnVecToAngles",
	"pfnMoveToOrigin",
	"pfnChangeYaw",
	"pfnChangePitch",
	"pfnFindEntityByString",
	"pfnGetEntityIllum",
	"pfnFindEntityInSphere",
	"pfnFindClientInPVS",
	"pfnEntitiesInPVS",
	"pfnMakeVectors",
	"pfnAngleVectors",
	"pfnCreateEntity",
	"pfnRemoveEntity",
	"pfnCreateNamedEntity",
	"pfnMakeStatic",
	"pfnEntIsOnFloor",
	"pfnDropToFloor",
	"pfnWalkMove",
	"pfnSetOrigin",
	"pfnEmitSound",
	"pfnEmitAmbientSound",
	"pfnTraceLine",
	"pfnTraceToss",
	"pfnTraceMonsterHull",
	"pfnTraceHull",
	"pfnTraceModel",
	"pfnTraceTexture",
	"pfnTraceSphere",
	"pfnGetAimVector",
	"pfnServerCommand",
	"pfnServerExecute",
	"pfnClientCommand",
	"pfnParticleEffect",
	"pfnLightStyle",
	"pfnDecalIndex",
	"pfnPointContents",
	"pfnMessageBegin",
	"pfnMessageEnd",
	"pfnWriteByte",
	"pfnWriteChar",
	"pfnWriteShort",
	"pfnWriteLong",
	"pfnWriteAngle",
	"pfnWriteCoord",
	"pfnWriteString",
	"pfnWriteEntity",
	"pfnCVarRegister",
	"pfnCVarGetFloat",
	"pfnCVarGetString",
	"pfnCVarSetFloat",
	"pfnCVarSetString",
	"pfnAlertMessage",
	"pfnEngineFprintf",
	"pfnPvAllocEntPrivateData",
	"pfnPvEntPrivateData",
	"pfnFreeEntPrivateData",
	"pfnSzFromIndex",
	"pfnAllocString",
	"pfnGetVarsOfEnt",
	"pfnPEntityOfEntOffset",
	"pfnEntOffsetOfPEntity",
	"pfnIndexOfEdict",
	"pfnPEntityOfEntIndex",
	"pfnFindEntityByVars",
	"pfnGetModelPtr",
	"pfnRegUserMsg",
	"pfnAnimationAutomove",
	"pfnGetBonePosition",
	"pfnFunctionFromName",
	"pfnNameForFunction",
	"pfnClientPrintf",
	"pfnServerPrint",
	"pfnCmd_Args",
	"pfnCmd_Argv",
	"pfnCmd_Argc",
	"pfnGetAttachment",
	"pfnCRC32_Init",
	"pfnCRC32_ProcessBuffer",
	"pfnCRC32_ProcessByte",
	"pfnCRC32_Final",
	"pfnRandomLong",
	"pfnRandomFloat",
	"pfnSetView",
	"pfnTime",
	"pfnCrosshairAngle",
	"pfnLoadFileForMe",
	"pfnFreeFile",
	"pfnEndSection",
	"pfnCompareFileTime",
	"pfnGetGameDir",
	"pfnCvar_RegisterVariable",
	"pfnFadeClientVolume",
	"pfnSetClientMaxspeed",
	"pfnCreateFakeClient",
	"pfnRunPlayerMove",
	"pfnNumberOfEntities",
	"pfnGetInfoKeyBuffer",
	"pfnInfoKeyValue",
	"pfnSetKeyValue",
	"pfnSetClientKeyValue",
	"pfnIsMapValid",
	"pfnStaticDecal",
	"pfnPrecacheGeneric",
	"pfnGetPlayerUserId",
	"pfnBuildSoundMsg",
	"pfnIsDedicatedServer",
	"pfnCVarGetPointer",
	"pfnGetPlayerWONId",
	"pfnInfo_RemoveKey",
	"pfnGetPhysicsKeyValue",
	"pfnSetPhysicsKeyValue",
	"pfnGetPhysicsInfoString",
	"pfnPrecacheEvent",
	"pfnPlaybackEvent",
	"pfnSetFatPVS",
	"pfnSetFatPAS",
	"pfnCheckVisibility ",
	"pfnDeltaSetField",
	"pfnDeltaUnsetField",
	"pfnDeltaAddEncoder",
	"pfnGetCurrentPlayer",
	"pfnCanSkipPlayer",
	"pfnDeltaFindField",
	"pfnDeltaSetFieldByIndex",
	"pfnDeltaUnsetFieldByIndex",
	"pfnSetGroupMask",
	"pfnCreateInstancedBaseline",
	"pfnCvar_DirectSet",
	"pfnForceUnmodified",
	"pfnGetPlayerStats",
	"pfnAddServerCommand",
	"pfnVoice_GetClientListening",
	"pfnVoice_SetClientListening",
	"pfnGetPlayerAuthId",
};

const char* paDllFuncs[] = {
	"pfnGameInit",
	"pfnSpawn",
	"pfnThink",
	"pfnUse",
	"pfnTouch",
	"pfnBlocked",
	"pfnKeyValue",
	"pfnSave",
	"pfnRestore",
	"pfnSetAbsBox",
	"pfnSaveWriteFields",
	"pfnSaveReadFields",
	"pfnSaveGlobalState",
	"pfnRestoreGlobalState",
	"pfnResetGlobalState",
	"pfnClientConnect",
	"pfnClientDisconnect",
	"pfnClientKill",
	"pfnClientPutInServer",
	"pfnClientCommand",
	"pfnClientUserInfoChanged",
	"pfnServerActivate",
	"pfnServerDeactivate",
	"pfnPlayerPreThink",
	"pfnPlayerPostThink",
	"pfnStartFrame",
	"pfnParmsNewLevel",
	"pfnParmsChangeLevel",
	"pfnGetGameDescription",
	"pfnPlayerCustomization",
	"pfnSpectatorConnect",
	"pfnSpectatorDisconnect",
	"pfnSpectatorThink",
	"pfnSys_Error",
	"pfnPM_Move",
	"pfnPM_Init",
	"pfnPM_FindTextureType",
	"pfnSetupVisibility",
	"pfnUpdateClientData",
	"pfnAddToFullPack",
	"pfnCreateBaseline",
	"pfnRegisterEncoders",
	"pfnGetWeaponData",
	"pfnCmdStart",
	"pfnCmdEnd",
	"pfnConnectionlessPacket ",
	"pfnGetHullBounds",
	"pfnCreateInstancedBaselines",
	"pfnInconsistentFile",
	"pfnAllowLagCompensation",
};

const char* paNewDllFuncs[] = {
	"pfnOnFreeEntPrivateData",
	"pfnGameShutdown",
	"pfnShouldCollide",
};
//======================================================================

//======================================================================
edict_s* FindEnemyPlayer(playerinfo_s* pBasePlayer)
{
	for (int i = 1; i <= g_pGlobalVars->maxClients; i++) {
		edict_s* pEdict = g_pEngFuncs->pfnPEntityOfEntIndex(i);
		if ((pEdict) && (pEdict != pBasePlayer->pEnt) && (pEdict->v.deadflag == DEAD_NO) && (!(pEdict->v.flags & FL_SPECTATOR)) && (pEdict->v.health > 0)) {
			playerinfo_s* pInfo = gPlayers.GetPlayerByEdict(pEdict);
			if ((pInfo) && (pInfo->Team != pBasePlayer->Team)) {
				return pEdict;
			}
		}
	}

	return NULL;
}
//======================================================================

//======================================================================
bool Cmd_Auth(edict_s* pEntity)
{
	//Called for command: "ssh_auth"

	const char *sz = g_pEngFuncs->pfnCmd_Argv(1);
	if (sz) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
		if ((pPlayer) && (strcmp(sz, g_AdminPass)==0)) {
			pPlayer->bAuthed = true;

			g_pEngFuncs->pfnEmitSound(pEntity, 3, "vox/hello.wav", 1.0f, 0.8f, 0, 101);
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "Welcome to ApocServ!\nType ssh_help for more info\n");

			return true;
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_Help(edict_s* pEntity)
{
	//Called for command: "ssh_help"

	const char *sz = g_pEngFuncs->pfnCmd_Argv(1);
	if (sz) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
		if ((pPlayer) && (pPlayer->bAuthed)) {
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ApocServ for GoldSrc CS/CZ\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "Available commands and cvars:\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_bunnyhop <0/1> //Enable or disable silent bunnyhop\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_invincible <0/1> //Enable or disable invincibility\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_noclip <0/1> //Enable or disable noclip\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_sethealth <value> //Set your health to the given value\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_healthcare <0/1> //Enable or disable health care\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_gravity <value> //Set your gravity value\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_setglow <me/ct/t/all/none> <R> <G> <B> //Adjust glow effect\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_transparency <0/1> //Set player transparency\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_getitem <item> //Get an item by class name\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_spawnzone <zone> //Spawn a zone entity (buy, bomb, hostage)\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_spawnhostage //Spawn a hostage near you\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_traceaim <0/1> //Enable or disable traceline hitbot\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_launchrocket //Launches a rocket\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_energyball //Launches an energy ball\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_startmortar <player> //Starts the mortar\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_savepos //Save current position\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_gotopos //Go to saved position\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_selplayer <id> //Set target player selection\n");
			g_EngBackup.pfnClientPrintf(pEntity, print_console, "ssh_actionmenu //Shows the action menu to you\n");

			return true;
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_Bunnyhop(edict_s* pEntity)
{
	//Called for command: "ssh_bunnyhop"

	const char *sz = g_pEngFuncs->pfnCmd_Argv(1);
	if (sz) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
		if ((pPlayer) && (pPlayer->bAuthed)) {
			pPlayer->cvars.bunnyhop = atoi(g_pEngFuncs->pfnCmd_Argv(1));

			if (pPlayer->cvars.bunnyhop) {
				g_EngBackup.pfnClientPrintf(pEntity, print_console, "Bunnyhop is now activated\n");
			} else {
				g_EngBackup.pfnClientPrintf(pEntity, print_console, "Bunnyhop is now deactivated\n");
			}

			return true;
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_NoClip(edict_s* pEntity)
{
	//Called for command: "ssh_noclip"

	const char* sz = g_pEngFuncs->pfnCmd_Argv(1);
	if (sz) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
		if ((pPlayer) && (pPlayer->bAuthed)) {
			pPlayer->cvars.noclip = atoi(g_pEngFuncs->pfnCmd_Argv(1));

			if (pPlayer->cvars.noclip) {
				pPlayer->pEnt->v.movetype = MOVETYPE_NOCLIP;
				pPlayer->pEnt->v.solid = SOLID_NOT;

				g_EngBackup.pfnClientPrintf(pEntity, print_console, "Noclip is now activated\n");
			} else {
				pPlayer->pEnt->v.movetype = MOVETYPE_WALK;
				pPlayer->pEnt->v.solid = SOLID_SLIDEBOX;

				g_EngBackup.pfnClientPrintf(pEntity, print_console, "Noclip is now deactivated\n");
			}

			return true;
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_Invincible(edict_s* pEntity)
{
	//Called for command: "ssh_invincible"

	const char* sz = g_pEngFuncs->pfnCmd_Argv(1);
	if (sz) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
		if ((pPlayer) && (pPlayer->bAuthed)) {
			pPlayer->cvars.invincibility = atoi(g_pEngFuncs->pfnCmd_Argv(1));
			if (pPlayer->cvars.invincibility) {
				pPlayer->pEnt->v.takedamage = DAMAGE_NO;

				g_EngBackup.pfnClientPrintf(pEntity, print_console, "Invincibility is now activated\n");
			} else {
				pPlayer->pEnt->v.takedamage = DAMAGE_YES;

				g_EngBackup.pfnClientPrintf(pEntity, print_console, "Invincibility is now deactivated\n");
			}

			return true;
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_Gravity(edict_s* pEntity)
{
	//Called for command: "ssh_gravity"

	const char* sz = g_pEngFuncs->pfnCmd_Argv(1);
	if (sz) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
		if ((pPlayer) && (pPlayer->bAuthed)) {
			pPlayer->cvars.gravity = atof(g_pEngFuncs->pfnCmd_Argv(1));
			pPlayer->pEnt->v.gravity = pPlayer->cvars.gravity;

			g_EngBackup.pfnClientPrintf(pEntity, print_console, std::string("Individual gravity is now set to " + std::to_string(pPlayer->cvars.gravity) + "\n").c_str());

			return true;
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_SetHealth(edict_s* pEntity)
{
	//Called for command: "ssh_sethealth"

	const char* sz = g_pEngFuncs->pfnCmd_Argv(1);
	if (sz) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
		if ((pPlayer) && (pPlayer->bAuthed)) {
			int health = atoi(g_pEngFuncs->pfnCmd_Argv(1));
			pPlayer->cvars.maxhealth = health;
			pPlayer->pEnt->v.max_health = health;
			pPlayer->pEnt->v.health = health;

			g_EngBackup.pfnClientPrintf(pEntity, print_console, std::string("Individual health is now set to " + std::to_string(pPlayer->cvars.maxhealth) + "\n").c_str());

			return true;
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_HealthCare(edict_s* pEntity)
{
	//Called for command: "ssh_healthcare"

	const char* sz = g_pEngFuncs->pfnCmd_Argv(1);
	if (sz) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
		if ((pPlayer) && (pPlayer->bAuthed)) {
			pPlayer->cvars.healthcare = atoi(g_pEngFuncs->pfnCmd_Argv(1));

			if (pPlayer->cvars.healthcare) {
				g_EngBackup.pfnClientPrintf(pEntity, print_console, "Healthcare is now activated\n");
			} else {
				g_EngBackup.pfnClientPrintf(pEntity, print_console, "Healthcare is now deactivated\n");
			}

			return true;
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_SetGlow(edict_s* pEntity)
{
	//Called for command: "ssh_setglow"

	playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
	if ((pPlayer) && (pPlayer->bAuthed)) {
		const char* state = g_pEngFuncs->pfnCmd_Argv(1);

		const char* sr = g_pEngFuncs->pfnCmd_Argv(2);
		const char* sg = g_pEngFuncs->pfnCmd_Argv(3);
		const char* sb = g_pEngFuncs->pfnCmd_Argv(4);

		float r = ((sr) ? (float)atof(sr) : 255);
		float g = ((sg) ? (float)atof(sg) : 255);
		float b = ((sb) ? (float)atof(sb) : 255);

		if (strcmp(state, "me") == 0) {
			SetPlayerGlow(pPlayer->pEnt, 2, r, g, b);

			g_EngBackup.pfnClientPrintf(pEntity, print_console, std::string("Glow effect set to: me = rgb(" + std::string(sr) + ", " + std::string(sg) + ", " + std::string(sb) + ")\n").c_str());
		} else if (strcmp(state, "all") == 0) {
			for (int i = 1; i <= g_pGlobalVars->maxClients; i++) {
				edict_s* pEdict = g_pEngFuncs->pfnPEntityOfEntIndex(i);
				if ((pEdict) && (pEdict->v.deadflag == DEAD_NO) && (!(pEdict->v.flags & FL_SPECTATOR)) && (pEdict->v.health > 0)) {
					SetPlayerGlow(pEdict, 2, r, g, b);
				}
			}

			g_EngBackup.pfnClientPrintf(pEntity, print_console, std::string("Glow effect set to: all = rgb(" + std::string(sr) + ", " + std::string(sg) + ", " + std::string(sb) + ")\n").c_str());
		} else if (strcmp(state, "ct") == 0) {
			for (int i = 1; i <= g_pGlobalVars->maxClients; i++) {
				playerinfo_s* pPlayer = gPlayers.GetPlayerById(i);
				if ((pPlayer) && (!(pPlayer->pEnt->v.flags & FL_SPECTATOR)) && (pPlayer->Team == PLAYER_TEAM_CT)) {
					SetPlayerGlow(pPlayer->pEnt, 2, r, g, b);
				}
			}

			g_EngBackup.pfnClientPrintf(pEntity, print_console, std::string("Glow effect set to: ct = rgb(" + std::string(sr) + ", " + std::string(sg) + ", " + std::string(sb) + ")\n").c_str());
		} else if (strcmp(state, "t") == 0) {
			for (int i = 1; i <= g_pGlobalVars->maxClients; i++) {
				playerinfo_s* pPlayer = gPlayers.GetPlayerById(i);
				if ((pPlayer) && (!(pPlayer->pEnt->v.flags & FL_SPECTATOR)) && (pPlayer->Team == PLAYER_TEAM_T)) {
					SetPlayerGlow(pPlayer->pEnt, 2, r, g, b);
				}
			}

			g_EngBackup.pfnClientPrintf(pEntity, print_console, std::string("Glow effect set to: t = rgb(" + std::string(sr) + ", " + std::string(sg) + ", " + std::string(sb) + ")\n").c_str());
		} else if (strcmp(state, "none") == 0) {
			for (int i = 1; i <= g_pGlobalVars->maxClients; i++) {
				edict_s* pEdict = g_pEngFuncs->pfnPEntityOfEntIndex(i);
				if ((pEdict) && (pEdict->v.deadflag == DEAD_NO) && (!(pEdict->v.flags & FL_SPECTATOR)) && (pEdict->v.health > 0)) {
					pEdict->v.renderfx = kRenderFxNone;
				}
			}

			g_EngBackup.pfnClientPrintf(pEntity, print_console, "Glow effect is now deactivated\n");
		} else {
			g_EngBackup.pfnClientPrintf(pEntity, print_console, std::string("Invalid state: \"" + std::string(state) + "\"\n").c_str());
		}

		return true;
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_Transparency(edict_s* pEntity)
{
	//Called for command: "ssh_transparency"

	playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
	if ((pPlayer) && (pPlayer->bAuthed)) {
		int state = atoi(g_pEngFuncs->pfnCmd_Argv(1));

		if (state) {
			pPlayer->pEnt->v.rendermode = kRenderTransAdd;

			g_EngBackup.pfnClientPrintf(pEntity, print_console, "Transparency is now activated\n");
		} else {
			pPlayer->pEnt->v.rendermode = kRenderNormal;

			g_EngBackup.pfnClientPrintf(pEntity, print_console, "Transparency is now deactivated\n");
		}

		return true;
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_GetItem(edict_s* pEntity)
{
	//Called for command: "ssh_getitem"

	const char* sz = g_pEngFuncs->pfnCmd_Argv(1);
	if (sz) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
		if ((pPlayer) && (pPlayer->bAuthed)) {
			const char* item = g_pEngFuncs->pfnCmd_Argv(1);
			SpawnItemToPlayer(pPlayer->pEnt, item);

			g_EngBackup.pfnClientPrintf(pEntity, print_console, std::string("Spawned and attached \"" + std::string(item) + "\" to you\n").c_str());

			return true;
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_SpawnZone(edict_s* pEntity)
{
	//Called for command: "ssh_spawnzone"

	playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
	if ((pPlayer) && (pPlayer->bAuthed)) {
		const char* zone = g_pEngFuncs->pfnCmd_Argv(1);
		if (strcmp(zone, "buy") == 0) {
			SpawnZone("func_buyzone", pPlayer->pEnt->v.origin);

			return true;
		} else if (strcmp(zone, "bomb") == 0) {
			SpawnZone("func_bomb_target", pPlayer->pEnt->v.origin);

			return true;
		} else if (strcmp(zone, "hostage") == 0) {
			SpawnZone("func_hostage_rescue", pPlayer->pEnt->v.origin);

			return true;
		} else {
			g_EngBackup.pfnClientPrintf(pEntity, print_console, std::string("Invalid zone specified: \"" + std::string(zone) + "\"\n").c_str());
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_SpawnHostage(edict_s* pEntity)
{
	//Called for command: "ssh_spawnhostage"

	playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
	if ((pPlayer) && (pPlayer->bAuthed)) {
		float fPos[3];
		fPos[0] = pPlayer->pEnt->v.origin[0];
		fPos[1] = pPlayer->pEnt->v.origin[1];
		fPos[2] = pPlayer->pEnt->v.origin[2];

		fPos[2] += 100.0f;

		if (SpawnEntity("hostage_entity", fPos)) {
			return true;
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_TraceAim(edict_s* pEntity)
{
	//Called for command: "ssh_traceaim"

	const char* sz = g_pEngFuncs->pfnCmd_Argv(1);
	if (sz) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
		if ((pPlayer) && (pPlayer->bAuthed)) {
			pPlayer->cvars.traceaim = atoi(g_pEngFuncs->pfnCmd_Argv(1));

			if (pPlayer->cvars.traceaim) {
				g_EngBackup.pfnClientPrintf(pEntity, print_console, "Traceaim is now activated\n");
			} else {
				g_EngBackup.pfnClientPrintf(pEntity, print_console, "Traceaim is now deactivated\n");
			}

			return true;
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_RocketLauncher(edict_s* pEntity)
{
	//Called for command: "ssh_launchrocket"

	playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
	if ((pPlayer) && (pPlayer->bAuthed)) {
		RCK_Create(pEntity);

		return true;
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_EnergyBall(edict_s* pEntity)
{
	//Called for command: "ssh_energyball"

	playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
	if ((pPlayer) && (pPlayer->bAuthed)) {
		gEnergyBall.Spawn(pEntity);

		return true;
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_Mortar(edict_s* pEntity)
{
	//Called for command: "ssh_startmortar"

	const char *sz = g_pEngFuncs->pfnCmd_Argv(1);
	if (sz) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
		if ((pPlayer) && (pPlayer->bAuthed)) {
			if (!INC_IsStarted()) {
				playerinfo_s* pRemote = gPlayers.GetPlayerById(atoi(sz));
				if ((isValidPlayer(pRemote)) && (pRemote->pEnt->v.deadflag == DEAD_NO)) {
					INC_SetTarget(pRemote->pEnt);
					INC_Start();

					return true;
				}
			}
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_SavePosition(edict_s* pEntity)
{
	//Called for command: "ssh_savepos"

	playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
	if ((pPlayer) && (pPlayer->bAuthed)) {
		pPlayer->cvars.savedpos[0] = pPlayer->pEnt->v.origin[0];
		pPlayer->cvars.savedpos[1] = pPlayer->pEnt->v.origin[1];
		pPlayer->cvars.savedpos[2] = pPlayer->pEnt->v.origin[2];

		g_EngBackup.pfnClientPrintf(pEntity, print_console, "Saved your current position\n");

		return true;
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_GoToPosition(edict_s* pEntity)
{
	//Called for command: "ssh_gotopos"

	playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
	if ((pPlayer) && (pPlayer->bAuthed)) {
		g_pEngFuncs->pfnSetOrigin(pPlayer->pEnt, pPlayer->cvars.savedpos);

		return true;
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_SelPlayer(edict_s* pEntity)
{
	//Called for command: "ssh_selplayer"

	const char* sz = g_pEngFuncs->pfnCmd_Argv(1);
	if (sz) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
		if ((pPlayer) && (pPlayer->bAuthed)) {
			pPlayer->cvars.selplayer = atoi(sz);

			g_EngBackup.pfnClientPrintf(pEntity, print_console, std::string("Selected target player #" + std::to_string(pPlayer->cvars.selplayer) + "\n").c_str());

			return true;
		}
	}

	return false;
}
//======================================================================

//======================================================================
bool Cmd_ActionMenu(edict_s* pEntity)
{
	//Called for command: "ssh_actionmenu"

	const char *sz = g_pEngFuncs->pfnCmd_Argv(1);
	if (sz) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
		if ((pPlayer) && (pPlayer->bAuthed)) {
			if (gActionMenu.IsReady()) {
				gActionMenu.Show(pEntity);

				return true;
			}
		}
	}

	return false;
}
//======================================================================

//======================================================================
void Menu_SelectAction(edict_s* pEntity, int iSelection)
{
	//Handle menu selection of player that has selected a menu entry

	playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pEntity);
	if ((pPlayer) && (pPlayer->bAuthed) && (pPlayer->cvars.selplayer > 0)) {
		edict_s* pTarget = g_pEngFuncs->pfnPEntityOfEntIndex(pPlayer->cvars.selplayer);
		if ((pTarget) && (pTarget->v.deadflag == DEAD_NO) && (!(pTarget->v.flags & FL_SPECTATOR))) {
			switch (iSelection) {
			case 1:
				SlapPlayer(pTarget, 1);
				break;
			case 2:
				BlindPlayer(pTarget);
				break;
			case 3:
				PlayStepNearPlayer(pTarget);
				break;
			default:
				break;
			}
		} else {
			PrintSayText(pPlayer->pEnt, "You need to select a player", NULL, NULL, NULL);
		}
	}
}
//======================================================================

//======================================================================
void new_pfnServerActivate(edict_s *pEdictList, int edictCount, int clientMax)
{
	//Called when activating the server or changing map

	g_DllBackup.pfnServerActivate(pEdictList, edictCount, clientMax);

	//Check target game mod
	char szGameDir[MAX_PATH] = { 0 };
	g_pEngFuncs->pfnGetGameDir(szGameDir);
	if (strcmp(szGameDir, HLMOD) != 0) {
		printf("Warning: Built for \"%s\", but \"%s\" found\n", HLMOD, szGameDir);
	}

	//Initialize player class
	gPlayers.Initialize(pEdictList);

	//Load all SteamID profile configs
	gPlayers.LoadAllSteamIDConfigs(SSH_CONFIG);
	
	//Add client command handlers
	gClientCmd.AddHandler(&Cmd_Auth, "ssh_auth");
	gClientCmd.AddHandler(&Cmd_Help, "ssh_help");
	gClientCmd.AddHandler(&Cmd_Bunnyhop, "ssh_bunnyhop");
	gClientCmd.AddHandler(&Cmd_NoClip, "ssh_noclip");
	gClientCmd.AddHandler(&Cmd_Invincible, "ssh_invincible");
	gClientCmd.AddHandler(&Cmd_Gravity, "ssh_gravity");
	gClientCmd.AddHandler(&Cmd_SetHealth, "ssh_sethealth");
	gClientCmd.AddHandler(&Cmd_HealthCare, "ssh_healthcare");
	gClientCmd.AddHandler(&Cmd_SetGlow, "ssh_setglow");
	gClientCmd.AddHandler(&Cmd_Transparency, "ssh_transparency");
	gClientCmd.AddHandler(&Cmd_GetItem, "ssh_getitem");
	gClientCmd.AddHandler(&Cmd_SpawnZone, "ssh_spawnzone");
	gClientCmd.AddHandler(&Cmd_SpawnHostage, "ssh_spawnhostage");
	gClientCmd.AddHandler(&Cmd_TraceAim, "ssh_traceaim");
	gClientCmd.AddHandler(&Cmd_RocketLauncher, "ssh_launchrocket");
	gClientCmd.AddHandler(&Cmd_EnergyBall, "ssh_energyball");
	gClientCmd.AddHandler(&Cmd_Mortar, "ssh_startmortar");
	gClientCmd.AddHandler(&Cmd_SavePosition, "ssh_savepos");
	gClientCmd.AddHandler(&Cmd_GoToPosition, "ssh_gotopos");
	gClientCmd.AddHandler(&Cmd_SelPlayer, "ssh_selplayer");
	gClientCmd.AddHandler(&Cmd_ActionMenu, "ssh_actionmenu");

	//Clear menu if already initialized
	if (gActionMenu.IsReady())
		gActionMenu.Clear();

	//Initialize action menu
	if (gActionMenu.Initialize("Action Menu\n", 0, &Menu_SelectAction)) {
		gActionMenu.AddSelection("#1 Slap\n");
		gActionMenu.AddSelection("#2 Blind\n");
		gActionMenu.AddSelection("#3 Step\n");
		gActionMenu.AddSelection("\n#0 Close\n");
	}

	g_pEngFuncs->pfnPrecacheSound((char*)"vox/hello.wav");
	
	INC_Precache();
	RCK_Precache();

	gEnergyBall.Precache();

	#if defined(CSVER_CSTRIKE)
	g_pEngFuncs->pfnPrecacheModel((char*)"models/scientist.mdl");
	g_pEngFuncs->pfnPrecacheSound((char*)"hostage/hos1.wav");
	g_pEngFuncs->pfnPrecacheSound((char*)"hostage/hos2.wav");
	g_pEngFuncs->pfnPrecacheSound((char*)"hostage/hos3.wav");
	g_pEngFuncs->pfnPrecacheSound((char*)"hostage/hos4.wav");
	g_pEngFuncs->pfnPrecacheSound((char*)"hostage/hos5.wav");
	#elif defined(CSVER_CZERO)
	g_pEngFuncs->pfnPrecacheModel((char*)"models/hostageA.mdl");
	g_pEngFuncs->pfnPrecacheModel((char*)"models/hostageB.mdl");
	g_pEngFuncs->pfnPrecacheModel((char*)"models/hostageC.mdl");
	g_pEngFuncs->pfnPrecacheModel((char*)"models/hostageD.mdl");
	g_pEngFuncs->pfnPrecacheSound((char*)"hostage/hos1.wav");
	g_pEngFuncs->pfnPrecacheSound((char*)"hostage/hos2.wav");
	g_pEngFuncs->pfnPrecacheSound((char*)"hostage/hos3.wav");
	g_pEngFuncs->pfnPrecacheSound((char*)"hostage/hos4.wav");
	g_pEngFuncs->pfnPrecacheSound((char*)"hostage/hos5.wav");
	#endif

	g_bServerStarted = true;
}
//======================================================================

//======================================================================
void new_pfnServerDeactivate(void)
{
	//Called on server deactivation

	g_bServerStarted = false;

	g_DllBackup.pfnServerDeactivate();
}
//======================================================================

//======================================================================
qboolean new_pfnClientConnect(edict_s *pEntity, const char*pszName, const char*pszAddress, char szRejectReason[128])
{
	//Called when a new player is connected to the server

	gPlayers.Connect(pEntity, pszName, pszAddress, szRejectReason);

	//Load config file for player if exists
	if (pEntity) {
		const char* szSteamID = g_pEngFuncs->pfnGetPlayerAuthId(pEntity);
		if (szSteamID) {
			if (gPlayers.ConfigReadyForPlayer(szSteamID)) /* Or call GetConfigNameForSteamID directly */ {
				char* szConfig = gPlayers.GetConfigNameForSteamID(szSteamID);
				if (szConfig)
					gPlayers.ExecForSteamID(szSteamID, szConfig);
			}
		}
	}

	return g_DllBackup.pfnClientConnect(pEntity, pszName, pszAddress, szRejectReason);
}
//======================================================================

//======================================================================
void new_pfnClientPutInServer(edict_s* pEntity)
{
	//Called when a client is put in server, for both network clients and bots

	gPlayers.PutInServer(pEntity);

	g_DllBackup.pfnClientPutInServer(pEntity);
}
//======================================================================

//======================================================================
void new_pfnClientDisconnect(edict_s*pEntity)
{
	//Called when a player is disconnected from server
	
	gPlayers.Disconnect(pEntity);

	g_DllBackup.pfnClientDisconnect(pEntity);
}
//======================================================================

//======================================================================
void new_pfnClientCommand(edict_s*pEntity)
{
	//Called when a client has send a command for server (console, bind, ...)

	//Handle command
	for (unsigned int i = 0; i < gClientCmd.GetAmount(); i++) {
		if (gClientCmd.CmdEqual(i, g_pEngFuncs->pfnCmd_Argv(0))) { //If cmd found
			//Call handler and handle result
			if (gClientCmd.CallHandler(i, pEntity))
				return;
		}
	}

	//Process menu command

	if ((gActionMenu.IsReady()) && (gActionMenu.IsShown()) && (strcmp(g_pEngFuncs->pfnCmd_Argv(0), "menuselect") == 0))
		gActionMenu.ProcessInput(pEntity, g_pEngFuncs->pfnCmd_Argv(1));

	//Call original function
	g_DllBackup.pfnClientCommand(pEntity);
}
//======================================================================

//======================================================================
void new_pfnPlayerPreThink(edict_s* pEntity)
{
	//Called before engine manages client stuff
	
	playerinfo_s* pInfo = gPlayers.GetPlayerByEdict(pEntity);
	if (pInfo) {
		if (pInfo->cvars.bunnyhop == 1) {
			//Bunnyhop is active when player is alive, has jump button pressed and is touched to ground
			if ((pEntity->v.deadflag == DEAD_NO) && (pEntity->v.button & IN_JUMP) && (pEntity->v.flags & FL_ONGROUND))
				pEntity->v.velocity[2] += 250.0f;
		}

		if (pInfo->cvars.healthcare == 1) {
			if (pInfo->pEnt->v.max_health < pInfo->cvars.healthcare) {
				pInfo->pEnt->v.max_health = pInfo->cvars.healthcare;
			}

			if (pInfo->pEnt->v.health < pInfo->pEnt->v.max_health) {
				pInfo->pEnt->v.health = pInfo->pEnt->v.max_health;
			}
		}
	}

	g_DllBackup.pfnPlayerPreThink(pEntity);
}
//======================================================================

//======================================================================
void new_pfnStartFrame(void)
{
	//Called every frame

	INC_Think();
	RCK_Think();

	gEnergyBall.Think();

	g_DllBackup.pfnStartFrame();
}
//======================================================================

//======================================================================
void new_pfnClientUserInfoChanged(edict_s *pEntity, char *infoBuffer)
{
	//Called when client changes user info (name, ...)

	gPlayers.RefreshName(pEntity, infoBuffer);
	gPlayers.RefreshTeam(pEntity, infoBuffer);

	g_DllBackup.pfnClientUserInfoChanged(pEntity, infoBuffer);
}
//======================================================================

//======================================================================
int new_pfnRegUserMsg(const char *pszName, int iSize)
{
	//Called when registering a new user message

	int retValue = g_EngBackup.pfnRegUserMsg(pszName, iSize);

	//Add message to own manager
	gUserMsgs.AddNewMsg(pszName, retValue);

	return retValue;
}
//======================================================================

//======================================================================
void new_pfnMessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_s*ed)
{
	//Called when starting new user message

	g_EngBackup.pfnMessageBegin(msg_dest, msg_type, pOrigin, ed);
}
//======================================================================

//======================================================================
void new_pfnWriteByte(int iValue)
{
	//Called when writing a byte value to the user message buffer

	g_EngBackup.pfnWriteByte(iValue);
}
//======================================================================

//======================================================================
void new_pfnWriteChar(int iValue)
{
	//Called when writing a char value to the user message buffer

	g_EngBackup.pfnWriteChar(iValue);
}
//======================================================================

//======================================================================
void new_pfnWriteShort(int iValue)
{
	//Called when writing a short int value to the user message buffer

	g_EngBackup.pfnWriteShort(iValue);
}
//======================================================================

//======================================================================
void new_pfnWriteLong(int iValue)
{
	//Called when writing a long int value to the user message buffer

	g_EngBackup.pfnWriteLong(iValue);
}
//======================================================================

//======================================================================
void new_pfnWriteAngle(float flValue)
{
	//Called when writing an angle dimension value to the user message buffer

	g_EngBackup.pfnWriteAngle(flValue);
}
//======================================================================

//======================================================================
void new_pfnWriteCoord(float flValue)
{
	//Called when writing a coordinate dimension value to the user message buffer

	g_EngBackup.pfnWriteCoord(flValue);
}
//======================================================================

//======================================================================
void new_pfnWriteString(const char *sz)
{
	//Called when writing a string buffer to the user message buffer

	g_EngBackup.pfnWriteString(sz);
}
//======================================================================

//======================================================================
void new_pfnMessageEnd(void)
{
	//Called when finishing a user message

	g_EngBackup.pfnMessageEnd();
}
//======================================================================

//======================================================================
void new_pfnTraceLine(const float*p1, const float*p2, int fNoMonsters, edict_s*pentToSkip, TraceResult*pptr)
{
	//Called when calculating traces between a start and an end point. Many uses.

	static bool bTraceNext = false;

	if ((g_bServerStarted) && (!bTraceNext)) {
		playerinfo_s* pPlayer = gPlayers.GetPlayerByEdict(pentToSkip);

		if ((isValidPlayer(pPlayer)) && (pPlayer->cvars.traceaim)) {
			edict_s* pTarget = FindEnemyPlayer(pPlayer);

			if (pTarget) {
				pptr->pHit = pTarget;
				pptr->flFraction = 0.1f;
				pptr->vecEndPos[0] = pTarget->v.origin[0];
				pptr->vecEndPos[1] = pTarget->v.origin[1];
				pptr->vecEndPos[2] = pTarget->v.origin[2];
				pptr->iHitgroup = HITGROUP_HEAD;

				bTraceNext = true;

				return;
			}
		}
	}

	if (bTraceNext) {
		bTraceNext = false;
	}

	g_EngBackup.pfnTraceLine(p1, p2, fNoMonsters, pentToSkip, pptr);
}
//======================================================================

//======================================================================
void Set_Password(void)
{
	//Called when setting a new auth password

	const char* pw = g_pEngFuncs->pfnCmd_Argv(1);
	if (pw) {
		strcpy(g_AdminPass, pw); //Copy to authentication buffer

		//Print info into server console
		char buf[1024];
		sprintf(buf, "Auth password has been set to \"%s\"\n", g_AdminPass);
		g_pEngFuncs->pfnServerPrint(buf);

		//Remove successful auth state of each player
		for (int i = 1; i <= g_pGlobalVars->maxClients; i++) {
			playerinfo_s* pInfo = gPlayers.GetPlayerById(i);
			if (pInfo) {
				pInfo->bAuthed = false;
			}
		}
	}
}
//======================================================================

//======================================================================
EXPORT_LIBRARY int ENTAPI GetEntityAPI(DLL_FUNCTIONS *pFunctionTable, int interfaceVersion)
{
	//Called to recieve DLL_FUNCTIONS pointer and Interface version from engine

	//Get address and call it from original module
	#ifdef __linux__
	orig_GetEntityAPI = (T_GetEntityAPI)dlsym(hOrigModule, "GetEntityAPI");
	#else
	orig_GetEntityAPI = (T_GetEntityAPI)GetProcAddress((HMODULE)g_hOrigModule, "GetEntityAPI");
	#endif
	orig_GetEntityAPI(pFunctionTable, interfaceVersion);

	g_pDllFuncs = pFunctionTable;
	memcpy((void*)&g_DllBackup, (const void*)g_pDllFuncs, sizeof(DLL_FUNCTIONS));

	//Initialize hooking
	gDllTable.Initialize(pFunctionTable, sizeof(DLL_FUNCTIONS));

	//Register aliases
	for (unsigned int i = 0; i < sizeof(paDllFuncs) / sizeof(unsigned int); i++) 
		gDllTable.AddNewName(paDllFuncs[i], i);

	//Hook functions
	gDllTable.Hook("pfnServerActivate", &new_pfnServerActivate);
	gDllTable.Hook("pfnServerDeactivate", &new_pfnServerDeactivate);
	gDllTable.Hook("pfnClientConnect", &new_pfnClientConnect);
	gDllTable.Hook("pfnClientPutInServer", &new_pfnClientPutInServer);
	gDllTable.Hook("pfnClientDisconnect", &new_pfnClientDisconnect);
	gDllTable.Hook("pfnClientCommand", &new_pfnClientCommand);
	gDllTable.Hook("pfnStartFrame", &new_pfnStartFrame);
	gDllTable.Hook("pfnPlayerPreThink", &new_pfnPlayerPreThink);
	gDllTable.Hook("pfnClientUserInfoChanged", &new_pfnClientUserInfoChanged);

	return 1;
}
//======================================================================

//======================================================================
EXPORT_LIBRARY int NEWAPI GetNewDLLFunctions(NEW_DLL_FUNCTIONS *pNewFunctionTable, int*pinterfaceVersion)
{
	//Called to recieve NEW_DLL_FUNCTIONS and Interface version pointers from engine

	if (!pNewFunctionTable) return 0;

	//Get address and call it from original module
	#ifdef __linux__
	orig_GetNewDLLFunctions = (T_GetNewDLLFunctions)dlsym(hOrigModule, "GetNewDLLFunctions");
	#else
	orig_GetNewDLLFunctions = (T_GetNewDLLFunctions)GetProcAddress((HMODULE)g_hOrigModule, "GetNewDLLFunctions");
	#endif

	//Initialize hooking
	gNewDllTable.Initialize(pNewFunctionTable, sizeof(NEW_DLL_FUNCTIONS));

	//Register aliases
	for (unsigned int i = 0; i < sizeof(paNewDllFuncs) / sizeof(unsigned int); i++)
		gNewDllTable.AddNewName(paNewDllFuncs[i], i);

	return orig_GetNewDLLFunctions(pNewFunctionTable, pinterfaceVersion);
}
//======================================================================

//======================================================================
EXPORT_LIBRARY void FNPAPI GiveFnptrsToDll(enginefuncs_s *pengfuncsFromEngine, globalvars_t* pGlobals)
{
	//Called to recieve enginefuncs_s and globalvars_s pointers from engine

	//Initialize random timer
	srand(time(NULL));

	g_pEngFuncs = pengfuncsFromEngine;
	g_pGlobalVars = pGlobals;

	memcpy((void*)&g_EngBackup, (void*)g_pEngFuncs, sizeof(enginefuncs_s));

	//Initialize hooking
	gEngTable.Initialize(pengfuncsFromEngine, sizeof(enginefuncs_s));

	//Register aliases

	for (unsigned int i = 0; i < sizeof(paEngineFuncs) / sizeof(unsigned int); i++)
		gEngTable.AddNewName(paEngineFuncs[i], i);

	//Hook functions
	gEngTable.Hook("pfnRegUserMsg", &new_pfnRegUserMsg);
	gEngTable.Hook("pfnMessageBegin", &new_pfnMessageBegin);
	gEngTable.Hook("pfnWriteByte", &new_pfnWriteByte);
	gEngTable.Hook("pfnWriteChar", &new_pfnWriteChar);
	gEngTable.Hook("pfnWriteShort", &new_pfnWriteShort);
	gEngTable.Hook("pfnWriteLong", &new_pfnWriteLong);
	gEngTable.Hook("pfnWriteAngle", &new_pfnWriteAngle);
	gEngTable.Hook("pfnWriteCoord", &new_pfnWriteCoord);
	gEngTable.Hook("pfnWriteString", &new_pfnWriteString);
	gEngTable.Hook("pfnMessageEnd", &new_pfnMessageEnd);
	gEngTable.Hook("pfnTraceLine", &new_pfnTraceLine);

	//Load original module into memory (on windows also check for Z-Bots module)
	#ifdef __linux__
	hOrigModule = dlopen(LMODULE, RTLD_NOW);
	#elif _WIN32
	g_hOrigModule = (void*)LoadLibraryA(ZMODULE);
	if (!g_hOrigModule) g_hOrigModule = (void*)LoadLibraryA(LMODULE);
	#endif
	
	//Print about info
	printf("dnyApocServ for %s by Daniel Brendel\n", OPERATINGSYSTEM);
	
	//Get amount of bytes to patch for GetProcAddress/dlsym from config
	int lOpLen = 0;
	if (!ReadPatchLen(SSH_CONFIG, &lOpLen)) {
		lOpLen = 0x05;

		printf("Could not load config from \"%s\", using default value: %d\n", SSH_CONFIG, lOpLen);
	}

	//Detour the specific function
	#ifdef __linux__
	orig_dlsym = (T_dlsym)(detour_func((unsigned char*)&dlsym, (const unsigned char*)&new_dlsym, lOpLen));
	#else
	orig_GetProcAddress = (T_GetProcAddress)JMPDetour(GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetProcAddress"), &new_GetProcAddress, lOpLen);
	#endif

	//Add server-sided password command
	g_pEngFuncs->pfnAddServerCommand((char*)"ssh_password", &Set_Password);

	//Get address and call it from original module
	#ifdef __linux__
	orig_GiveFnptrsToDll = (T_GiveFnptrsToDll)dlsym(hOrigModule, "GiveFnptrsToDll");
	#else
	orig_GiveFnptrsToDll = (T_GiveFnptrsToDll)GetProcAddress((HMODULE)g_hOrigModule, "GiveFnptrsToDll");
	#endif

	orig_GiveFnptrsToDll(pengfuncsFromEngine, pGlobals);
}
//======================================================================

#ifdef _WIN32
//======================================================================
EXPORT_LIBRARY BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved) 
{ 
	return TRUE; 
}
//======================================================================
#endif