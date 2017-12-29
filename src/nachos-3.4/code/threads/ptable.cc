#include "system.h"
#include "ptable.h"

PTable::PTable(int size) {
	bm = new BitMap(MAX_PROCESS);
	bmsem = new Semaphore("bmsem", 1);
	psize = size;
	for (int i = 0; i < MAX_PROCESS; ++i) {
		pcb[i] = NULL;
	}
}

PTable::~PTable() {
	delete bm;
	delete bmsem;
	for (int i = 0; i < MAX_PROCESS; ++i)
		if (pcb[i])
			delete pcb[i];
}

int PTable::ExecUpdate(char* filename) {
	bmsem->P();

	if (filename == NULL) {
		bmsem->V();
		return -1;
	}

	OpenFile* executable = fileSystem->Open(filename);	
	if (executable == NULL) {
		bmsem->V();
		return -1;
	}
	delete executable;

	int parentid = -1;
	int pid;
	
	if (bm->NumClear() == MAX_PROCESS) {
		pid = 0;
		bm->Mark(pid);
	}
	else {
		if (strncmp(currentThread->getName(), filename, MAX_FILENAME_LEN) == 0) {
			bmsem->V();
			return -1;
		}

		for (int i = 0; i < MAX_PROCESS; ++i) 
			if (pcb[i]) 
				if (pcb[i]->GetThread() == currentThread) {
					parentid = i;
					break;
				}

		if (parentid == -1) {
			bmsem->V();
			return -1;
		}	

		pid = bm->Find();
		if (pid == -1) {
			bmsem->V();
			return -1;
		}
	}

	pcb[pid] = new PCB();
	int ret = pcb[pid]->Exec(filename, pid, parentid);
	if (ret == -1) {
		bm->Clear(pid);
		delete pcb[pid];
		bmsem->V();
		return -1;
	}
	pcb[parentid]->IncNumWait();
	bmsem->V();
	return pid;
}

int PTable::ExitUpdate(int ec) {
}

int PTable::JoinUpdate(int id) {

}

int PTable::GetFreeSlot() {

}

bool PTable::IsExist(int pid) {

}

void PTable::Remove(int pid) {

}

char* PTable::GetFileName(int id) {

}
