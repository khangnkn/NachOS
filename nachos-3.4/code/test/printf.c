/*#include"syscall.h"
int main()
{
	ConsolePrint("Thanh Huong Nguyen");
	return 0;
}*/
#include"syscall.h"
#include"copyright.h"
int main()
{
	char buffer[256];
	char str[256];
	int f;
	int len;

	ConsolePrint("file name = ");
	ConsoleRead(str,256);

	f=Open(str,1);

	if(f==-1)
	{
		ConsolePrint("Fail..\n");
	}
	else
	{
		 len = Read(buffer, 256, f);
		if(len==-1 | len ==-2)
		{
			ConsolePrint("Fail..\n");	
		}
		else
		{
			int stdout = Open("stdout",3);
			int a = Write(buffer,len,stdout);
			Close(stdout);
		}
		Close(f);
	}

	return 0;
}
