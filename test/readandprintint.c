#include "syscall.h"
int main()
{
	int b;
	int i;
	PrintString("Nhap so nguyen: ");
	b = ReadInt();
	PrintString("So nguyen la: ");
	PrintInt(b);
	Halt();
} 
