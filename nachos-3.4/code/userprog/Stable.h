#include "sem.h"
#include "bitmap.h"

#define MAX_SEMAPHORE 10
class Stable
{
    private:
        BitMap *bm; //quan ly cac slot trong
        Sem* semTab[MAX_SEMAPHORE]; //quan ly toi da 10 doi tuong semaphore
    public:
        Stable();
        ~Stable();
        int FindFreeSlot(); //tim slot trong trong bitmap bm
        int FindSemByName(char *name); //tim semaphore theo ten
        bool CheckExist(char *name);//kiem tra Sem "name" co ton tai trong mang Sem chua
        int Create(char* name, int init); //kiem tra doi tuong semaphore "name" chua ton tai thi tao Sem moi, nguoc lai thi bao loi
        int Wait(char* name); //Neu ton tai Sem "name" thi goi this->P() de thuc thi, nguoc lai thi bao loi
        int Signal(char* name); //Neu ton tai Sem "name" thi goi this->V() de thuc thi, nguoc lai thi bao loi
};