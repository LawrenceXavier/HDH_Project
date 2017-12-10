#include "syscall.h"

#define BUF_LEN 255

char buf[BUF_LEN + 1];

int main()
{
	int charCount = 0, charWritten;
	while ((charCount = Read(buf, BUF_LEN, 0)) > 0) {
		buf[charCount++] = '\n';
		charWritten = Write(buf, charCount, 1);
		if (charWritten != charCount) {
			PrintString("Error when printing to console.\n");
			break;
		}
	}
	Halt();
}
