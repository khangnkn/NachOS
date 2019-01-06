#include "syscall.h"

int main()
{
    int i;
    for(i = 0;i<5;i++)
    {
        Down("S3");
        ConsolePrint("A xuat hien\n");
        Up("S2");
        Down("S1");
        ConsolePrint("A chao B\n");
        Up("S4");
    }
    
    Exit(0);
    return 0;
}
