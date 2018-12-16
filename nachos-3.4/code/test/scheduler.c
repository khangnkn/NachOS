#include "syscall.h"

int main()
{
    int pingID, pongID;

    pingID = Exec("./test/ping");
    pongID = Exec("./test/pong");
    Join(pingID);
    Join(pongID);  
    return 0;
}
