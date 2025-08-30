#ifndef _TABLEHOOK_H
#define _TABLEHOOK_H

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: tablehook.h
	Purpose: Table Hook interface
*/

#include "sdkinclude.h"
#include <vector>

//======================================================================
#define MAX_FUNC_NAME 25
#define INVALID_PROC_ID 0xFFFFFFFF
//======================================================================

//======================================================================
struct tablehook_s {
	char fName[MAX_FUNC_NAME];
	unsigned int fId;
};
//======================================================================

//======================================================================
class CTableHook {
private:
	unsigned int *pOrigTable;
	unsigned int *pBackup;
	unsigned int dwTableSize;
	bool bEnabled;
	std::vector<tablehook_s> vNames;
	unsigned int GetNameAmount(void);
	unsigned int GetProcId(const char* szProcName);
public:
	CTableHook() { pOrigTable = NULL; pBackup = NULL; dwTableSize = 0; bEnabled = false; }
	~CTableHook() { Clear(false); }
	bool Initialize(void* pTable, unsigned int dwTableSize);
	void AddNewName(const char* szProcName, unsigned int fId);
	void* Hook(unsigned int fId, void* pNew);
	void* Hook(const char* szProcName, void* pNew);
	void Restore(unsigned int fId);
	void Restore(const char* szProcName);
	void Clear(bool bCopyBack);
};
//======================================================================

//======================================================================
extern CTableHook gEngTable, gDllTable, gNewDllTable;
//======================================================================

#endif