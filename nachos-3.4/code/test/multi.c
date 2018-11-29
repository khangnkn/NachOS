#include "syscall.h"

// ./userprog/nachos -rs 1023 -x ./test/multi
int
main()
{
    ConsolePrint("Entry...");
    Exec("./test/echo");
    ConsolePrint("\nEnd");
    return 0;
}