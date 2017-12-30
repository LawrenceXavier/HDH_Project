// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

#define BLOCK_SIZE	32768
#define MAX_BLOCK	32
#define MAX_FILENAME_LEN 255

#define ABS(x)		((x) < 0 ? -(x) : (x))

/* void PrintChar(char c); */
void
SyscallPrintChar()
{
	char character;
	character = (char)machine->ReadRegister(4); 
				// Read input character from register r4
	gSynchConsole->Write(&character, 1);
				// Print to SynchConsole
	machine->WriteRegister(2, 0);
	machine->AdjustPCRegs();
}

void SyscallReadChar() {	// char ReadChar();
	char character;
	gSynchConsole->Read(&character, 1);	// Read a character from SynchConsole
	machine->WriteRegister(2, character); // return character
	machine->AdjustPCRegs();
}

void SyscallPrintString() {	// void PrintString(char[] buffer);
	// We will try to sequently print a block of BLOCK_SIZE charaters from
	//	buffer to SynchConsole until we encounter a terminate character
	// The maximum block can be printed is MAX_BLOCK, so the total number
	//	of character that can be printed is MAX_BLOCK * BLOCK_SIZE
	int strAddr = machine->ReadRegister(4);
	int i;

	for (i = 0; i < MAX_BLOCK; ++i) {
		char *kernelBuff = machine->User2System(strAddr + i * BLOCK_SIZE, BLOCK_SIZE);
					// Transfer BLOCK_SIZE characters from
					//	User Space to Kernel Space
		int cnt = 0;
		while (cnt < BLOCK_SIZE && kernelBuff[cnt] != '\0')
			++cnt;		// Count number of characters will be
					//	printed
		gSynchConsole->Write(kernelBuff, cnt);
					// Print to SynchConsole
		if (cnt < BLOCK_SIZE)	// If we encouter a terminate character
			break;		//	we exit
		delete[]kernelBuff;
	}

	machine->WriteRegister(2, 0);
	machine->AdjustPCRegs();
}

void SyscallReadString() { 	// void ReadString(char[] buffer, int length);
	int strAddr = machine->ReadRegister(4);
	int len = machine->ReadRegister(5);

	char* kernelBuff = new char[len + 1];   // reserve a position for \0
	memset(kernelBuff, 0, len);

	int realLen = gSynchConsole->Read(kernelBuff, len);	// realLen <= len
	ASSERT(realLen <= len);
	kernelBuff[realLen++] = '\0';		// SynchConsole does not add
						// NULL terminate character,
						// so we have to handle

	machine->System2User(strAddr, realLen, kernelBuff);
						// Move read string into User
						// Space

	delete[] kernelBuff;
	machine->WriteRegister(2, 0);
	machine->AdjustPCRegs();
}

/* void PrintInt(int number);*/
void
SyscallPrintInt()
{
	int number, tmp_number, len, i, j;
	char tmp;
	number = machine->ReadRegister(4); // Read integer from register r4
	tmp_number = number;
	len = 0;
	char *str = new char[13];
	do {
		str[len++] = (char)(ABS(tmp_number % 10) + '0');
					// Get the last digit and append to array
		tmp_number /= 10;	// Divide number by 10 to bring the next digit
					//	become last digit
	} while (tmp_number);
	if (number < 0)
		str[len++] = '-'; // If number < 0 then it should have '-' at begin
	// The str is storing number in reverse order
	// Example: number = 1234 -> str = "4321" or number = "-567" -> str = "765-"
	// Therefore, we have to reverse str in order to get the true display
	for (i = 0; i < len / 2; ++i) {
		j = len - i - 1;
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
	}
	gSynchConsole->Write(str, len); // Print str to SynchConsole
	machine->AdjustPCRegs();
	delete[]str;
}

/* int ReadInt(); */
void
SyscallReadInt()
{
	char c;
	int count, res;
				// First, we have to skip all seperating characters
				//	include ' ', '\t', '\n'
	do {
		count = gSynchConsole->Read(&c, 1);
		if (count == -1)
			break;
	} while (c == ' ' || c == '\t' || c == '\n' || count == 0);

	if (count == -1 || (!(c == '-' || (c >= '0' && c <= '9')))) {
				// Reach EOF or pointer is at non-number
		machine->WriteRegister(2, 0);
		machine->AdjustPCRegs();
		return;
	}

	if (c == '-') {
				// Negative integer
		res = 0;
		count = gSynchConsole->Read(&c, 1);
		while (c >= '0' && c <= '9' && count == 1) {
			if (res <= 0)
				res = res * 10 - (int)(c - '0');
			count = gSynchConsole->Read(&c, 1);
		}

		if (res > 0)	// Integer overflow
			res = 0;
	} else {
				// Positive integer
		res = 0;
		while (c >= '0' && c <= '9' && count == 1) {
			if (res >= 0)
				res = res * 10 + (int)(c - '0');
			count = gSynchConsole->Read(&c, 1);
		}

		if (res < 0)	// Integer overflow
			res = 0;
	}

	machine->WriteRegister(2, res);
	machine->AdjustPCRegs();
}

void
SyscallCreateFile()
{
	int strAddr = machine->ReadRegister(4);
	char *kernelBuf = machine->User2System(strAddr, MAX_FILENAME_LEN + 1);
	bool success = fileSystem->Create(kernelBuf, 0);
	if (success)
		machine->WriteRegister(2, 0);
	else
		machine->WriteRegister(2, -1);
	delete[]kernelBuf;
	machine->AdjustPCRegs();
}

void
SyscallOpen()
{
	int strAddr = machine->ReadRegister(4);
	char *kernelBuf = machine->User2System(strAddr, MAX_FILENAME_LEN + 1);
	int openType = machine->ReadRegister(5);
	OpenFileId fid = fileSystem->fopen(kernelBuf, openType);
	machine->WriteRegister(2, fid);
	delete[]kernelBuf;
	machine->AdjustPCRegs();
}

void
SyscallRead()
{
	int strAddr = machine->ReadRegister(4);
	int charCount = machine->ReadRegister(5);
	OpenFileId fid = machine->ReadRegister(6);

	if (fid == 1 || charCount < 0) {
		machine->WriteRegister(2, -1);
		machine->AdjustPCRegs();
		return;
	}

	if (fid == 0) {
		char *kernelBuf = new char[charCount + 1];
		int charRead = gSynchConsole->Read(kernelBuf, charCount);
		if (charRead == -1)
			machine->WriteRegister(2, -2);
		else {
			kernelBuf[charRead] = '\0';
			machine->System2User(strAddr, charRead + 1, kernelBuf);
			machine->WriteRegister(2, charRead);
		}
		delete[]kernelBuf;
	} else {
		OpenFile *f = fileSystem->getOpenFile(fid);
		if (f == NULL) {
			machine->WriteRegister(2, -1);
		} else {
			char *kernelBuf = new char[charCount + 1];
			int charRead = f->Read(kernelBuf, charCount);
			if (charRead >= 0) {
				machine->System2User(strAddr, charRead, kernelBuf);
				if (charRead > 0) {
					machine->WriteRegister(2, charRead);
				} else {
					machine->WriteRegister(2, -2);
				}
			} else {
				machine->WriteRegister(2, -1);
			}
			delete[]kernelBuf;
		}
	}
	machine->AdjustPCRegs();
}

void
SyscallWrite()
{
	int strAddr = machine->ReadRegister(4);
	int charCount = machine->ReadRegister(5);
	OpenFileId fid = machine->ReadRegister(6);

	if (fid == 0 || charCount < 0) {
		machine->WriteRegister(2, -1);
		machine->AdjustPCRegs();
		return;
	}

	if (fid == 1) {
		char *kernelBuf = machine->User2System(strAddr, charCount);
		int charWritten = gSynchConsole->Write(kernelBuf, charCount);
		machine->WriteRegister(2, charWritten);
		delete[]kernelBuf;
	} else {
		OpenFile *f = fileSystem->getOpenFile(fid);
		if (f == NULL) {
			machine->WriteRegister(2, -1);
		} else {
			char *kernelBuf = machine->User2System(strAddr, charCount);
			int charWritten = f->Write(kernelBuf, charCount);
			if (charWritten == charCount) {
				machine->WriteRegister(2, charCount);
			} else {
				machine->WriteRegister(2, -1);
			}
			delete[]kernelBuf;
		}
	}
	machine->AdjustPCRegs();
}

void
SyscallSeek()
{
	int pos = machine->ReadRegister(4);
	OpenFileId fid = machine->ReadRegister(5);

	if (fid < 2) {
		machine->WriteRegister(2, -1);
		machine->AdjustPCRegs();
		return;
	}

	OpenFile *f = fileSystem->getOpenFile(fid);
	if (f == NULL) {
		machine->WriteRegister(2, -1);
	} else {
		int new_pos = f->Seek(pos);
		machine->WriteRegister(2, new_pos);
	}
	machine->AdjustPCRegs();
}

void
SyscallClose()
{
	OpenFileId fid = machine->ReadRegister(4);
	int ret = fileSystem->fclose(fid);
	machine->AdjustPCRegs();
}

void
SyscallExec()
{
}

void
SyscallJoin()
{
}

void
SyscallExit()
{
}

void
SyscallCreateSemaphore()
{
}

void SyscallWait()
{
}

void
SyscallSignal()
{
}

void
ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);
//    if ((which == SyscallException) && (type == SC_Halt)) {
//	DEBUG('a', "Shutdown, initiated by user program.\n");
//   	interrupt->Halt();
//    } else {
//	printf("Unexpected user mode exception %d %d\n", which, type);
//	ASSERT(FALSE);
//    }
//
	switch (which) {
		case NoException:
			break;
		case PageFaultException: // No valid translation found
			printf("No valid translation found\n");
			interrupt->Halt();
			break;
		case ReadOnlyException: // Write attempted to page marked "read-only"
			printf("Write attempted to page marked \"read-only\"\n");
			interrupt->Halt();
			break;
		case BusErrorException: // Translation resulted in an invalid physical address
			printf("Translation resulted in an invalid physical address\n");
			interrupt->Halt();
			break;
		case AddressErrorException: // Unaligned reference or one that was beyond the end of the address space\n");
			printf("Unaligned reference or one that was beyond the end of the address space\n");
			interrupt->Halt();
			break;
		case OverflowException: // Integer overflow in add or sub.
			printf("Integer overflow in add or sub.\n");
			interrupt->Halt();
			break;
		case IllegalInstrException: // Unimplemented or reserved instr.
			printf("Unimplemented or reserved instr.\n");
			interrupt->Halt();
			break;
		case SyscallException:
			switch (type) {
			case SC_Halt: // Halt machine
				interrupt->Halt();
				break;
			case SC_ReadInt: // Read an integer
				SyscallReadInt();
				break;
			case SC_PrintInt: // Print an integer
				SyscallPrintInt();
				break;
			case SC_ReadChar: // Read a character
				SyscallReadChar();
				break;
			case SC_PrintChar: // Print a character
				SyscallPrintChar();
				break;
			case SC_ReadString: // Read a string
				SyscallReadString();
				break;
			case SC_PrintString: // Print a string
				SyscallPrintString();
				break;
			case SC_CreateFile: // Create a new file
				SyscallCreateFile();
				break;
			case SC_Open: // Open a file for read/read+write
				SyscallOpen();
				break;
			case SC_Read: // Read a block of data from file
				SyscallRead();
				break;
			case SC_Write: // Write a block of data to file
				SyscallWrite();
				break;
			case SC_Seek: // Seek to a postion on file
				SyscallSeek();
				break;
			case SC_Close: // Close file
				SyscallClose();
				break;


			case SC_Exit:
				// interrupt->Halt();
				SyscallExit();
				break;
			case SC_Exec:
				// interrupt->Halt();
				SyscallExec();
				break;
			case SC_Join:
				// interrupt->Halt();
				SyscallJoin();
				break;
			case SC_Wait:
				SyscallWait();
				break;
			case SC_Signal:
				SyscallSignal();
				break;
			// case SC_Create:
			// 	// Not implemented yet
			// 	interrupt->Halt();
			// 	break;
			// case SC_Open:
			// 	// Not implemented yet
			// 	interrupt->Halt();
			// 	break;
			// case SC_Read:
			// 	// Not implemented yet
			// 	interrupt->Halt();
			// 	break;
			// case SC_Write:
			// 	// Not implemented yet
			// 	interrupt->Halt();
			// 	break;
			// case SC_Close:
			// 	// Not implemented yet
			// 	interrupt->Halt();
			// 	break;
			case SC_Fork:
				// Not implemented yet
				interrupt->Halt();
				break;
			case SC_Yield:
				// Not implemented yet
				interrupt->Halt();
				break;
			case SC_CreateSemaphore:
				SyscallCreateSemaphore();
				break;
			default:
				printf("Unexpected user mode exception %d %d\n", which, type);
				ASSERT(FALSE);
				break;
			}
			break;
		default:
			printf("Unexpected user mode exception %d %d\n", which, type);
			ASSERT(FALSE);
			break;
	}
}
