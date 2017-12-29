#ifndef PCB_H
#define PCB_H

#include "synch.h"
#include "thread.h"

#define MAX_FILENAME_LEN 255

class PCB {
private:
	Semaphore* joinsem;
	Semaphore* exitsem;
	Semaphore* mutex;
	Thread* thread;
	char fileName[MAX_FILENAME_LEN+1];

	int pID;				// process ID
	int exitcode;
	int numwait;
	int joinID;
	int isExit;
public:
	int parentID;
	int joinExitcode;

	PCB();
	~PCB();

	int Exec(char* filename, int pid, int parentid);
	const Thread* GetThread(); 
	int GetID();
	int GetNumWait();
	void JoinWait(int joinid);
	void ExitWait();
	void JoinRelease(int joinid, int joinexitcode);
	void ExitRelease();
	void IncNumWait();
	void DecNumWait();
	void SetExitCode(int ec);
	int GetExitCode();
	void SetFileName(char* fn);
	const char* GetFileName();
};

#endif
