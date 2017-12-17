#include "stable.h"

STable::STable() {
	bm = new BitMap(MAXSEMAPHORE);
	for (int i = 0; i<MAXSEMAPHORE; i++)
		semTab[i] = NULL;
}

STable::~STable() {
	for (int i = 0; i<MAXSEMAPHORE; i++)
		if (semTab!=NULL)
			delete semTab[i];
	delete bm;
}

int STable::Create(char* name, int init) {
	int id = FindSem(name);
	if (id > -1) {
		printf("Semaphore already exist\n");
		return -1;	
	}
	int freeSlot = bm->Find();
	if (freeSlot > -1) {
		semTab[freeSlot] = new Sem(name, init);
		return 0;
	}
	printf("No freeslot left for new Semaphore\n");	
	return -1;
}

int STable::Wait(char* name) {
	int id = FindSem(name);
	if (id > -1) {
		semTab[id]->wait();
		return 0;
	} else {
		printf("Semaphore not found\n");
		return -1;
	}
}

int STable::Signal(char* name) {
	int id = FindSem(name);
	if (id > -1) {
		semTab[id]->signal();
		return 0;
	} else {
		printf("Semaphore not found\n");
		return -1;
	}
}

int STable::FindFreeSlot(int id) {
	return bm->Find();
}

int STable::FindSem(char* name) {
	for (int i = 0; i<MAXSEMAPHORE; i++)
		if (bm->Test(i) && strcmp(semTab[i]->GetName(),name) == 0)
				return i;		
	return -1;
}

