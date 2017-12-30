#include "syscall.h"

int read_unsigned_integer(OpenFileId fid)
{
	char c;
	int ret, res = 0;
	do {
		ret = Read(&c, 1, fid);
		if (ret == 1 && c == '-')
			return -1;
	} while (ret == 1 && (c < '0' || c > '9'));

	while (ret == 1 && c >= '0' && c <= '9') {
		res = res * 10 + (c - '0');
		ret = Read(&c, 1, fid);
	}
	return res;
}

int main()
{
	OpenFileId f_vn, f_tmp;
	int s_1, s_2, n;
	f_vn = Open("voinuoc_input.txt", 0);
	if (f_vn == -1)
		Exit(-1);
	if (CreateFile("tmp.txt") != 0)
		Exit(-1);
	f_tmp = Open("tmp.txt", 0);
	if (f_tmp == -1)
		Exit(-1);
	s_1 = s_2 = 0;
	while (1) {
		Wait("lock_file");
		n = read_unsigned_integer(f_vn);
		if (n == -1) {
			Signal("lock_file");
			break;
		}
		Signal("lock_file");
		if (s_1 <= s_2) {
			s_1 += n;
			if (n > 0)
				Write("1 ", 2, f_tmp);
		} else {
			s_2 += n;
			if (n > 0)
				Write("2 ", 2, f_tmp);
		}
	}
	Write("\n", 1, f_tmp);
	Close(f_vn);
	Close(f_tmp);
}
