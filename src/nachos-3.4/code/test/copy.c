#include "syscall.h"

#define MAX_LEN 256

char s_name[MAX_LEN + 1], d_name[MAX_LEN + 1];
char buf[MAX_LEN + 1];

int main()
{
	int s_fid, d_fid;
	int charCount, charWritten;
	PrintString("Nhap file nguon: ");
	ReadString(s_name, MAX_LEN);
	PrintString("Nhap file dich: ");
	ReadString(d_name, MAX_LEN);

	s_fid = Open(s_name, 1);
	if (s_fid == -1) {
		PrintString("Loi khi mo file nguon.\n");
		Halt();
	}

	if (CreateFile(d_name) == -1) {
		PrintString("Loi khi tao file dich.\n");
		Halt();
	}

	d_fid = Open(d_name, 0);
	if (d_fid == -1) {
		PrintString("Loi khi mo file dich.\n");
		Halt();
	}

	while ((charCount = Read(buf, MAX_LEN, s_fid)) > 0) {
		charWritten = Write(buf, charCount, d_fid);
		if (charCount != charWritten) {
			PrintString("Loi trong qua trinh copy file.\n");
			Halt();
		}
	}

	if (charCount == -1)
		PrintString("Loi trong qua trinh doc file.\n");
	else
		PrintString("Copy file thanh cong.\n");
	Halt();
}
