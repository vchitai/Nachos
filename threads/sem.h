#ifndef SEM_H
#define SEM_H

#include "synch.h"

class Sem {
private:
	char name[50];
	Semaphore *sem;
public:
	//Khoi tao Semaphore co ten na voi gia tri khoi tao i
	Sem(char* na, int i){
		strcpy(this->name,na);
		sem = new Semaphore(name,i);
	}
	//Huy bo Semaphore
	~Sem(){
		delete sem;
	}
	//Yeu cau Semaphore o trang thai doi
	void wait(){
		sem->P();
	}
	//Cho phep Semaphore chay
	void signal(){
		sem->V();
	}
	//Tra ve ten cua Semaphore
	char* GetName(){
		return name;
	} 
};
#endif
