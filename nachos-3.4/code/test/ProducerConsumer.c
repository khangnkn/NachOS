#include "syscall.h"

#define N 10  


	int buffer[N];                   // khoi tao buffer co do dai N phan tu
	int in = 0;                     // vi tri chen phan tu vao
	int out = 0;                    // vi tri lay phan tu ra

void producer()
{
		Down("full");// kiem tra buffer co full hay khong, full thi se dung o day cho
		Down("multex");// doc quyen truy xuat
		ConsolePrint("Producer: insert item at\n");
		insert_item(1); //chen 1 vao buffer
		Up("multex");// giai phong doc quyen truy xuat
		Up("empty");//bao hieu cho comsumer biet la buffer hien dang co phan tu
}
void consumer()
{
		Down("empty"); //kiem tra buffer co rong hay khong, rong se dung o day cho
		Down("multex");//doc quyen truy xuat
		ConsolePrint("Consumer: remove item at\n");
		remove_item();
		Up("multex");//giai phong doc quyen truy xuat
		Up("full");//bao hieu cho producer biet la buffer hien khong full
}
void insert_item(int item)
{
	buffer[in] = item;
	in = (in + 1) % N;//tang item len: nhu i++
}
int remove_item()
{
	int item = buffer[out];                     // Get the item
	out = (out + 1) % N;
	return item;
}
int main()
{
	int multex = CreateSemaphore("multex", 1);
	int full = CreateSemaphore("full", N);
	int empty = CreateSemaphore("empty", 0);
	

	producer();
	consumer();
	return 0;
}
