#include "syscall.h"

#define MAX_LEN 255
#define BUF_LEN 255

char file_name[MAX_LEN + 1];
char buf[BUF_LEN + 1];

int main()
{
	int charRead, charWritten;
	int fid;
	PrintString("Nhap ten file: ");
	ReadString(file_name, MAX_LEN);

	fid = Open(file_name, 1);
	while ((charRead = Read(buf, BUF_LEN, fid)) > 0) {
		charWritten = Write(buf, charRead, 1);
		if (charRead != charWritten) {
			PrintString("Loi khi hien thi file.\n");
			Halt();
		}
	}
	Close(fid);
	Halt();
}
