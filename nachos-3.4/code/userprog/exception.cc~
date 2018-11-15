// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"


#define MaxFileLength 32 // Do dai quy uoc cho file name
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------
// Doi thanh ghi Program counter cua he thong ve sau 4 byte de tiep tuc nap lenh <Tai sao dung ham nay??: HDH dung ham nay de nap lenh ke tiep khi thuc hien chuong trinh>
void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
   	machine->WriteRegister(PrevPCReg, counter);
    	counter = machine->ReadRegister(NextPCReg);
    	machine->WriteRegister(PCReg, counter);
   	machine->WriteRegister(NextPCReg, counter + 4);
}
// Input: Khong gian dia chi User(int) - gioi han cua buffer(int)
// Output: Bo nho dem Buffer(char*)
// Chuc nang: Sao chep vung nho User sang vung nho System
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
// Input: Khong gian vung nho User(int) - gioi han cua buffer(int) - bo nho dem buffer(char*)
// Output: So byte da sao chep(int)
// Chuc nang: Sao chep vung nho System sang vung nho User
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
void ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);

	// Bien toan cuc cho lop SynchConsole

	//Bat dau
	switch (which)
	{
	case NoException:
		return;

	case PageFaultException:
		DEBUG('a', "\n No valid translation found");
		printf("\n\n No valid translation found");
		interrupt->Halt();
		break;

	case ReadOnlyException:
		DEBUG('a', "\n Write attempted to page marked read-only");
		printf("\n\n Write attempted to page marked read-only");
		interrupt->Halt();
		break;

	case BusErrorException:
		DEBUG('a', "\n Translation resulted invalid physical address");
		printf("\n\n Translation resulted invalid physical address");
		interrupt->Halt();
		break;

	case AddressErrorException:
		DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
		printf("\n\n Unaligned reference or one that was beyond the end of the address space");
		interrupt->Halt();
		break;

	case OverflowException:
		DEBUG('a', "\nInteger overflow in add or sub.");
		printf("\n\n Integer overflow in add or sub.");
		interrupt->Halt();
		break;

	case IllegalInstrException:
		DEBUG('a', "\n Unimplemented or reserved instr.");
		printf("\n\n Unimplemented or reserved instr.");
		interrupt->Halt();
		break;

	case NumExceptionTypes:
		DEBUG('a', "\n Number exception types");
		printf("\n\n Number exception types");
		interrupt->Halt();
		break;

	case SyscallException:
		switch (type)
		{

		case SC_Halt:
			// Input: Khong co
			// Output: Thong bao tat may
			// Chuc nang: Tat HDH
			DEBUG('a', "\nShutdown, initiated by user program. ");
			printf("\nShutdown, initiated by user program. ");
			interrupt->Halt();
			return;
		case SC_CreateFile:
		{
			// Input: Dia chi tu vung nho user cua ten file
			// Output: -1 = Loi, 0 = Thanh cong
			// Chuc nang: Tao ra file voi tham so la ten file
			int virtAddr;
			char* filename;
			DEBUG('a', "\n SC_CreateFile call ...");
			DEBUG('a', "\n Reading virtual address of filename");

			virtAddr = machine->ReadRegister(4); //Doc dia chi cua file tu thanh ghi R4
			DEBUG('a', "\n Reading filename.");

			//Sao chep khong gian bo nho User sang System, voi do dang toi da la (32 + 1) bytes
			filename = User2System(virtAddr, MaxFileLength + 1);
			if (strlen(filename) == 0)
			{
				printf("\n File name is not valid");
				DEBUG('a', "\n File name is not valid");
				machine->WriteRegister(2, -1); //Return -1 vao thanh ghi R2
				IncreasePC();
				return;
			}

			if (filename == NULL)  //Neu khong doc duoc
			{
				printf("\n Not enough memory in system");
				DEBUG('a', "\n Not enough memory in system");
				machine->WriteRegister(2, -1); //Return -1 vao thanh ghi R2
				delete filename;
				IncreasePC();
				return;
				//break;
			}
			DEBUG('a', "\n Finish reading filename.");

			if (!fileSystem->Create(filename, 0)) //Tao file bang ham Create cua fileSystem, tra ve ket qua
			{
				//Tao file that bai
				printf("\n Error create file '%s'", filename);
				machine->WriteRegister(2, -1);
				delete filename;
				IncreasePC();
				return;
				//break;
			}

			//Tao file thanh cong
			machine->WriteRegister(2, 0);
			delete filename;
			IncreasePC(); //Day thanh ghi lui ve sau de tiep tuc ghi
			return;
			//break;
		}
		case SC_ReadString:
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
			//break;
		}
		case SC_PrintChar:
		{
			// Input: Ki tu(char)
			// Output: Ki tu(char)
			// Cong dung: Xuat mot ki tu la tham so arg ra man hinh
			char c = (char)machine->ReadRegister(4); // Doc ki tu tu thanh ghi r4
			gSynchConsole->Write(&c, 1); // In ky tu tu bien c, 1 byte
			//IncreasePC();
			break;
		}

		case SC_PrintString:
		{
			//void String(char*buffer)
			// Input: Buffer(char*)
			// Output: Chuoi doc duoc tu buffer(char*)
			// Cong dung: Xuat mot chuoi la tham so buffer truyen vao ra man hinh console
			int virtAddr;
			char* buffer;
			virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
			buffer = User2System(virtAddr, 255); // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai 255 ki tu
			int length = 0;
			while (buffer[length] != 0) length++; // Dem do dai that cua chuoi
			//->SynchConsole* gSynchConsole: Duoc define va contruction trong system.cc & system.h
			//int SynchConsole::Write(char *into, int numBytes)
			//Writes numBytes of into buffer to I/O device 
			//Returns the number of bytes written
			gSynchConsole->Write(buffer, length + 1); // Goi ham Write cua SynchConsole de in chuoi
			delete buffer; 
			IncreasePC(); // Tang Program Counter 
			return;
			//break;
		}

	case SC_Open:
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
				//IncreasePC();
				break;
			}
			machine->WriteRegister(2, -1); //Khong mo duoc file return -1
			
			delete[] filename;
			//IncreasePC();
			break;
		}

		case SC_Close:
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
					break;
				}
			}
			machine->WriteRegister(2, -1);
			IncreasePC();
			return;
			break;
		}
		case SC_Read:
		{
			// Input: buffer(char*), so ky tu(int), id cua file(OpenFileID)
			// Output: -1: Loi, So byte read thuc su: Thanh cong, -2: Thanh cong
			// Cong dung: Doc file voi tham so la buffer, so ky tu cho phep va id cua file
			int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
			int charcount = machine->ReadRegister(5); // Lay charcount tu thanh ghi so 5
			int id = machine->ReadRegister(6); // Lay id cua file tu thanh ghi so 6 
			int OldPos;
			int NewPos;
			char *buf;
			// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
			if (id < 0 || id > 10)
			{
				printf("\nKhong the read vi id nam ngoai bang mo ta file.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Kiem tra file co ton tai khong
			if (fileSystem->openf[id] == NULL)
			{
				printf("\nKhong the read vi file nay khong ton tai.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			if (fileSystem->openf[id]->type == 3) // Xet truong hop doc file stdout (type quy uoc la 3) thi tra ve -1
			{
				printf("\nKhong the read file stdout.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			OldPos = fileSystem->openf[id]->GetCurrentPos(); // Kiem tra thanh cong thi lay vi tri OldPos
			buf = User2System(virtAddr, charcount); // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai charcount
			// Xet truong hop doc file stdin (type quy uoc la 2)
			if (fileSystem->openf[id]->type == 2)
			{
				// Su dung ham Read cua lop SynchConsole de tra ve so byte thuc su doc duoc
				int size = gSynchConsole->Read(buf, charcount); 
				System2User(virtAddr, size, buf); // Copy chuoi tu vung nho System Space sang User Space voi bo dem buffer co do dai la so byte thuc su
				machine->WriteRegister(2, size); // Tra ve so byte thuc su doc duoc
				delete buf;
				IncreasePC();
				return;
			}
			// Xet truong hop doc file binh thuong thi tra ve so byte thuc su
			if ((fileSystem->openf[id]->Read(buf, charcount)) > 0)
			{
				// So byte thuc su = NewPos - OldPos
				NewPos = fileSystem->openf[id]->GetCurrentPos();
				// Copy chuoi tu vung nho System Space sang User Space voi bo dem buffer co do dai la so byte thuc su 
				System2User(virtAddr, NewPos - OldPos, buf); 
				machine->WriteRegister(2, NewPos - OldPos);
			}
			else
			{
				// Truong hop con lai la doc file co noi dung la NULL tra ve -2
				//printf("\nDoc file rong.");
				machine->WriteRegister(2, -2);
			}
			delete buf;
			IncreasePC();
			return;
		}
		case SC_Write:
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
		 if(id<0 || id>9)
		 {
		   printf("\nKhong the ghi file vi id cua file nam ngoai bang mo ta");
		   machine -> WriteRegister(2, -1);
		   IncreasePC();
		   return;
		 }
		 //Kiem tra file co ton tai khong
		 if(fileSystem -> openf[id] == NULL)
		 {
		   printf("File khong ton tai");
		   machine -> WriteRegister(2, -1);
		   IncreasePC();
		   return;
		 }
		 //Xet truong hop ghi file only read (type=1) hoac file stdin (type=2) thi tra ve -1
		 if(fileSystem->openf[id]->type==1 || fileSystem->openf[id]->type==2)
		 {
		   printf("\nKhong the ghi file chi doc hoac stdin");
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
		   while(buf[i] !=0 && buf[i] != '\n')
		   {
			gSynchConsole -> Write(buf+i, 1); //su dung ham write cua lop Synchcons
		   	i++;
		   }
		   buf[i]='\n';
		   gSynchConsole->Write(buf+i, 1);
		   machine->WriteRegister(2, i-1);
		   delete buf;
		   IncreasePC();
		   return;
		 }
		}

		case SC_Seek:
		{
			// Input: Vi tri(int), id cua file(OpenFileID)
			// Output: -1: Loi, Vi tri thuc su: Thanh cong
			// Cong dung: Di chuyen con tro den vi tri thich hop trong file voi tham so la vi tri can chuyen va id cua file
			int pos = machine->ReadRegister(4); // Lay vi tri can chuyen con tro den trong file
			int id = machine->ReadRegister(5); // Lay id cua file
			// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong? Theo de: Bang mo ta file gom 10  file
			if (id < 0 || id >10)//Nam ngoai bang mo to file
			{
				printf("\nKhong the seek vi id nam ngoai bang mo ta file.");
				machine->WriteRegister(2, -1);//gan ket qua khong thanh cong vo thanh ghi r2
				IncreasePC();//Luu d/c cua lenh ke tiep de thuc hien
				return;
			}
			// Kiem tra file co ton tai khong
			if (fileSystem->openf[id] == NULL)//OpenFile** openf;
			{
				printf("\nKhong the seek vi file nay khong ton tai.");
				machine->WriteRegister(2, -1);//gan ket qua khong thanh cong vo thanh ghi r2
				IncreasePC();//Luu d/c cua lenh ke tiep de thuc hien
				return;
			}
			// Kiem tra co goi Seek tren console khong
			//Day la 2 luong file stdin va stout tren bang mo ta file
			if (id == 0 || id == 1)
			{
				printf("\nKhong the seek tren file console.");
				machine->WriteRegister(2, -1);//gan ket qua khong thanh cong vo thanh ghi r2
				IncreasePC();
				return;
			}
			// Neu pos = -1 thi gan pos = Length nguoc lai thi giu nguyen pos
			pos = (pos == -1) ? fileSystem->openf[id]->Length() : pos;
			if (pos > fileSystem->openf[id]->Length() || pos < 0) // Kiem tra lai vi tri pos co hop le khong
			{
				printf("\nKhong the seek file den vi tri nay.");
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
	}
	IncreasePC();
	//.
	}
}

