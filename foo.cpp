#include <cstdio>
#include "processthreadsapi.h"


int main(){
    printf("%d",GetCurrentProcessorNumber());
    return 0;
}