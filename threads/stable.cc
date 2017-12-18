#include "stable.h"

STable::STable() {
	//Khoi tao thuoc tinh STable
	bm = new BitMap(MAXSEMAPHORE);
	for (int i = 0; i<MAXSEMAPHORE; i++)
		semTab[i] = NULL;
}

STable::~STable() {
	//Huy thuoc tinh STable
	for (int i = 0; i<MAXSEMAPHORE; i++)
		if (semTab!=NULL)
			delete semTab[i];
	delete bm;
}

int STable::Create(char* name, int init) {
	//Tim semaphore, neu tim duoc thong bao da ton tai
	int id = FindSem(name);
	if (id > -1) {
		printf("Semaphore already exist\n");
		return -1;	
	}

	//Tim vi tri trong trong bang Semaphore, neu khong tim duoc thong bao het slot
	//Neu khong thi khoi tao
	int freeSlot = bm->Find();
	if (freeSlot > -1) {
		semTab[freeSlot] = new Sem(name, init);
		return 0;
	}
	printf("No freeslot left for new Semaphore\n");	
	return -1;
}

int STable::Wait(char* name) {
	//Tim semaphore, neu ton tai thi goi ham wait cua class Sem
	//Neu khong xuat thong bao loi
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
	//Tim semaphore, neu ton tai thi goi ham signal cua class Sem
	//Neu khong xuat thong bao loi
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
	//Tim vi tri trong bang ham Find cua class BitMap
	return bm->Find();
}

int STable::FindSem(char* name) {
	//Duyet danh sach va tim semaphore cung ten
	for (int i = 0; i<MAXSEMAPHORE; i++)
		if (bm->Test(i) && strcmp(semTab[i]->GetName(),name) == 0)
				return i;		
	return -1;
}

