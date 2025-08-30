#include "clientcmd.h"

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: clientcmd.cpp
	Purpose: Client command class inmplementation
*/

//======================================================================
unsigned int CClientCmd::GetAmount(void)
{
	//Return amount of existing cmd entries
	return vCmds.size();
}
//======================================================================

//======================================================================
void CClientCmd::AddHandler(TCmdProc pCmdProc, const char* szCmd)
{
	//Add new client command handler
	if ((pCmdProc) && (szCmd) && (strlen(szCmd) < MAX_CMD_LEN)) {
		//Add new handler to memory
		clientcmd_s cc;

		cc.pCmdProc = pCmdProc;
		strcpy(cc.szCmd, szCmd);

		vCmds.push_back(cc);
	}
}
//======================================================================

//======================================================================
bool CClientCmd::CmdEqual(unsigned int id, const char* szToCompare)
{
	//Check if list entry is the wanted command handler
	if ((id < vCmds.size()) && (szToCompare) && (strlen(szToCompare) < MAX_CMD_LEN))
		return strcmp(vCmds[id].szCmd, szToCompare) == 0;

	return false;
}
//======================================================================

//======================================================================
bool CClientCmd::CallHandler(unsigned int id, edict_s* pEntity)
{
	//Call client command handler
	if ((id < vCmds.size()) && (pEntity)) {
		return vCmds[id].pCmdProc(pEntity);
	}

	return false;
}
//======================================================================

//======================================================================
void CClientCmd::ClearAll(void)
{
	//Clean up the list
	vCmds.clear();
}
//======================================================================

//======================================================================
CClientCmd gClientCmd;
//======================================================================