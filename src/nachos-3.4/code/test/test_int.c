#include "syscall.h"

int main()
{
	int a, b;
	a = ReadInt();
	PrintInt(a);
	PrintChar('\n');

	b = ReadInt();
	PrintInt(b);
	PrintChar('\n');
	Halt();
}
