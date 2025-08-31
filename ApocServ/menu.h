#ifndef _MENU_H
#define _MENU_H

#include "sdkinclude.h"
#include <vector>

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: menu.h
	Purpose: Menu interface
*/

//======================================================================
#define MAX_MENU_STRING 30
//======================================================================

//======================================================================
typedef void (*TSelectionHandler)(edict_s* pEntity, int iSelection);
//======================================================================

//======================================================================
struct menuentry_s {
	char szSelectionName[MAX_MENU_STRING];
};
//======================================================================

//======================================================================
class CMenu {
private:
	char szName[MAX_MENU_STRING];
	int iDisplayTime; //How long shall the menu be displayed?
	unsigned int iBits; //For each menu entry set a bit (1 - n)
	std::vector<menuentry_s> vEntries;
	bool bEnabled;
	bool bShown;
	TSelectionHandler gSelectionHandler;
	edict_s* pCurrentEnt;
	void SetBit(unsigned int* iValue, unsigned int iPos);
	void ShowMenuMsg(edict_s* pEntity, short sBits, int iDisplayTime, unsigned char fNeedMore, char* szText);
public:
	CMenu() { bEnabled = false; gSelectionHandler = NULL; iDisplayTime = 0; iBits = 0; bShown = false; memset(szName, 0x00, sizeof(szName)); }
	bool Initialize(const char* szMenuName, int iDisplayTime, TSelectionHandler handler);
	bool IsReady(void) { return bEnabled; }
	bool IsShown(void) { return bShown; }
	void AddSelection(const char* szSelectionName);
	void ProcessInput(edict_s* pEntity, const char* szSelection);
	void Show(edict_s* pEntity);
	void Clear(void);
};
//======================================================================

extern CMenu gActionMenu;
extern CMenu gPlayerMenu;

#endif