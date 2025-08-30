#ifndef _USERMSGS_H
#define _USERMSGS_H

#include "sdkinclude.h"
#include <vector>

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: usermsgs.h
	Purpose: User message handler interface
*/

//======================================================================
#define MAX_MSG_NAME 50
//======================================================================

//======================================================================
struct usermsg_s {
	char szMsg[MAX_MSG_NAME];
	int mId;
};
//======================================================================

//======================================================================
class CUserMsgs {
private:
	std::vector<usermsg_s> vMsgs;
public:
	void AddNewMsg(const char* szMsg, int mId);
	int GetMsgByName(const char* szMsg);
	char* GetMsgById(int iMsg);
	void Clear(void);
};
//======================================================================

//======================================================================
extern CUserMsgs gUserMsgs;
//======================================================================

#endif