#ifndef STABLE_H
#define STABLE_H

#include "bitmap.h"
#include "sem.h"

#define MAXSEMAPHORE 10

class STable {
private:
	BitMap* bm;                //Bitmap quan ly vi tri Semaphore
	Sem* semTab[MAXSEMAPHORE]; //Mang cac Sem quan ly Semaphore
	int FindSem(char* name);   //Ham tra ve vi tri mot Semaphore co ten name
public:
	STable();                  //Ham khoi tao STable
	~STable();                 //Ham huy STable
	int Create(char* name, int init); //Ham tao Semaphore voi ten name, gia tri khoi tao init
	int Wait(char* name);      //Ham bao hieu Semaphore name doi
	int Signal(char* name);    //Ham bao hieu Semaphore name duoc tiep tuc
	int FindFreeSlot(int id);  //Ham tim vi tri trong trong bang Semaphore
};
#endif
