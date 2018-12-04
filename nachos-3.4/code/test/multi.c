#include "syscall.h"

// ./userprog/nachos -rs 1023 -x ./test/multi
int
main()
{
    char * fun;
    Exec("./test/ping");
    Exec("./test/pong");
    ConsoleRead(fun, 1);
    Halt();    
    return 0;
}
