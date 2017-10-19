#include "syscall.h"

int main() {
	char s[100];
	int i;
	for (i = 0; i < 100; ++i)
		s[i] = '\0';
	s[0] = 'H';
	s[1] = 'o';
	s[2] = 'w';
	s[3] = ' ';
	s[4] = 'a';
	s[5] = 'b';
	s[6] = 'o';
	s[7] = 'u';
	s[8] = 't';
	s[9] = ' ';
	s[10] = 't';
	s[11] = 'h';
	s[12] = 'i';
	s[13] = 's';
	s[14] = '?';
	// for (i = 0; *(s+i) != '\0'; ++i)
	//	PrintChar(s[i]);
	PrintString(s);
	Halt();
	// PrintString("How about this?");
	// PrintString("hello world \n how are you? \n is everything okay?");
	return 0;
}
