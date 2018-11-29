#include "syscall.h"
#define TRUE 1
int str_cpy(char* dest, const char* src){

     return 0;
 }
int main(int argc, char *argv[])
{
    char buf[255];
    char * filename_in;
    char * filename_out;
    int size = 255;
    int rSize;
    int stdin;
    int srcFile;
    int desFile;
    int in = 0;
    for (; in < argc; in++)
    {
        ConsolePrint(argv[in]);
        ConsolePrint("\n");
    }
    return;
    if (argc == 1|| argc == 2)
    {
        ConsolePrint("\nFile input:");
        ConsoleRead(filename_in, 32);


        ConsolePrint("\nFile output:");
        ConsoleRead(filename_out, 32);
        srcFile = Open(filename_in, 1);
        CreateFile(filename_out);
   
        desFile = Open(filename_out, 0);
    }
    else {
        srcFile = Open(argv[1], 1);
        CreateFile(argv[2]);
        desFile = Open(argv[2], 0);
        ConsolePrint("open 2 file");
    }

    if(srcFile!= -1 && desFile!= -1){
        rSize = Read(buf, size, srcFile);
        while (rSize != -2 && rSize != -1)
        {
            Write(buf, rSize, desFile);
            rSize = Read(buf, size, srcFile);
        }
        Close(srcFile);
        Close(desFile);
    }
    
    ConsolePrint("Finish!");
    return 0;
}
