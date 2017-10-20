#include "syscall.h"

int main() {
	int i;
	char c;
	for (i = 0, c = 0; i < 128; ++i, ++c) {
		PrintChar(c);
		PrintChar(' ');
		PrintInt(i);
		PrintChar('\n');
	}
	Halt();
	return 0;
}
