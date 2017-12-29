#include "pcb.h"
#include "system.h"

PCB::PCB() {
	pID = -1;
	parentID = -1;
	exitcode = 0;
	numwait = 0;
	thread = NULL;
	joinID = -1;
	joinExitcode = 0;
	isExit = 0;
	joinsem = new Semaphore("joinsem", 0);
	exitsem = new Semaphore("exitsem", 0);
	mutex = new Semaphore("multex", 1);
	memset(fileName, 0, MAX_FILENAME_LEN+1);
}

PCB::~PCB() {
	delete joinsem;
	delete exitsem;
	delete mutex;
}

void exec_func(int) {
	currentThread->space->InitRegisters();
	currentThread->space->RestoreState();
	machine->Run();
}

int PCB::Exec(char* filename, int pid, int parentid) {
	mutex->P();

	if (filename == NULL || pID < 0 || thread != NULL) {
		mutex->V();
		return -1;
	}

	OpenFile* executable = fileSystem->Open(filename);
	if (executable == NULL) {
		mutex->V();
		return -1;
	}
	
	pID = pid;
	parentID = parentid;

	strncpy(fileName, filename, MAX_FILENAME_LEN);
	
	thread = new Thread(fileName);
	thread->space = new AddrSpace(executable);
	delete executable;

	if (parentID == -1) {
		Thread* tempthread = currentThread;
		currentThread = thread;
		currentThread->setStatus(RUNNING);
		delete tempthread;
		thread->space->InitRegisters();
		thread->space->RestoreState();
	}
	else {
		thread->Fork(exec_func, 0);
	}	

	mutex->V();
	return pID;
}

int PCB::GetID() {
	return pID;
}

int PCB::GetNumWait() {
	return numwait;
}

void PCB::JoinWait(int joinid) {
	if (joinID != -1) return;
	joinID = joinid;
	joinsem->P();
}

void PCB::ExitWait() {
	if (numwait > 0) {
		isExit = 1;
		exitsem->P();
	}
}

void PCB::JoinRelease(int joinid, int joinexitcode) {
	if (joinID != joinid) return;
	joinID = -1;
	joinExitcode = joinexitcode;
	joinsem->V();
}

void PCB::ExitRelease() {
	if (numwait == 0 && isExit == 1) {
		exitsem->V();
	}
}

void PCB::IncNumWait() {
	++numwait;
}

void PCB::DecNumWait() {
	--numwait;
}

void PCB::SetExitCode(int ec) {
	exitcode = ec;
}

int PCB::GetExitCode() {
	return exitcode;
}

void PCB::SetFileName(char* fn) {
	strncpy(fileName, fn, MAX_FILENAME_LEN);
}

const char* PCB::GetFileName() {
	return fileName;
}

const Thread* PCB::GetThread() {
	return thread;
}
