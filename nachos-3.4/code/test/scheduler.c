#include "syscall.h"

int main()
{
    int pingID, pongID;

    pingID = Exec("./test/ping");
    Join(pingID);
    pongID = Exec("./test/pong");
    
    Join(pongID);  
    return 0;
}
