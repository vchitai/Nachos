#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

class PCB {
private:
	Semaphore* joinsem; //Semaphore de join
	Semaphore* exitsem; //Semaphore de exit
	Semaphore* mutex;   //Semaphore de thuc thi
	Thread* thread;     //Thread duoc khoi tao de chay
	char* filename;     //Ten tap tin
	int exitcode;       //Ma bao loi
	int numwait;        //So tien trinh dang doi
	int processID;      //ID cua tien trinh hien tai
	int joinedID;       //ID cua tien trinh dang join vao
public:
	int parrentID;      //ID cua tien trinh cha cua tien trinh hien tai
	PCB();              //Ham khoi tao
	~PCB();             //Ham huy
	int Exec(char *filename, int pid); //Ham thuc thi file o voi id la pid
	int GetID();        //Tra ve ID cua tien trinh hien tai
	int GetNumWait();   //Tra ve so tien trinh dang doi cua tien trinh hien tai
	void JoinWait(int id); //Doi tien trinh id cho phep join
	void ExitWait();    //Doi tien trinh cha cho phep exit
	void JoinRelease(int id); //Doi tien trinh cha giai thoat cho tien trinh id
	void ExitRelease(); //Cho phep tien trinh Exit
	void IncNumWait();  //Tang so tien trinh dang doi
	void DecNumWait();  //Giam so tien trinh dang doi
	void SetExitCode(int ec); //Dat ma bao loi la ec
	int GetExitCode();  //Tra ve ma bao loi
	void SetFileName(char* fn); //Dat ten tap tin la fn
	char* GetFileName(); //Tra ve ten tap tin dang thuc thi
	Thread* GetThread() { return thread;} //Tra ve thread dang thuc thi
};
#endif
