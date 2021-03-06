#include "sem.h"
#include "system.h"


Sem::Sem(char* semname, int i) {
	isExist = false;
	sem = NULL;

	if (name == NULL || i < 0) 
		return;
	
	isExist = true;
	memset(name, 0, MAX_SEMNAME_LEN+1);
	strncpy(name, semname, MAX_SEMNAME_LEN);
	sem = new Semaphore(name, i);
}

Sem::~Sem() {
	if (sem)
		delete sem;
}

void Sem::wait() {
	sem->P();
}

void Sem::signal() {
	sem->V();
}

const char* Sem::getName() {
	return name;
}
