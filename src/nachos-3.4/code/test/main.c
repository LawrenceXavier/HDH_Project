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
	OpenFileId f_in, f_out, f_sv, f_tmp;
	SpaceId job_sv, job_vn;
	int n, ret;
	char c;
	if (CreateFile("output.txt") != 0) {
		PrintString("Cannot create output file\n");
		Exit(-1);
	}
	if (CreateSemaphore("lock_file", 1) == -1) {
		PrintString("Cannot create semaphore lock_file\n");
		Exit(-1);
	}
	f_in = Open("input.txt", 1);
	if (f_in == -1) {
		PrintString("Fail to open input\n");
		Exit(-1);
	}
	f_out = Open("output.txt", 0);
	if (f_out == -1) {
		PrintString("Fail to open output\n");
		Exit(-1);
	}
	read_unsigned_integer(f_in, &n);
	while (n--) {
		if (CreateFile("sinhvien_inp.txt") != 0) {
			PrintString("Cannot create input for sinhvien\n");
			Exit(-1);
		}
		f_sv = Open("sinhvien_inp.txt", 0);
		if (f_sv == -1) {
			PrintString("Fail to open input for sinhvien\n");
			Exit(-1);
		}
		if (CreateFile("voinuoc_input.txt") != 0) {
			PrintString("Cannout create input for voinuoc\n");
			Exit(-1);
		}
		do {
			ret = Read(&c, 1, f_in);
			if (ret == 1)
				Write(&c, 1, f_sv);
		} while (ret == 1 && c != '\n');
		Close(f_sv);

		job_sv = Exec("test/sinhvien");
		if (job_sv == -1) {
			PrintString("Fail to exec sinhvien\n");
			Exit(-1);
		}
		job_vn = Exec("test/voinuoc");
		if (job_vn == -1) {
			PrintString("Fail to exec voinuoc\n");
			Exit(-1);
		}
		if (Join(job_sv) == -1) {
			PrintString("sinhvien execute uncompletely\n");
			Exit(-1);
		}
		if (Join(job_vn) == -1) {
			PrintString("voinuoc execute uncompletely\n");
			Exit(-1);
		}
		f_tmp = Open("tmp.txt", 1);
		if (f_tmp == -1) {
			PrintString("Fail to open tmp file to read.\n");
			Exit(-1);
		}
		do {
			ret = Read(&c, 1, f_tmp);
			if (ret == 1)
				Write(&c, 1, f_out);
		} while (ret == 1);
		Close(f_tmp);
	}
	Close(f_in);
	Close(f_out);
	Exit(0);
}
