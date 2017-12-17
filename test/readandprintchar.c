#include "syscall.h"
int
main()
{
	char c[7];
	ReadString(c,7);
	
	PrintString(c);
	Halt();
}
