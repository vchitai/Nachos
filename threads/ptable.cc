#include "ptable.h"
#include "system.h"

PTable::PTable()
{		
	//Khoi tao thuoc tinh PTable
	bm = new BitMap(MAXPROCESS);
	bmsem = new Semaphore("Bitmap Semaphore", 1);
	for (int i = 0; i<MAXPROCESS; i++)
		pcb[i] = NULL;
}

PTable::~PTable() {
	//Huy thuoc tinh PTable
	for (int i = 0; i<MAXPROCESS; i++)
		if (pcb[i]!=NULL)
			delete pcb[i];
	delete bm;
	delete bmsem;
}

int PTable::ExecUpdate(char* name) {
	//Neu ten tien trinh thuc thi khong hop le  
	if (name == NULL) {
		printf("Exec file name is invalid\n");		
		return -1;	
	}

	//Neu tien trinh dang chay
	char* cname = currentThread->getName();
	if (strcmp(name, cname) == 0) {
		printf("Process %s call itself\n", cname);
		return -1;
	}

	//Neu khong mo duoc file thuc thi
	OpenFile *executable = fileSystem->Open(name);
    	if (executable == NULL) {
		printf("Unable to open file\n");
		return -1;
    	}
	delete executable;

	//Bat dau tim slot trong
	bmsem->P();
	int freeSlot = bm->Find();
	bmsem->V();

	//Neu khong tim duoc slot trong
	if (freeSlot == -1) {
		printf("Not enough space \n");
		return -1;
	} 
	
	//Khoi tao mot doi tuong PCB moi va thuc thi
	pcb[freeSlot] = new PCB();
	int result = pcb[freeSlot]->Exec(name,freeSlot);

	//Neu qua trinh thuc thi gap loi
	if (result == -1) {
		bm->Clear(freeSlot);
		delete pcb[freeSlot];
		return -1;	
	}
	return result;
}

int PTable::ExitUpdate(int ec) {
	//Lay processID va parentID cua tien trinh hien tai
	int prid = currentThread->processID;
	int paid = currentThread->parrentID;
	
	//Neu tien trinh bi loi
	if (prid == -1)
		return -1;

	//Neu la tien trinh dau tien thi tat may
	//Neu la tien trinh sau do thic dat exit code, xin tien trinh cha giai thoat va cho tien trinh cha cho phep thoat
	//va tien hanh huy tien trinh
	if (paid == -1) {
		interrupt->Halt();
	}
	else {
		pcb[prid]->SetExitCode(ec);
		pcb[paid]->DecNumWait();
		pcb[paid]->JoinRelease(prid);
		pcb[prid]->ExitWait();

		delete currentThread->space;
		bm->Clear(prid);
		delete pcb[prid];
		pcb[prid] = NULL;

		currentThread->Finish();
	}
	return 0;
}

int PTable::JoinUpdate(int id) {
	//Lay processID cua tien trinh hien tai	
	int cId = currentThread->processID;

	//Neu tien trinh dang join = tien trinh hien tai
	if (id == cId) {
		printf("Cannot join itself\n");
		return -1;	
	}	

	//Neu tien trinh dang join khong ton tai
	if (!bm->Test(id)) {
		printf("Process to be joined not found\n");
		return -1;	
	}

	//Neu tien trinh dang join va tien trinh hien tai khong quan he cha con
	if (pcb[id]->parrentID != cId) {
		printf("Cannot join process other than parent\n");
		return -1;		
	}

	//Bat dau join
	//Tang so luong tien trinh dang doi cua tien trinh cha, xin phep cha cho phep join, nhan lai loi neu co, va thoat
	printf("%s joining %s\n", pcb[id]->GetFileName(), currentThread->getName());
	pcb[cId]->IncNumWait();
	pcb[cId]->JoinWait(id);
	int ec = pcb[id]->GetExitCode();
	pcb[id]->ExitRelease();
	return ec;
}
