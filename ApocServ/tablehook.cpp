#include "tablehook.h"

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: tablehook.cpp
	Purpose: Table Hook class implementation

*/

//======================================================================
bool CTableHook::Initialize(void* pTable, unsigned int dwTableSize)
{
	//Initialize table hook object

	//Check if already enabled
	if (bEnabled)
		return bEnabled;

	bEnabled = false;

	if ((pTable) && (dwTableSize) && ((dwTableSize%sizeof(unsigned int)) == 0)) {
		//Save values
		pOrigTable = (unsigned int*)pTable;
		this->dwTableSize = dwTableSize;

		//Make backup
		pBackup = new unsigned int[dwTableSize / sizeof(unsigned int)];
		memcpy(pBackup, pTable, dwTableSize);

		bEnabled = true;
	}

	return bEnabled;
}
//======================================================================

//======================================================================
void CTableHook::AddNewName(const char* szProcName, unsigned int fId)
{
	//Add new function alias name
	if ((szProcName) && (strlen(szProcName) < MAX_FUNC_NAME)) {
		tablehook_s th;
		th.fId = fId;
		strcpy(th.fName, szProcName);

		vNames.push_back(th);
	}
}
//======================================================================

//======================================================================
unsigned int CTableHook::GetNameAmount(void)
{
	//Return amount of existring alias names
	return vNames.size();
}
//======================================================================

//======================================================================
unsigned int CTableHook::GetProcId(const char* szProcName)
{
	//Return ID of a function by name
	if (!szProcName) return INVALID_PROC_ID;

	for (unsigned int i = 0; i < vNames.size(); i++) {
		if (strcmp(vNames[i].fName, szProcName) == 0) //If alias name found return its id
			return i;
	}

	return INVALID_PROC_ID;
}
//======================================================================

//======================================================================
void* CTableHook::Hook(unsigned int fId, void* pNew)
{
	//Hook a function by its ID
	if ((bEnabled) && (fId != INVALID_PROC_ID) && (pNew)) {
		//Hook entry and return original address
		pOrigTable[fId] = (unsigned int)pNew;
		return (void*)pBackup[fId];
	}

	return NULL;
}
//======================================================================

//======================================================================
void* CTableHook::Hook(const char* szProcName, void* pNew)
{
	//Hook a function by its alias name
	if ((bEnabled) && (pNew)) {
		unsigned int fId = GetProcId(szProcName);
		if (fId) {
			//Hook entry and return original address
			pOrigTable[fId] = (unsigned int)pNew;
			return (void*)pBackup[fId];
		}
	}

	return NULL;
}
//======================================================================

//======================================================================
void CTableHook::Restore(unsigned int fId)
{
	//Restore a hooked entry of the original table by its ID
	if ((bEnabled) && (fId != INVALID_PROC_ID)) {
		pOrigTable[fId] = pBackup[fId];
	}
}
//======================================================================

//======================================================================
void CTableHook::Restore(const char* szProcName)
{
	//Restore a hooked entry of the original table by its alias name
	if ((bEnabled) && (szProcName) && (strlen(szProcName) < MAX_FUNC_NAME)) {
		unsigned int fId = GetProcId(szProcName);
		if (fId != INVALID_PROC_ID) {
			pOrigTable[fId] = pBackup[fId];
		}
	}
}
//======================================================================

//======================================================================
void CTableHook::Clear(bool bCopyBack)
{
	//Clean up stuff
	if (bEnabled) {
		if (bCopyBack) //Restore backup if needed
			memcpy(pOrigTable, pBackup, dwTableSize);

		//Free memory and zero variables
		delete [] pBackup;
		pBackup = NULL;
		pOrigTable = NULL;
		dwTableSize = 0;

		bEnabled = false;
	}
}
//======================================================================

//======================================================================
CTableHook gEngTable, gDllTable, gNewDllTable;
//======================================================================