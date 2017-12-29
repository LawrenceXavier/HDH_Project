#ifndef PTABLE_H
#define PTABLE_H

#include "bitmap.h"
#include "pcb.h"
#include "semaphore.h"

#define MAX_PROCESS 10

class PTable {
private:
	BitMap* bm;
	PCB* pcb[MAX_PROCESS];
	int psize;
	Semaphore* bmsem;
public:
	PTable(int size);
	~PTable();

	int ExecUpdate(char* filename);
	int ExitUpdate(int ec);
	int JoinUpdate(int id);
// implemented inside
/*  
	int GetFreeSlot();
	bool IsExist(int pid);
	void Remove(int pid);
	char* GetFileName(int id);
*/
};

#endif
