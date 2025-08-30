#include "detour.h"

/*
	ApocServ Server-side Hack for GoldSrc CS/CZ
	Developed by Daniel Brendel

	! See readme.md file for more details !

	File: detour.cpp
	Purpose: Detouring function implementations
*/

#ifdef __linux__

#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

//======================================================================
void* detour_func(unsigned char* src, const unsigned char* dst, const int len)
{
	unsigned char* jmp = (unsigned char*)malloc(len+5);
	if (!jmp) return 0;
	
	unsigned long mask = ~(sysconf(_SC_PAGESIZE)-1);
	unsigned long page = (unsigned long)src & mask;

	mprotect((void*)page, len, PROT_READ|PROT_WRITE|PROT_EXEC);

	memcpy(jmp, src, len); jmp += len;

	jmp[0] = 0xE9;
	*(unsigned long*)(jmp+1) = (unsigned long)(src+len - jmp) - 5;

	src[0] = 0xE9;
	*(unsigned long*)(src+1) = (unsigned long)(dst - src) - 5;	
	

	return (jmp-len);
}
//======================================================================

#else

#include <windows.h>
 
//======================================================================
void* JMPDetour(void*OrigAddr, void*NewAddr, unsigned int uiSize)
{
    //Simple JMP detour function

    #ifndef NOP
    #define NOP 0x90
    #endif

    #ifndef JMP
    #define JMP 0xE9
    #endif

    if ((!OrigAddr) || (!NewAddr) || (uiSize < 5)) return NULL; //Check for invalid parameters

    void*ReturnAddr = malloc(uiSize + 0x05); //Address of gateway
	if (!ReturnAddr) return NULL;

    unsigned char JMPArrayO[5] = {JMP, 0x00, 0x00, 0x00, 0x00}; //Jump back to original address
    unsigned char JMPArrayN[5] = {JMP, 0x00, 0x00, 0x00, 0x00}; //Jump to new address

    *(unsigned int*)(&JMPArrayO[1]) = ((unsigned int)(OrigAddr) + uiSize) - (unsigned int)ReturnAddr - (uiSize+0x05); //Set relative address

    memcpy(ReturnAddr, OrigAddr, uiSize); //Copy the original bytes to gateway which will be overwritten
    memcpy((void*)((unsigned int)(ReturnAddr)+uiSize), JMPArrayO, uiSize+0x05); //Copy "Back Jump" to gateway code

    *(unsigned int*)(&JMPArrayN[1]) = (unsigned int)NewAddr - (unsigned int)OrigAddr - 0x05; //Set relative address

    DWORD dwProtect;

    VirtualProtect(OrigAddr, uiSize, PAGE_EXECUTE_READWRITE, &dwProtect); //Change permission rights to the code which to overwrite

    if (uiSize > 5) { //If the code we want to overwrite is greater then 5 then fill with NOPs to prevent corrupting code
        for(unsigned int g = 0x00; g < uiSize; g++) {
            *(unsigned char*)((unsigned int)(OrigAddr) + g) = NOP;
        }
    }

    memcpy(OrigAddr, JMPArrayN, 0x05); //Now overwrite code to let the function jump to the new function

    VirtualProtect(OrigAddr, uiSize, dwProtect, &dwProtect); //Restore permission rights

    return ReturnAddr; //Return gateway address
}
//======================================================================

#endif
