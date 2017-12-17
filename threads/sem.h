#ifndef SEM_H
#define SEM_H

#include "synch.h"

class Sem {
private:
	char name[50];
	Semaphore *sem;
public:
	Sem(char* na, int i){
		strcpy(this->name,na);
		sem = new Semaphore(name,i);
	}
	~Sem(){
		delete sem;
	}
	void wait(){
		sem->P();
	}
	void signal(){
		sem->V();
	}
	char* GetName(){
		return name;
	} 
};
#endif
