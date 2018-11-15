#include "syscall.h"
#define MAX_LENGTH 32
int main()
{
	int stdin;
	char fileName[MAX_LENGTH];
	//ConsolePrint("\n\t\t\t-----Program createfile-----\n\n");
	ConsolePrint("Input FileName ");
	//ReadString(fileName, MAX_LENGTH);//Doc ten file nhap tu consile <stdin> save vo fileName
	stdin = Open("stdin", 2); // Goi ham Open mo file stdin nhap vao ten file, 2<=>stdin
	if(stdin != -1)//Thanh cong
	{
		int len = Read(fileName, MAX_LENGTH, stdin); // Goi ham Read doc ten file vua nhap <tu stdin>
		if (len != -1 && len != -2) //Kiem tra co bi loi, hay co EOF hay khong
		{
			if (CreateFile(fileName)==0) // Goi ham CreateFile
			{
				ConsolePrint("Create file Success...\n\n");
			}

		}
		else
			ConsolePrint("FileName is Invalid...!\n\n");
	Close(stdin); // Goi ham Close de dong stdin
	}
	return 0;
}
