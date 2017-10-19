#include "syscall.h"

int main() {
	char s[51];
	ReadString(s, 50);
	PrintString(s);
	ReadString(s, 50);
	PrintString(s);
	Halt();
}
