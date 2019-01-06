#include"syscall.h"

int main()
{
    int S1,S2, S3, S4;
    int idP1,idP2;

    S1 = CreateSemaphore("S1",0);
    S2 = CreateSemaphore("S2",0);
    S3 = CreateSemaphore("S3",1);
    S4 = CreateSemaphore("S4",1);
    idP1 = Exec("./test/person1");
    idP2 = Exec("./test/person2");
    Join(idP1);
    Join(idP2);

    return 0;
}