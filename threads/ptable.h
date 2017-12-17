#ifndef PTABLE_H
#define PTABLE_H

#include "bitmap.h"
#include "pcb.h"
#include "synch.h"

#define MAXPROCESS 10

class PTable {
private:
	BitMap*	bm;
	PCB* 	pcb[MAXPROCESS];
	Semaphore* bmsem;
public:
	PTable();
	~PTable();
	int ExecUpdate(char* name);
	int ExitUpdate(int ec);
	int JoinUpdate(int id);
};
#endif
