#include "syscall.h"

int n, a[100];

int main()
{
	int i, j, tmp;
	PrintString("Nhap so luong phan tu cua mang: ");
	n = ReadInt();
	PrintString("Nhap cac phan tu cua mang: ");
	for (i = 0; i < n; ++i) {
		a[i] = ReadInt();
		//PrintChar('a');
	}

	for (i = 0; i + 1 < n; ++i)
		for (j = i + 1; j < n; ++j)
			if (a[i] > a[j]) {
				tmp = a[i];
				a[i] = a[j];
				a[j] = tmp;
			}

	PrintString("Cac phan tu cua mang sau khi duoc sap xep: ");
	for (i = 0; i < n; ++i) {
		PrintInt(a[i]);
		if (i + 1 == n)
			PrintChar('\n');
		else
			PrintChar(' ');
	}
}
