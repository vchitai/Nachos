#include "syscall.h"
int
main()
{	
	int i;
	for (i = 0; i<100; i++)
		PrintChar('b');
	Exit(0);
}
