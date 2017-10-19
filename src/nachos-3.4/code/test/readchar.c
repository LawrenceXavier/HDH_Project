#include "syscall.h"

int main() {
	char res;
	res = ReadChar();
	PrintChar(res);
	res = ReadChar();
	PrintChar(res);
	Halt();
	return 0;
}
