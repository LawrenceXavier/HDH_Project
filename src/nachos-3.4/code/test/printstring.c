#include "syscall.h"

char str[100];

int main() {
	ReadString(str, 90);
	PrintString(str);
	Halt();
}
