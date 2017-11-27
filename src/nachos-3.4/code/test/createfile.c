#include "syscall.h"

#define MAX_FILENAME_SIZE 255

char fileName[MAX_FILENAME_SIZE + 1];

int main()
{
	PrintString("Name of file to create: ");
	ReadString(fileName, MAX_FILENAME_SIZE);
	if (CreateFile(fileName) == 0)
		PrintString("Successfully create new file.\n");
	else
		PrintString("Fail to create file.\n");

	Halt();
}
