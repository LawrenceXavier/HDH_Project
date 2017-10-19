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

void
AdjustPCRegs()
{
	int pc;

	pc = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, pc);
	pc = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, pc);
	pc += 4;
	machine->WriteRegister(NextPCReg, pc);
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
			case SC_Halt:
				interrupt->Halt();
				break;
			case SC_ReadInt:
				break;
			case SC_PrintInt:
				break;
			case SC_ReadChar:
				break;
			case SC_PrintChar:
				break;
			case SC_ReadString:
				break;
			case SC_PrintString:
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
