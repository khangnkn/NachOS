#include "ptable.h"
#include "system.h"

PTable::PTable(int size)
{
	int i = 0;
	psize = size;
	bm = new BitMap(size);
	bmsem = new Semaphore("BMsem",1);
		
	for(i = 0 ; i < MAXPROCESS ; i++)
		pcb[i] = NULL;
	
	/*vi tri cua tien trinh cha trong pcb la 0, nhung tien trinh con se su dung tu 1->9*/
	bm->Mark(0);
	pcb[0] = new PCB(0);
	pcb[0]->parentID = -1;
}

PTable::~PTable()
{
	int i=0;
	if(bm!=NULL)
		delete bm;
	if(bmsem!=NULL)
		delete bmsem;
	for(i=0; i<MAXPROCESS; i++)
		if(pcb[i]!=NULL)
			delete pcb[i];
}

//--------------------------------------------------------------------

int PTable::ExecUpdate(char* filename)
{	
	//doc quyen truy xuat, chi nap 1 tien trinh vao mot thoi diem
	bmsem->P();
	//kiem tra ten file co bang null hay khong
	if(filename == NULL)
	{
		printf("\nFile name is null!\n");
		bmsem->V();
		return -1;
	}
	//Kiem tra file co ton tai tren may khong
	OpenFile *executable = fileSystem->Open(filename);
	if (executable == NULL) 
	{
		printf("\nUnable to open file %s\n", filename);
		bmsem->V();
		return -1;
    }
	delete executable;			// close file
////////////////////////////////////////////////////////////

	//Kiem tra chuong trinh duoc goi co la chinh no khong
	if(strcmp(filename,currentThread->getName())==0)
	{
		printf("\nLoi: khong duoc phep goi chinh no !!!\n");
		bmsem->V();
		return -1;
	}
////////////////////////////////////////////////////////////

	//Kiem tra con slot trong khong
	int ID = GetFreeSlot();
	if(ID == -1)
	{
		printf("\nLoi: Da vuot qua 10 tien trinh !!!\n");
		bmsem->V();
		return -1;
	}
////////////////////////////////////////////////////////////
	pcb[ID]= new PCB(ID);
	pcb[ID] -> parentID = currentThread -> processID;
	int pID = pcb[ID]->Exec(filename,ID);
	bmsem->V();
	return pID;
}

int PTable::ExitUpdate(int exitcode)
{
	int pID = currentThread->processID;
//////////////////////////////////////////////////////////////

	//Neu la main process thi Halt()
	if(pID == 0)
	{
		interrupt->Halt();
		return 0;
	}
/////////////////////////////////////////////////////////////
	//kiem tra id cua tien trinh nay co ton tai hay khong
	if(!IsExist(pID))
	{
		printf("\nLoi: Tien trinh khong ton tai !!!\n");
		return -1;
	}
	pcb[pID]->SetExitCode(exitcode);
	if(pcb[pID]->JoinStatus!=-1) //co tien trinh cha dang cho
	{
		//pcb[0]->DecNumWait(); //giam so tien trinh con ma tien trinh cha phai cho

		pcb[pID]->JoinRelease();//bao hieu cho tien trinh cha biet chuon trinh da thuc hien xong
		pcb[pID]->ExitWait();//dung o day va cho cho den khi tien trinh cha cho phep ket thuc
	}
	Remove(pID);//giai phong vung nho tai vi tri pID trong PCB
	return exitcode;
}

int PTable::JoinUpdate(int pID)
{
	
	if(pID <= 0 || pID > 9)
	{
		printf("\nLoi: Khong ton tai process: id = %d\n",pID);
		return -1;
	}

	if (pcb[pID] == NULL)
	{
		printf("Loi: Khong ton tai process id nay!");
		return -1;
	}

//kiem tra tien trinh dang chay co la cha cua tien trinh can join hay khong
	if(currentThread->processID != pcb[pID]->parentID)
	{
		printf("\nLoi: Khong duoc phep join vao tien trinh khong phai cha cua no !!!\n");
		return -1;
	}
///////////////////////////////////////////////////////////////////////////////////////////
	pcb[pcb[pID]->parentID]->IncNumWait(); //tang so tien trinh con ma tien trinh cha phai cho
	pcb[pID]->JoinWait(); 	//dung o day va doi cho den khi tien trinh con ket thuc

	//sau khi chuong trinh con goi JoinRelease
	int exitCode = pcb[pID]->GetExitCode();

	if(exitCode != 0)
	{
		printf("\nProcess exit with exitcode EC = %d ",exitCode);
		return -1;
	}

	pcb[pID]->ExitRelease();//cho phep tien trinh con ket thuc
	return exitCode;
}

void PTable::Remove(int pID)
{
	if(pID<0 || pID>9)
		return;
	if(bm->Test(pID))
	{
		bm->Clear(pID);
		delete pcb[pID];
	}
}

//----------------------------------------------------------------------------------------------
int PTable::GetFreeSlot()
{
	return bm->Find();
}

bool PTable::IsExist(int pID)
{
	if(pID<0 || pID>9)
		return FALSE;
	return bm->Test(pID);
}

char* PTable::GetName(int pID)
{
	if(pID>=0 && pID<10 && bm->Test(pID))
		return pcb[pID]->GetNameThread();
}
