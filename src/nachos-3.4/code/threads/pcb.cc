#include "pcb.h"

PCB::PCB(int id) {

}

PCB::~PCB() {

}

int PCB::Exec(char* filename, int pid) {

}

int PCB::GetID() {
	return pid;
}

int PCB::GetNumWait() {

}

void PCB::JoinWait() {

}

void PCB::ExitWait() {

}

void PCB::JoinRelease() {

}

void PCB::ExitRelease() {

}

void PCB::IncNumWait() {

}

void PCB::DecNumWait() {

}

void PCB::SetExitCode(int ec) {
	exitcode = ec;
}

int PCB::GetExitCode() {
	return exitcode;
}

void PCB::SetFileName(char* fn) {

}

char* PCB::GetFileName() {

}
