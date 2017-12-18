#include "pcb.h"
#include "filesys.h"
#include "machine.h"

extern Machine* machine;	// user program memory and registers
extern Thread* currentThread;
extern FileSystem  *fileSystem;

void StartProcess_2(int thr) {
	//Khoi tao thanh ghi va khoi phuc trang thai process
	currentThread->space->InitRegisters();
	currentThread->space->RestoreState();
	machine->Run();
	ASSERT(FALSE);	
}

PCB::PCB() {
	//Khoi tao thuoc tinh class
	filename = NULL;
	thread = NULL;
	parrentID = -1;
	processID = -1;
	joinedID = -1;
	numwait = 0;
	joinsem = new Semaphore("Join Semaphore", 0);
	exitsem = new Semaphore("Exit Semaphore", 0);
	mutex = new Semaphore("Mutex Semaphore", 1);
}

PCB::~PCB() {
	//Huy class
	delete joinsem;
	delete exitsem;
	delete mutex;
}

int PCB::Exec(char* name, int pid) {
	mutex->P();
	//Neu processID khong dung
	if (pid < 0) {
		printf("Invalid process ID\n");
		mutex->V();
		return -1;
	}

	//Neu ten file truyen vao khong dung
	if (name == NULL) {
		printf("Exec file name is invalid\n");		
		mutex->V();
		return -1;	
	}

	//Neu PCB dang quan ly mot tien trinh khac
	if (thread != NULL) {
		printf("Another process on the line\n");
		mutex->V();
		return -1;	
	}

	//Neu khong mo duoc file
	OpenFile *executable = fileSystem->Open(name);
    	if (executable == NULL) {
		printf("Unable to open file\n");
		mutex->V();
		return -1;
    	}
	delete executable;

	//Khoi tao addrSpace
	AddrSpace *space = new AddrSpace(name);
	 
	//Neu khong khoi tao duoc space
	if (space == NULL) {
		printf("Not enough space\n");
		mutex->V();
		return -1;
	}	

	//Gan ten file
	SetFileName(name);

	//Tao tien trinh moi va gan thuoc tinh
	thread = new Thread(filename);
	thread->space = space;

	processID = thread->processID = pid;
	parrentID = thread->parrentID = currentThread->processID;
	
	//Neu la tien trinh dau tien thi thay the currentThread bang tien trinh vua khoi tao
	//Neu khong ta dua tien trinh vao danh sach cho thuc thi
	if (parrentID == -1) {
		Thread *tmp = currentThread;
		currentThread = thread;
		currentThread->setStatus(RUNNING);
		delete tmp;
		currentThread->space->InitRegisters();
		currentThread->space->RestoreState();
	} else {
		thread->Fork(StartProcess_2,(int)thread);
	}

	mutex->V();
	return pid;
}

int PCB::GetID() {
	//Tra ve processID cua tien trinh
	return processID;
}

int PCB::GetNumWait() {
	//Tra ve danh sach tien trinh dang doi
	return numwait;
}

void PCB::JoinWait(int id) {
	//Neu mot tien trinh join vao tien trinh
	//Neu co mot tien trinh da join thi thong bao va bao loi
	//Neu chua thi tien hanh join va gan joinedID = id cua tien trinh
	if (joinedID != -1) {
		printf("Another process joined this process\n");
		SetExitCode(-1);
		return;
	}
	joinedID = id;
	joinsem->P();
}

void PCB::ExitWait() {
	//Doi tien trinh cha cho thoat
	exitsem->P();
}

void PCB::JoinRelease(int id) {
	//Giai thoat tien trinh con
	if (joinedID != id) {		
		return;	
	}
	joinedID = -1;
	joinsem->V();
}

void PCB::ExitRelease() {
	//Cho phep tien trinh thoat
	exitsem->V();
}

void PCB::IncNumWait() {
	//Tang so tien trinh dang doi
	numwait++;
}

void PCB::DecNumWait() {
	//Giam so tien trinh dang doi
	numwait--;
}

void PCB::SetExitCode(int ec) {
	//Gan ExitCode
	exitcode = ec;
}

int PCB::GetExitCode() {
	//Lay ExitCode
	return exitcode;
}

void PCB::SetFileName(char* fn) {
	//Copy FileName
	filename = new char[strlen(fn)];
	strcpy(filename, fn);
}

char* PCB::GetFileName() {
	//Tra ve file name
	return filename;
}

