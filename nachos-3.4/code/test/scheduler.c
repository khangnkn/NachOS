#include "syscall.h"

// ./userprog/nachos -rs 1023 -x ./test/multi
int
main()
{
    char * fun;
    ConsolePrint(" \tAfter excuting ping/pong, press [ENTER] to halt.\n");
    Exec("./test/ping");
    Exec("./test/pong");
    
    ConsoleRead(fun, 1);  
    return 0;
}
