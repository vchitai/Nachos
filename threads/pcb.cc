#include "pcb.h"
#include "filesys.h"
#include "machine.h"

extern Machine* machine;	// user program memory and registers
extern Thread* currentThread;
extern FileSystem  *fileSystem;

void StartProcess_2(int thr) {
	currentThread->space->InitRegisters();
	currentThread->space->RestoreState();
	machine->Run();
	ASSERT(FALSE);	
}

PCB::PCB() {
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
	delete joinsem;
	delete exitsem;
	delete mutex;
}

int PCB::Exec(char* name, int pid) {
	mutex->P();
	if (pid < 0) {
		printf("Invalid process ID\n");
		mutex->V();
		return -1;
	}

	if (name == NULL) {
		printf("Exec file name is invalid\n");		
		mutex->V();
		return -1;	
	}

	if (thread != NULL) {
		printf("Another process on the line\n");
		mutex->V();
		return -1;	
	}

	OpenFile *executable = fileSystem->Open(name);
    	if (executable == NULL) {
		printf("Unable to open file\n");
		mutex->V();
		return -1;
    	}
	AddrSpace *space = new AddrSpace(name);
	delete executable;
	
	SetFileName(name);
	 
	if (space == NULL) {
		printf("Not enough space\n");
		mutex->V();
		return -1;
	}	

	thread = new Thread(filename);
	thread->space = space;

	processID = thread->processID = pid;
	parrentID = thread->parrentID = currentThread->processID;
	
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
	return processID;
}

int PCB::GetNumWait() {
	return numwait;
}

void PCB::JoinWait(int id) {
	if (joinedID != -1) {
		printf("Another process joined this process\n");
		SetExitCode(-1);
		return;
	}
	joinedID = id;
	joinsem->P();
}

void PCB::ExitWait() {
	exitsem->P();
}

void PCB::JoinRelease(int id) {
	if (joinedID != id) {		
		return;	
	}
	joinedID = -1;
	joinsem->V();
}

void PCB::ExitRelease() {
	exitsem->V();
}

void PCB::IncNumWait() {
	numwait++;
}

void PCB::DecNumWait() {
	numwait--;
}

void PCB::SetExitCode(int ec) {
	exitcode = ec;
}

int PCB::GetExitCode() {
	return exitcode;
}

void PCB::SetFileName(char* fn) {
	filename = new char[strlen(fn)];
	strcpy(filename, fn);
}

char* PCB::GetFileName() {
	return filename;
}

