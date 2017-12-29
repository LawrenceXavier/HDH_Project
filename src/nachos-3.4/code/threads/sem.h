#ifndef SEM_H
#define SEM_H

#include "synch.h"

#define MAX_SEMNAME_LEN 255

class Sem {
private:
	char name[MAX_SEMNAME_LEN];
	Semaphore* sem;
public:
	bool isExist;

	Sem(char* semname, int i);
	~Sem();

	void wait();
	void signal();
	const char* getName();
};

#endif
