#ifndef STABLE_H
#define STABLE_H

#include "synch.h"
#include "bitmap.h"
#include "sem.h"

#define MAX_LOCK 10

class STable {
private:
	BitMap* bm;
	Sem* lockTab[MAX_LOCK];
public:
	STable();
	~STable();
	int create(char* name, int init);
	int wait(char* name);
	int signal(char* name);
};

#endif
