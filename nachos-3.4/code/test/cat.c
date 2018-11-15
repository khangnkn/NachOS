#include "syscall.h"
#define MAX_LENGTH 32

int main()
{
	int srcFile;
	int stdout;
	int size = 255;
	int rSize;
	char * buffer;
	char buf[255];
	int i = 1;
	ConsolePrint("Input filename to read: ");
    ConsoleRead(buffer, 32);//Doc chuoi vua nhap luu vao buffer

	srcFile = Open(buffer, 1); // Goi ham Open de mo file do, 1<=> only read
	stdout = Open("stdout", 3);
	if (srcFile != -1) //Kiem tra Open co loi khong
	{
		ConsolePrint("Opened\n");
		rSize = Read(buf, size, srcFile);
		ConsolePrint("Read\n");
		
		while (rSize != 0)
		{

			Write(buf, rSize, stdout);
			rSize = Read(buf, size, srcFile);
		}
	}
	else

	{
		ConsolePrint("Not Open File to read...\n\n");
	}
	Close(srcFile);
	return 0;
}

