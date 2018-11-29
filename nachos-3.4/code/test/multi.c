#include "syscall.h"

// ./userprog/nachos -rs 1023 -x ./test/multi
int
main()
{
    char * fun;
    ConsolePrint("Entry...");
    Exec("./test/ping");
    //Exec("./test/cat");
    // Exec("./test/pong");
    //ConsoleRead(fun, 255);
    return 0;
}