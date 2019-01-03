#include"Stable.h"

Stable::Stable()
{
    bm = new BitMap(MAX_SEMAPHORE);
    for(int i=0; i<MAX_SEMAPHORE; i++)
        semTab[i] = NULL;
}
Stable::~Stable()
{
    if(bm!=NULL) delete bm;
    for(int i=0; i<MAX_SEMAPHORE; i++)
    {
        delete semTab[i];
    }
}
bool Stable::CheckExist(char *name)
{
    for(int i=0;i<MAX_SEMAPHORE; i++)
    {
        if(semTab[i]!=NULL && strcmp(semTab[i]->getName(), name)==0)
            return TRUE;
    }
    return FALSE;
}
int Stable::FindFreeSlot()
{
    return bm->Find();
}
int Stable::FindSemByName(char* name)
{
    for(int i=0; i<MAX_SEMAPHORE; i++)
    {
        if(semTab[i]!=NULL && strcmp(name, semTab[i]->getName())==0)
            return i;
    }
    return -1;
}
int Stable::Create(char *name, int init)
{
    if(name==NULL)
    {
        printf("\nTen semaphore khong the la gia tri NULL!!!\n");
        return -1;
    }
    if(CheckExist(name))
    {    printf("\nSemaphore %s da ton tai!!!\n", name);
         return -1;
    }
    int freeSlot = FindFreeSlot();
    if(freeSlot==-1)
    {
        printf("\nChi co the quan ly toi da 10 semaphore!!!\n");
        return -1;
    }
    semTab[freeSlot] = new Sem(name, init);
    if(semTab[freeSlot]==NULL)
    {
        printf("\nKhoi tao semaphore khong thanh cong!!!\n");
        bm->Clear(freeSlot);
        return -1;
    }
    return freeSlot;
}
int Stable::Wait(char *name)
{
    if(name==NULL)
    {
        printf("\nTen semaphore khong the nhan gia tri NULL!!!\n");
        return -1;
    }
    int id = FindSemByName(name);
    if(id==-1)
    {
        printf("\nKhong ton tai semaphore %s\n", name);
        return -1;
    }
    semTab[id]->wait();
    return id;
}
int Stable::Signal(char *name)
{
     if(name==NULL)
    {
        printf("\nTen semaphore khong the nhan gia tri NULL!!!\n");
        return -1;
    }
    int id = FindSemByName(name);
    if(id==-1)
    {
        printf("\nKhong ton tai semaphore %s\n", name);
        return -1;
    }
    semTab[id]->signal();
    return id;
}
