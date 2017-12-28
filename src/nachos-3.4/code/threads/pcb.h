#ifndef PCB_H
#define PCB_H

#include "synch.h"
#include "thread.h"

class PCB {
private:
	Semaphore* joinsem;
	Semaphore* exitsem;
	Semaphore* multex;
	Thread* thread;

	int pid;
	int exitcode;
	int numwait;
public:
	int parentID;

	PCB(int id);
	~PCB();

	int Exec(char* filename, int pid);
	int GetID();
	int GetNumWait();
	void JoinWait();
	void ExitWait();
	void JoinRelease();
	void ExitRelease();
	void IncNumWait();
	void DecNumWait();
	void SetExitCode(int ec);
	int GetExitCode();
	void SetFileName(char* fn);
	char* GetFileName();
};

#endif
