#include "syscall.h"

void main()
{
    int i;
    for (i = 0; i < 200; i++)
        PrintChar('A');
    ConsolePrint(" end ");
}