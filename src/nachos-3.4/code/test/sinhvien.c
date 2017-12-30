#include "syscall.h"

int read_unsigned_integer(OpenFileId fid, int *n)
{
	char c;
	int ret;
	*n = 0;
	do {
		ret = Read(&c, 1, fid);
	} while (ret == 1 && (c < '0' || c > '9'));

	while (ret == 1 && c >= '0' && c <= '9') {
		*n = (*n) * 10 + (c - '0');
		ret = Read(&c, 1, fid);
	}
	
	if (ret <= 0)
		return -2;
	if (c == '\n')
		return -1;
	return 0;
}


int main()
{
	OpenFileId f_sv, f_vn;
	char c;
	int ret;
	f_sv = Open("sinhvien_inp.txt", 1);
	if (f_sv == -1)
		Exit(-1);
	f_vn = Open("voinuoc_input.txt", 0);
	if (f_vn == -1)
		Exit(-1);

	do {
		ret = Read(&c, 1, f_sv);
		if (ret == 1 && c >= '0' && c <= '9') {
			Wait("lock_file");
			do {
				Write(&c, 1, f_vn);
				ret = Read(&c, 1, f_sv);
			} while (ret == 1 && c >= '0' && c <= '9');
			Write(" ", 1, f_vn);
			Signal("lock_file");
		}
	} while (ret == 1);
	Wait("lock_file");
	// if (CreateFile("voinuoc_input.txt") != 0) {
	// 	Signal("lock_file");
	// 	Exit(-1);
	// }
	Write("-\n", 2, f_vn);
	Signal("lock_file");

	Close(f_vn);
	Exit(0);
}
