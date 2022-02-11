#include <cstdio>
#include <stdio.h>
#include <windows.h>
#include "processthreadsapi.h"
#include "winbase.h"
//#include "wdbgexts.h"

// processorNumber start from 0->(Max-1)
DWORD_PTR getAffinityMask(int processorNumber){
    return (1 << processorNumber);
}

// Ignore this for now...
// MSR_CORE_THREAD_COUNT 0x35 MSR?
int getNumberOfLogicalProcessor(){
    int numberOfLogicalProcessor;
    __asm__(
        "movl $0xB, %%eax\n"
        "cpuid\n"
        "movl %%ebx, %0\n"
        :"=r" (numberOfLogicalProcessor)
    );
    printf("\n%d", numberOfLogicalProcessor);
    return (numberOfLogicalProcessor);
}


int main(){
    HANDLE currentProcessHandle;
    DWORD currentLogicalProcessor;
    DWORD_PTR processAffinityMask;

    currentProcessHandle = GetCurrentProcess();
    currentLogicalProcessor = GetCurrentProcessorNumber();
    printf("%d", currentLogicalProcessor);
    // GetCurrentProcess()
    //printf("\n%d", currentProcessHandle);
    SetProcessAffinityMask(currentProcessHandle, getAffinityMask(0));
    printf("\n%d", GetCurrentProcessorNumber());
    //getNumberOfLogicalProcessor();
    //printf("\n%d", getNumberOfLogicalProcessor());
    return 0;
}