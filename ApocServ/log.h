#ifndef _LOG_H
#define _LOG_H

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: log.h
	Purpose: Logging interface
*/

//======================================================================
bool CreateLog(const char *lpszFileName, bool CreateNew);
void LogMessage(const char *lpszMessage, ...);
void LogError(const char *lpszError, ...);
void CloseLog(void);
//======================================================================

#endif
