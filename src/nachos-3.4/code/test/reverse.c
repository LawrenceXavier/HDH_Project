#include "syscall.h"

#define MAX_LEN 256

#define MAX(a,b) ((a) > (b) ? (a) : (b))

char s_name[MAX_LEN + 1], d_name[MAX_LEN + 1];
char buf[MAX_LEN + 1];

void reverse(char *str, int len)
{
	int i;
	char tmp;
	for (i = 0; i < len / 2; ++i) {
		tmp = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = tmp;
	}
}

int main()
{
	int s_fid, d_fid;
	int charCount, charWritten;
	int curPos, prePos;
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

	curPos = Seek(-1, s_fid);
	
	while (curPos > 0) {
		prePos = MAX(0, curPos - MAX_LEN);
		if (Seek(prePos, s_fid) == -1) {
			PrintString("Loi seek file.\n");
			Halt();
		}
		charCount = Read(buf, curPos - prePos, s_fid);
		if (charCount != curPos - prePos) {
			PrintString("Loi doc file.\n");
			Halt();
		}
		reverse(buf, charCount);
		charWritten = Write(buf, charCount, d_fid);
		if (charWritten != charCount) {
			PrintString("Loi ghi file.\n");
			Halt();
		}
		curPos = prePos;
	}
	PrintString("Reverse file thanh cong.\n");
	Halt();
}
