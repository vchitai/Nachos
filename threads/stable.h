#ifndef STABLE_H
#define STABLE_H

#include "bitmap.h"
#include "sem.h"

#define MAXSEMAPHORE 10

class STable {
private:
	BitMap* bm;
	Sem* semTab[MAXSEMAPHORE];
	int FindSem(char* name);
public:
	STable();
	~STable();
	int Create(char* name, int init);
	int Wait(char* name);
	int Signal(char* name);
	int FindFreeSlot(int id);
};
#endif
