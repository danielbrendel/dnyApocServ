#ifndef _CLIENTCMD_H
#define _CLIENTCMD_H

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See README.md file for more details !

	File: clientcmd.h
	Purpose: Client Command interface
*/

#include "sdkinclude.h"
#include <vector>

//======================================================================
#define MAX_CMD_LEN 30
//======================================================================

//======================================================================
typedef bool (*TCmdProc)(edict_s* pEntity);
//======================================================================

//======================================================================
struct clientcmd_s {
	TCmdProc pCmdProc;
	char szCmd[MAX_CMD_LEN];
};
//======================================================================

//======================================================================
class CClientCmd {
private:
	std::vector<clientcmd_s> vCmds;
	void ClearAll(void);
public:
	~CClientCmd() { ClearAll(); }
	unsigned int GetAmount(void);
	void AddHandler(TCmdProc pCmdProc, const char* szCmd);
	bool CmdEqual(unsigned int id, const char* szToCompare);
	bool CallHandler(unsigned int id, edict_s* pEntity);
};
//======================================================================

//======================================================================
extern CClientCmd gClientCmd;
//======================================================================

#endif