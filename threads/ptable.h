#ifndef PTABLE_H
#define PTABLE_H

#include "bitmap.h"
#include "pcb.h"
#include "synch.h"

#define MAXPROCESS 10

class PTable {
private:
	BitMap*	bm;              //Bitmap quan ly ID tien trinh
	PCB* 	pcb[MAXPROCESS]; //Mang cac PCB quan ly tien trinh
	Semaphore* bmsem;        //Semaphore quan ly bitmap
public:
	PTable();                //Ham khoi tao PTable
	~PTable();               //Ham huy PTable
	int ExecUpdate(char* name); //Ham thuc thi tien trinh name
	int ExitUpdate(int ec);  //Ham goi thoat tien trinh hien tai voi ma bao loi ec
	int JoinUpdate(int id);  //Ham xin phep cho tien trinh id join vao tien trinh hien tai
};
#endif
