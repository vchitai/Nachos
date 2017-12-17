#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

class PCB {
private:
	Semaphore* joinsem;
	Semaphore* exitsem;
	Semaphore* mutex;
	Thread* thread;
	char* filename;
	int exitcode;
	int numwait;
	int processID;
	int joinedID;
public:
	int parrentID;
	PCB();
	~PCB();
	int Exec(char *filename, int pid);
	int Exec(char *filename, int processID, int parentID);
	int GetID();
	int GetNumWait();
	void JoinWait(int id);
	void ExitWait();
	void JoinRelease(int id);
	void ExitRelease();
	void IncNumWait();
	void DecNumWait();
	void SetExitCode(int ec);
	int GetExitCode();
	void SetFileName(char* fn);
	char* GetFileName();
	Thread* GetThread() { return thread;}
};
#endif
