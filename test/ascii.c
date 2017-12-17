#include "syscall.h"
int
main()
{	
	int i;
	for (i = 0; i<256; i++)
		PrintChar((char)i);
	Halt();
}
