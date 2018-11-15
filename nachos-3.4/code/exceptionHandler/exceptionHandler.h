#include "system.h"
#include "syscall.h"

#define MaxFileLength 32

void IncreasePC();

// Input: Khong gian dia chi User(int) - gioi han cua buffer(int)
// Output: Bo nho dem Buffer(char*)
// Chuc nang: Sao chep vung nho User sang vung nho System
char* User2System(int virtAddr, int limit);
// Input: Khong gian vung nho User(int) - gioi han cua buffer(int) - bo nho dem buffer(char*)
// Output: So byte da sao chep(int)
// Chuc nang: Sao chep vung nho System sang vung nho User
int System2User(int virtAddr, int len, char* buffer);

// Handler for SC_Halt
void handleSC_Halt();

// Handler for SC_Create
void handleSC_Create();

// Handler for SC_ConsoleRead
void handleSC_ConsoleRead();

// Handler for SC_ConsolePrint
void handleSC_ConsolePrint();

// Handler for SC_PrintChar
void handleSC_PrintChar();

// Handler for SC_Open
void handleSC_Open();

// Handler for SC_Close
void handleSC_Close();

// Handler for SC_Read
void handleSC_Read();

// Handler for SC_Write
void handleSC_Write();

// Hander for SC_Seek
void handleSC_Seek();