#include "pcb.h"
#include "system.h"

PCB::PCB() {
	pID = -1;
	parentID = -1;
	exitcode = 0;
	numwait = 0;
	thread = NULL;
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

void PCB::JoinWait() {
	joinsem->P();
}

void PCB::ExitWait() {
	exitsem->P();
}

void PCB::JoinRelease() {
	joinsem->V();
}

void PCB::ExitRelease() {
	exitsem->V();
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
