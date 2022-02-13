#include <cstdio>
#include <stdio.h>
#include <stdint.h>  // To use uint64_t

// Windows API
#include <windows.h>
#include <intrin.h>
#include <sysinfoapi.h>
#include <processthreadsapi.h>
#include <winbase.h>
//#include "wdbgexts.h"

// Remember to use "-I/path/to/plog/include" to compile  
// else will report not found error..
#include <plog/Log.h> 
#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Appenders/ConsoleAppender.h"
#include "plog/Formatters/MessageOnlyFormatter.h"
//#include "include/spdlog/spdlog.h"

// processorNumber start from 0->(Max-1)
DWORD_PTR getAffinityMask(int processorNumber){
    return (1 << processorNumber);
}

// Ignore this for now...
// MSR_CORE_THREAD_COUNT 0x35 MSR?
// https://opensource.apple.com/source/xnu/xnu-1504.9.37/osfmk/i386/cpuid.c
// core_count = bit 31:16
// thread_count = bit 15:0
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

// https://stackoverflow.com/a/44247223
size_t NumberOfSystemCores() noexcept {
    SYSTEM_INFO system_info;
    ZeroMemory(&system_info, sizeof(system_info));
    
    GetSystemInfo(&system_info);
    
    return static_cast< size_t >(system_info.dwNumberOfProcessors);
}

// Initailize logger
// For more info, https://github.com/SergiusTheBest/plog#readme
void plog_init(){
    plog::init(plog::debug, "foo.log", 0, 0);
    static plog::ConsoleAppender<plog::MessageOnlyFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
}

void foo(){
    int numberOfLogicalProcessor = NumberOfSystemCores();
    HANDLE currentProcessHandle = GetCurrentProcess();
    DWORD originalProcessor = GetCurrentProcessorNumber();
    DWORD_PTR processAffinityMask;

    printf("\n\nDEM!");
    printf("\n Original Tread: %d", originalProcessor);
    printf("\n Number of Logical Cores: %d", numberOfLogicalProcessor);

    for (int processor = 0; processor < numberOfLogicalProcessor; processor++){
        if (processor == originalProcessor){
            continue;
        }
        printf("\n%d", processor);
        SetProcessAffinityMask(currentProcessHandle, getAffinityMask(processor));
        printf("\nCurrent Thread: %d", GetCurrentProcessorNumber());
    }
    SetProcessAffinityMask(currentProcessHandle, getAffinityMask(originalProcessor));
    printf("\nReset Thread: %d", GetCurrentProcessorNumber());
}

int main(){
    HANDLE currentProcessHandle;
    DWORD currentLogicalProcessor;
    DWORD_PTR processAffinityMask;
    
    plog_init();
    currentProcessHandle = GetCurrentProcess();
    currentLogicalProcessor = GetCurrentProcessorNumber();
    PLOGD << "Current Logical Processor: " << currentLogicalProcessor;
    // GetCurrentProcess()
    //printf("\n%d", currentProcessHandle);
    PLOGD << "Setting process affinity to logical processor 7";
    SetProcessAffinityMask(currentProcessHandle, getAffinityMask(7));
    PLOGD << "Current Logical Processor: " << GetCurrentProcessorNumber();
    //getNumberOfLogicalProcessor();
    PLOGD << "Number of Logical Processor: " << NumberOfSystemCores();

    //foo();
    return 0;
}


int main2()
{

    plog_init();
    PLOGD << "Hello log!"; // short macro
    PLOG_DEBUG << "Hello log!"; // long macro
    PLOG(plog::debug) << "Hello log!"; // function-style macro
    
    // Also you can use LOG_XXX macro but it may clash with other logging libraries
    LOGD << "Hello log!"; // short macro
    LOG_DEBUG << "Hello log!"; // long macro
    LOG(plog::debug) << "Hello log!"; // function-style macro

    return 0;
}