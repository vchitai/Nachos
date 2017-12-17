#include "syscall.h"
int
main()
{	
	int pid;
	Exec("../test/printa");
	pid = Exec("../test/printb");
	
	Join(pid);	
	PrintString("Huh?\n");
	Exit(0);
}
