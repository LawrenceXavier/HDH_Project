#include "system.h"
#include "ptable.h"

PTable::PTable() {
	bm = new BitMap(MAX_PROCESS);
	bmsem = new Semaphore("bmsem", 1);
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
	DEBUG('u', "Start to excute a threads\n");
	bmsem->P();

	if (filename == NULL) {
		bmsem->V();
		return -1;
	}

	OpenFile* executable = fileSystem->Open(filename);	
	if (executable == NULL) {
		DEBUG('u', "Unable to open executable file\n");
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
			DEBUG('u', "Fail at strncmp\n");
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
			DEBUG('u', "Fail at parentid\n");
			bmsem->V();
			return -1;
		}	

		pid = bm->Find();
		if (pid == -1) {
			DEBUG('u', "Fail at pid\n");
			bmsem->V();
			return -1;
		}
	}

	pcb[pid] = new PCB();
	int ret = pcb[pid]->Exec(filename, pid, parentid);
	if (ret == -1) {
		DEBUG('u', "Fail at Exec\n");
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
	int pid = -1;
	for (int i = 0; i < MAX_PROCESS; ++i) {
		if (pcb[i]) {
			if (pcb[i]->GetThread() == currentThread) {
				pid = i;
				break;
			}
		}
	}	

	if (pid == -1) return -1;
	int parentid = pcb[pid]->parentID;
	pcb[pid]->ExitWait();

	if (parentid == -1) {
		interrupt->Halt();
		return 0;
	}
	else {
		pcb[parentid]->JoinRelease(pid, ec);
		pcb[parentid]->DecNumWait();
		pcb[parentid]->ExitRelease();
		delete pcb[pid];
		pcb[pid] = NULL;
		bm->Clear(pid);
		delete currentThread->space;
		currentThread->Finish();
		return 0;
	}
}

int PTable::JoinUpdate(int id) {
	if (id < 0 || id >= MAX_PROCESS) return -1;
	if (bm->Test(id) == 0) return -1;
	int pid = -1;
	for (int i = 0; i < MAX_PROCESS; ++i) {
		if (pcb[i] == NULL) continue;
		if (pcb[i]->GetThread() == currentThread) {
			pid = i;
			break;
		}
	}

	if (pid == -1) return -1;
	if (pcb[id]->parentID != pid) return -1;
	
	pcb[pid]->JoinWait(id);
	int ec = pcb[pid]->joinExitcode;
	return ec;
}
