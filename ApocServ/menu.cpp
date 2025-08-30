#include "menu.h"
#include "usermsgs.h"
#include "global.h"

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: menu.cpp
	Purpose: Menu implementation
*/

//======================================================================
void CMenu::SetBit(unsigned int* iValue, unsigned int iPos)
{
	//Set a specific bit of a dword
	if (iValue) {
		//Set lowest bit to 1 and shift to the specified positon - 1
		unsigned int toXor = 1;
		toXor <<= iPos - 1;

		//Make XOR operation. Bit at positon is xored with 1 to set it if it isn't set.
		//All other bits are xored with 0 to hold their value.
		*iValue ^= toXor;
	}
}
//======================================================================

//======================================================================
void CMenu::ShowMenuMsg(edict_s* pEntity, short sBits, int iDisplayTime, unsigned char fNeedMore, char* szText)
{
	//Send ShowMenu usermsg to player to initialize and finish the menu showing
	if ((bEnabled) && (pEntity) && (szText) && (strlen(szText) < MAX_MENU_STRING)) {
		g_pEngFuncs->pfnMessageBegin(MSG_ONE, gUserMsgs.GetMsgByName("ShowMenu"), NULL, pEntity);
		g_pEngFuncs->pfnWriteShort(sBits);
		g_pEngFuncs->pfnWriteChar(iDisplayTime);
		g_pEngFuncs->pfnWriteByte(fNeedMore);
		g_pEngFuncs->pfnWriteString(szText);
		g_pEngFuncs->pfnMessageEnd();
	}
}
//======================================================================

//======================================================================
bool CMenu::Initialize(const char* szMenuName, int iDisplayTime, TSelectionHandler handler)
{
	//Initialize the menu class object

	//Return if already enabled
	if (bEnabled)
		return bEnabled;

	bEnabled = false;

	if ((szMenuName) && (strlen(szMenuName) < MAX_MENU_STRING) && (handler)) {
		//Set all values
		strcpy(this->szName, szMenuName);
		this->iDisplayTime = iDisplayTime;
		iBits = 0;
		bShown = false;
		gSelectionHandler = handler;
		
		vEntries.clear();

		//Set lowest bit for menu title
		SetBit(&iBits, 1);

		//Success
		bEnabled = true;
	}

	return bEnabled;
}
//======================================================================

//======================================================================
void CMenu::AddSelection(const char* szSelectionName)
{
	//Add new entry to menu
	if ((bEnabled) && (szSelectionName) && (strlen(szSelectionName) < MAX_MENU_STRING)) {
		//Store selection name buffer
		menuentry_s mnue;
		memset(&mnue, 0x00, sizeof(menuentry_s));
		strcpy(mnue.szSelectionName, szSelectionName);
		vEntries.push_back(mnue);

		//Set next bit for each entry
		SetBit(&iBits, vEntries.size() + 1);
	}
}
//======================================================================

//======================================================================
void CMenu::ProcessInput(edict_s* pEntity, const char* szSelection)
{
	//Process input if client has sent 'menuselect n'
	if ((bEnabled) && (bShown) && (pEntity) && (szSelection) && (gSelectionHandler)) {
		//Convert to integer
		int iSelection = atoi(szSelection);

		//If selection is in menu entry amount range call handler
		if (iSelection <= vEntries.size()) {
			gSelectionHandler(pCurrentEnt, iSelection);
		}
		
		pCurrentEnt = NULL;
		bShown = false;
	}
}
//======================================================================

//======================================================================
void CMenu::Show(edict_s* pEntity)
{
	//Show a menu to a player
	if ((bEnabled) && (!bShown) && (pEntity)) {
		//Initialize menu with title
		ShowMenuMsg(pEntity, iBits, iDisplayTime, true, szName);

		//Add all entries except the last
		for (unsigned int i = 0; i < vEntries.size() - 1; i++) {
			ShowMenuMsg(pEntity, iBits, iDisplayTime, true, vEntries[i].szSelectionName);
		}
		
		//Add last entry and tell client to now show the menu
		ShowMenuMsg(pEntity, iBits, iDisplayTime, false, vEntries[vEntries.size() - 1].szSelectionName);
		
		//Menu is now active and set current entity pointer for player
		pCurrentEnt = pEntity;
		bShown = true;
	}
}
//======================================================================

//======================================================================
void CMenu::Clear(void)
{
	//Clean up menu data
	if (bEnabled) {
		memset(szName, 0x00, sizeof(szName));
		iBits = 0;
		bShown = false;
		vEntries.clear();
		iDisplayTime = 0;
		gSelectionHandler = NULL;

		bEnabled = false;
	}
}
//======================================================================

//======================================================================
CMenu gActionMenu;
//======================================================================