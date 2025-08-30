#include "usermsgs.h"

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: usermsgs.cpp
	Purpose: User message class implementation
*/

//======================================================================
void CUserMsgs::AddNewMsg(const char* szMsg, int mId)
{
	//Add a new user message entry to list

	if ((szMsg) && (strlen(szMsg) < MAX_MSG_NAME) && (mId != -1)) {
		usermsg_s msg;
		msg.mId = mId;
		strcpy(msg.szMsg, szMsg);
		vMsgs.push_back(msg);
	}
}
//======================================================================

//======================================================================
int CUserMsgs::GetMsgByName(const char* szMsg)
{
	//Get a message id by its name

	//Check arguments
	if ((!szMsg) || (strlen(szMsg) > MAX_MSG_NAME))
		return -1;

	for (unsigned int i = 0; i < vMsgs.size(); i++) {
		//Check if entry with name exists
		if (strcmp(vMsgs[i].szMsg, szMsg) == 0)
			return vMsgs[i].mId;
	}

	return -1;
}
//======================================================================

//======================================================================
char* CUserMsgs::GetMsgById(int iMsg)
{
	//Get a message name by its id

	//Check arguments
	if (iMsg == -1)
		return NULL;

	for (unsigned int i = 0; i < vMsgs.size(); i++) {
		//Check if entry with id exists
		if (vMsgs[i].mId == iMsg)
			return &vMsgs[i].szMsg[0];
	}

	return NULL;
}
//======================================================================

//======================================================================
void CUserMsgs::Clear(void)
{
	//Clean up memory
	vMsgs.clear();
}
//======================================================================

//======================================================================
CUserMsgs gUserMsgs;
//======================================================================