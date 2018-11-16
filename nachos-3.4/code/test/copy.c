#include "syscall.h"
#define TRUE 1

int main(int argc, char const *argv[])
{
    char buf[255];
    char filename_in[32];
    char filename_out[32];
    int size = 255;
    int rSize;
    int stdin;
    int srcFile;
    int desFile;

    stdin = Open("stdin", 2);
    ConsolePrint("\nFile input:");
    ConsoleRead(filename_in, 32);
    //Read(filename_in, 32, stdin);
    srcFile = Open(filename_in, 1);

    ConsolePrint("\nFile output:");
    ConsoleRead(filename_out, 32);
    //Read(filename_out, 32, stdin);
    CreateFile(filename_out);
   
    desFile = Open(filename_out, 0);

    rSize = Read(buf, size, srcFile);
    while (rSize != 0 && rSize != -1)
    {
        Write(buf, rSize, desFile);
        rSize = Read(buf, size, srcFile);
    }

    Close(srcFile);
    Close(desFile);
    ConsolePrint("Finish!");
    return 0;
}
