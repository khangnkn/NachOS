#include "pcb.h"
#include "utility.h"
#include "system.h"
#include "thread.h"
#include "addrspace.h"

PCB::PCB(int id)
{
	joinsem= new Semaphore("JoinSem",0);
	exitsem= new Semaphore("ExitSem",0);
	mutex= new Semaphore("Mutex",1);
	pid= id;
	exitcode=0;
	numwait= 0;
	thread= NULL;
}

PCB::~PCB()
{
	if(joinsem != NULL)
		delete joinsem;
	if(exitsem != NULL)
		delete exitsem;
	if(mutex != NULL)
		delete mutex;
	if(thread!=NULL)
	{
		thread->Finish();
	}
}

//------------------------------------------------------------------
int PCB::GetID()
{
	return pid;
}

int PCB::GetNumWait()
{
	return numwait;
}

int PCB::GetExitCode()
{
	return exitcode;	
}

void PCB::SetExitCode(int ec)
{
	exitcode = ec;
}

void PCB::IncNumWait()
{
	//phai doc quyen truy xuat bien numwait dung chung
	mutex->P();
	numwait++;
	mutex->V();
}

void PCB::DecNumWait()
{
	//phai doc quyen truy xuat bien numwait dung chung
	mutex->P();
	if(numwait>0)
		numwait--;
	mutex->V();
}

char* PCB::GetNameThread()
{
	return this->thread->getName();
}

//-------------------------------------------------------------------
void PCB::JoinWait()
{
	joinsem->P();
}

void PCB::JoinRelease()
{
	joinsem->V();
}

void PCB::ExitWait()
{
	exitsem->P();
}

void PCB::ExitRelease()
{
	exitsem->V();
}

//------------------------------------------------------------------
int PCB::Exec(char *filename, int id)
{
	mutex->P(); // tranh 2 tien trinh nap cung luc
	this->thread = new Thread(filename);
	if(this->thread == NULL)
	{
		printf("\nLoi: Khong tao duoc tien trinh moi !!!\n");
		mutex->V(); //neu dinh loi thi tang semaphore len de tien trinh khac thuc hien
		return -1;
	}
	
	this->thread->processID = id; //gan id cho luong nay
	
	// this->parentID = currentThread->processID; //tien trinh cha cua luong chinh la tien trinh goi thuc thi Exec
	
	this->thread->Fork(MyStartProcess,id);//khoi chay luong
	mutex->V(); //tang semaphore de luong khac thuc hien
	return id; //tra ve id cua luong trong mang PCB o lop pTable
}


//*************************************************************************************
void MyStartProcess(int id) //truyen vao vi tri cua PCB trong mang pcb o lop pTab
{
	char *filename = pTab->GetName(id);//lay ten cua file duoc goi thuc thi o vi tri pID trong mang PCB cua lop pTable
	OpenFile * executable = fileSystem->Open(filename);//mo file
	if(executable==NULL)
	{
		printf("\nLoi: Khong the mo file %s o ham MyStartProcess trong Fork voi id = %d!!!\n", filename, id);
		return;
	}
	//printf("\nMo thanh cong file %s o ham MyStartProcess trong Fork voi id = %d!!!\n", filename, id);

	AddrSpace *space= new AddrSpace(executable); //khoi tao 1 vung nho
	if(space == NULL)
	{
		printf("\nLoi: Khong du bo nho de cap phat cho tien trinh !!!\n");
		return; 
	}

	currentThread->space = space;
	space->InitRegisters();		// set the initial register values
	space->RestoreState();		// load page table register

	machine->Run();			// jump to the user progam
	ASSERT(FALSE);			// machine->Run never returns;
						// the address space exits
						// by doing the syscall "exit"
}