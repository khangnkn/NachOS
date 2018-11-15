#include "exceptionHandler.h"

void IncreasePC()
{
    int counter = machine->ReadRegister(PCReg);
   	machine->WriteRegister(PrevPCReg, counter);
    counter = machine->ReadRegister(NextPCReg);
    machine->WriteRegister(PCReg, counter);
   	machine->WriteRegister(NextPCReg, counter + 4);
}

char* User2System(int virtAddr, int limit)
{
	int i; //chi so index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; //can cho chuoi terminal
	if (kernelBuf == NULL)
		return kernelBuf;
		
	memset(kernelBuf, 0, limit + 1);
	
	for (i = 0; i < limit; i++)
	{
		machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}

int System2User(int virtAddr, int len, char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0;
	do{
		oneChar = (int)buffer[i];
		machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}

void handleSC_Halt()
{
	DEBUG('a', "\nShutdown, initiated by user program. ");
	printf("\nShutdown, initiated by user program. ");
	interrupt->Halt();
	return;
}

void handleSC_Create()
{
    int virtAddr = machine->ReadRegister(4);
    char * filename;
    bool fCreate;

    filename = User2System(virtAddr, 33);

    if (strlen(filename) == 0)
    {
        DEBUG('a', "Invalid filename.\n");
        printf("Invalid filename.\n");
        machine->WriteRegister(2, -1);
        IncreasePC();
        return;
    }

    if (filename == NULL)
    {
        DEBUG('a', "Filename is NULL.\n");
        machine->WriteRegister(2, -1);
        IncreasePC();
        return;
    }

    fCreate = fileSystem->Create(filename, 0);
    if (fCreate == 0)
    {
        DEBUG('a', "\n Error create file '%s'", filename);
        printf("Error create file %s", filename);
		machine->WriteRegister(2, -1);
		delete filename;
		IncreasePC();
		return;
    }

    machine->WriteRegister(2, 0);
    delete filename;
    IncreasePC();
    return;
}

void handleSC_ConsoleRead()
{
	// Input: Buffer(char*), do dai toi da cua chuoi nhap vao(int)
	// Output: Khong co
	// Cong dung: Doc vao mot chuoi voi tham so la buffer va do dai toi da
	int virtAddr, length;
	char* buffer;
	virtAddr = machine->ReadRegister(4); // Lay dia chi tham so buffer truyen vao tu thanh ghi so 4
	length = machine->ReadRegister(5); // Lay do dai toi da cua chuoi nhap vao tu thanh ghi so 5
	buffer = User2System(virtAddr, length); // Copy chuoi tu vung nho User Space sang System Space
	gSynchConsole->Read(buffer, length); // Goi ham Read cua SynchConsole de doc chuoi
	System2User(virtAddr, length, buffer); // Copy chuoi tu vung nho System Space sang vung nho User Space
	delete buffer; 
	IncreasePC(); // Tang Program Counter 
	return;
}

void handleSC_ConsolePrint()
{
	//void String(char*buffer)
	// Input: Buffer(char*)
	// Output: Chuoi doc duoc tu buffer(char*)
	// Cong dung: Xuat mot chuoi la tham so buffer truyen vao ra man hinh console
	int virtAddr;
	char* buffer;
	virtAddr = machine->ReadRegister(4);
	buffer = User2System(virtAddr, 255); // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai 255 ki tu
	int length = 0;
	while (buffer[length] != 0) length++; // Dem do dai that cua chuoi

	gSynchConsole->Write(buffer, 255); // Goi ham Write cua SynchConsole de in chuoi
	delete buffer; 
	IncreasePC(); // Tang Program Counter 
	return;
}

void handleSC_PrintChar()
{
	char c = (char)machine->ReadRegister(4);
	gSynchConsole->Write(&c, 1); // In ky tu tu bien c, 1 byte
	IncreasePC();
	return;
}

void handleSC_Open()
{
	//OpenFileID Open(char *name, int type)
	// Input: arg1: Dia chi cua chuoi name, arg2: type
	// Output: Tra ve OpenFileID neu thanh công, -1 neu loi
	// Chuc nang: Tra ve ID cua file.
	int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so name tu thanh ghi so 4
	int type = machine->ReadRegister(5); // Lay tham so type tu thanh ghi so 5
	char* filename;
	filename = User2System(virtAddr, MaxFileLength); // Copy chuoi tu vung nho User Space sang System Space voi bo dem name dai MaxFileLength
	//Kiem tra xem OS con mo dc file khong

	int freeSlot = fileSystem->FindFreeSlot();//Tim nhung slot con NULL tren bang mo ta file
	if (freeSlot != -1) //Chi xu li khi con slot trong
	{
		if (type == 0 || type == 1) //chi xu li khi type = 0 hoac 1
		{
			
			if ((fileSystem->openf[freeSlot] = fileSystem->Open(filename, type)) != NULL) //Mo file thanh cong
			{
				machine->WriteRegister(2, freeSlot); //tra ve OpenFileID
			}
		}
		else if (type == 2) // xu li stdin voi type quy uoc la 2
		{
			machine->WriteRegister(2, 0); //tra ve OpenFileID
		}
		else // xu li stdout voi type quy uoc la 3
		{
			machine->WriteRegister(2, 1); //tra ve OpenFileID
		}
		delete[] filename;
		IncreasePC();
		return;
	}
	machine->WriteRegister(2, -1); //Khong mo duoc file return -1
	delete[] filename;
	IncreasePC();
	return;
}

void handleSC_Close()
{
	/*void Close(OpenFileId id)
	*Input: File id
	* Output: 0 thanh cong, -1 that bai*/
	int fileID = machine->ReadRegister(4);
	if(fileID >=0 && fileID <=9)
	{
		if(fileSystem->openf[fileID] != NULL)
		{
			delete fileSystem->openf[fileID];
			fileSystem->openf[fileID] = NULL;
			machine->WriteRegister(2, 0);
			return;	
		}
	}
	machine->WriteRegister(2, -1);
	IncreasePC();
	return;
}

void handleSC_Read()
{
    /* int Read(char *buffer, int size, OpenFileId id);
	* Input: buffer: return buffer
	* Output: -1 if fail; else size
    * Type: 0 - Read + Write
    *       1 - Read Only
    *       2 - stdin
    *       3 - stdout */
	int virtualAddr = machine->ReadRegister(4);
	int size = machine->ReadRegister(5);
	int fileID = machine->ReadRegister(6);
	int prevPos, currPos, realSize;
	char * buf;

	/*fileID nam ngoai ban mo ta file.*/
	if (fileID < 0 || fileID > 9) {
		DEBUG('a', "Fail to read file.\nFileID is out of openf table.\n");
		printf("Fail to read file.\nFileID is out of openf table.\n");
		machine->WriteRegister(2, -1);
		IncreasePC();
		return;
	}
	/* File chua duoc mo.*/
	if (fileSystem->openf[fileID] == NULL) {
		DEBUG('a', "Fail to read file.\nFile has not been opened yet.\n");
		printf("Fail to read file.\nFile has not been opened yet.\n");
		machine->WriteRegister(2, -1);
		IncreasePC();
		return;
	}
	/* Mo file stdout*/
	if (fileSystem->openf[fileID]->type == 3) {
		DEBUG('a', "Fail to read file.\nStdout is not readable.\n");
		printf("Fail to read file.\nStdout is not readable.\n");
		machine->WriteRegister(2, -1);
		IncreasePC();
		return;
	}
	/* Doc file thanh cong*/
	prevPos = fileSystem->openf[fileID]->GetCurrentPos();
	buf = User2System(virtualAddr, size);
	memset(buf, 0, sizeof buf);
	if (fileSystem->openf[fileID]->type == 2) {
		int realSize = gSynchConsole->Read(buf, size); 
		System2User(virtualAddr, realSize, buf);
		machine->WriteRegister(2, realSize);
		delete buf;
		IncreasePC();
		return;
	} else {
		realSize = fileSystem->openf[fileID]->Read(buf, size);
		//currPos = fileSystem->openf[fileID]->GetCurrentPos();
		//realSize = currPos - prevPos;
		System2User(virtualAddr, realSize, buf);
		machine->WriteRegister(2, realSize);
		delete buf;
		IncreasePC();
		return;
	}
}

void handleSC_Write()
{
	//Input: buffer(char*), so ky tu (int), id cua file(OpenFileId)
	//Output: -1: Loi; So byte thuc su: thanh cong; -2: NULL
	//Cong dung: ghi file voi tham so la buffer, so ky tu cho phep va id cua file
	int charCount;
	int id;
	int oldPos;
	int newPos;
	char* buf;
	int virtAddr = machine -> ReadRegister(4); //lay dia chi cua tham so buffer
	charCount = machine -> ReadRegister(5); //lay charCount
	id = machine -> ReadRegister(6); //Lay id cua file

	//kiem tra id cua file truyen vao co nam ngoai bang mo ta hay khong
	if(id < 0 || id > 9)
	{
		DEBUG('a', "Fail to write file.\nFileID is out of openf table.\n");
		printf("Fail to write file.\nFileID is out of openf table.\n");
		machine -> WriteRegister(2, -1);
		IncreasePC();
		return;
	}
	//Kiem tra file co ton tai khong
	if(fileSystem -> openf[id] == NULL)
	{
		DEBUG('a', "Fail to write file.\nFile has not been opened yet.\n");
		printf("Fail to write file.\nFile has not been opened yet.\n");
		machine -> WriteRegister(2, -1);
		IncreasePC();
		return;
	}
	//Xet truong hop ghi file only read (type=1) hoac file stdin (type=2) thi tra ve -1
	if(fileSystem->openf[id]->type==1 || fileSystem->openf[id]->type==2)
	{
		DEBUG('a', "Fail to write file.\nStdin could not be written.\n");
		printf("Fail to write file.\nStdin could not be written.\n");
		machine->WriteRegister(2, -1);
		IncreasePC();
		return;
	}

	oldPos = fileSystem -> openf[id] -> GetCurrentPos(); //kiem tra thanh cong thi lay old possition
	buf = User2System(virtAddr, charCount); //copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai charCount
	//Xet truong hop ghi file read&write (type=0) thi tra ve so byte thuc su
	if(fileSystem->openf[id]->type == 0)
	{
		if((fileSystem->openf[id] -> Write(buf, charCount)) > 0)
		{
			//So byte thuc su = newPos - oldPos;
			newPos = fileSystem -> openf[id] -> GetCurrentPos();
			machine -> WriteRegister(2, newPos - oldPos);
			delete buf;
			IncreasePC();
			return;
		}
	}
	//Xet truong hop con lai ghi file stdout (type=3)
	if(fileSystem->openf[id]->type == 3)
	{
		int i = 0;
		while(i<charCount)
		{
			gSynchConsole -> Write(buf+i, 1); //su dung ham write cua lop Synchcons
			i++;
		}
		gSynchConsole->Write(buf+i, 1);
		machine->WriteRegister(2, i-1);
		delete buf;
		IncreasePC();
		return;
	}
	delete buf;
	IncreasePC();
	return;
}

void handleSC_Seek()
{
	// Input: Vi tri(int), id cua file(OpenFileID)
	// Output: -1: Loi, Vi tri thuc su: Thanh cong
	// Cong dung: Di chuyen con tro den vi tri thich hop trong file voi tham so la vi tri can chuyen va id cua file
	int pos = machine->ReadRegister(4); // Lay vi tri can chuyen con tro den trong file
	int id = machine->ReadRegister(5); // Lay id cua file
	// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong? Theo de: Bang mo ta file gom 10  file
	if (id < 0 || id >10)//Nam ngoai bang mo to file
	{
		DEBUG('a', "Fail to seek file.\nFileID is out of openf table.\n");
		printf("Fail to seek file.\nFileID is out of openf table.\n");
		machine->WriteRegister(2, -1);//gan ket qua khong thanh cong vo thanh ghi r2
		IncreasePC();//Luu d/c cua lenh ke tiep de thuc hien
		return;
	}
	// Kiem tra file co ton tai khong
	if (fileSystem->openf[id] == NULL)//OpenFile** openf;
	{
		DEBUG('a', "Fail to seek file.\nFile has not been opened yet.\n");
		printf("Fail to seek file.\nFile has not been opened yet.\n");
		machine->WriteRegister(2, -1);//gan ket qua khong thanh cong vo thanh ghi r2
		IncreasePC();//Luu d/c cua lenh ke tiep de thuc hien
		return;
	}
	// Kiem tra co goi Seek tren console khong
	//Day la 2 luong file stdin va stout tren bang mo ta file
	if (id == 0 || id == 1)
	{
		DEBUG('a', "Fail to seek file. Console IO could not be seeked.\n");
		printf("Fail to seek file. Console IO could not be seeked.\n");
		machine->WriteRegister(2, -1);//gan ket qua khong thanh cong vo thanh ghi r2
		IncreasePC();
		return;
	}
	// Neu pos = -1 thi gan pos = Length nguoc lai thi giu nguyen pos
	pos = (pos == -1) ? fileSystem->openf[id]->Length() : pos;
	if (pos > fileSystem->openf[id]->Length() || pos < 0) // Kiem tra lai vi tri pos co hop le khong
	{
		DEBUG('a', "Fail to seek to this position.\n");
		printf("Fail to seek to this position.\n");
		machine->WriteRegister(2, -1);//gan ket qua khong thanh cong vo thanh ghi r2
	}
	else
	{
		// Neu hop le thi tra ve vi tri di chuyen thuc su trong file
		fileSystem->openf[id]->Seek(pos);
		machine->WriteRegister(2, pos);//gan ket qua thanh cong la vi tri thuc su trong file vo r2
	}
	IncreasePC();//Luu d/c cua lenh ke tiep de thuc hien
	return;
}