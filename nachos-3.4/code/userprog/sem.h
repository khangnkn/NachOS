#include "synch.h"
class Sem
{
    private:
        char name[50];
        Semaphore *sem; //Tao semaphore de quan ly
    public:
    //Khoi tao doi tuong Sem. Gan gia tri ban dau la null
    //nho khoi tao bm su dung
    Sem(char* semName, int i)
    {
        strcpy(name, semName);
        sem = new Semaphore(name, i);
    }
    ~Sem()
    {
        delete sem;
    }
    void wait() //thao tac cho
    {
        sem->P();
    }
    void signal() //thao tac giai phong semaphore
    {
        sem->V();
    }
    char* getName() //tra ve ten semaphore
    {
        return name;
    }
};