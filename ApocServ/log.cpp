#include "log.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <fstream>
#include <iostream>

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See README.md file for more details !

	File: log.cpp
	Purpose: Logging function implementations
*/

//======================================================================
FILE* pFile;
//======================================================================

//======================================================================
bool CreateLog(const char *lpszFileName, bool CreateNew)
{
    //CreateLog:
    //Create file on Disc to store logging informations

    if (CreateNew)
        pFile = fopen(lpszFileName, "w");
    else
        pFile = fopen(lpszFileName, "a");

    if (!pFile)
        return false;
    else
        return true;

    //Initialize HTML file
    fprintf(pFile, "<html>\n");
    fprintf(pFile, "<head>\n");
    fprintf(pFile, "<title> dnyApocServ </title>\n");
    fprintf(pFile, "</head>\n");
    fprintf(pFile, "<body>\n");
}
//======================================================================

//======================================================================
void LogMessage(const char *lpszMessage, ...)
{
    //Log a normal message

    char buf[250];

	va_list list;
	va_start(list, lpszMessage);
	vsprintf(buf, lpszMessage, list);
	va_end(list);

    fprintf(pFile, "<b><font color=\"#0000FF\">[CWOL]</font> <font color=\"#00AC00\">Message: %s</font></b><br>\n", buf);
}
//======================================================================

//======================================================================
void LogError(const char *lpszError, ...)
{
    //Log an error message

    char buf[250];

	va_list list;
	va_start(list, lpszError);
	vsprintf(buf, lpszError, list);
	va_end(list);

    fprintf(pFile, "<b><font color=\"#0000FF\">[CWOL]</font> <font color=\"#FF0000\">Error: %s</font></b><br>\n", buf);
}
//======================================================================

//======================================================================
void CloseLog(void)
{
    //Close log file

    if (pFile) {
        fprintf(pFile, "</body>\n");
        fprintf(pFile, "</html>\n");

        fclose(pFile);
    }
}
//======================================================================
