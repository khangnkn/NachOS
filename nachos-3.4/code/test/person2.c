#include"syscall.h"
int main()
{
    int i;
   for(i =0;i<5;i++)
    {
        Down("S4");
        ConsolePrint("B xuat hien\n");
        Up("S1");
        Down("S2");
        ConsolePrint("B chao A\n");
        Up("S3");
    }
    
    Exit(0);
    return 0;
}
