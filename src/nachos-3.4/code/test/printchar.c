#include "syscall.h"

int
main()
{
	char c = 'c';
	PrintChar(c);
	c = '\n';
	PrintChar(c);
	//Halt();
	Exit(0);
}
