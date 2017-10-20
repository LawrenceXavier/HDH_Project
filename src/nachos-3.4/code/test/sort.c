#include "syscall.h"

int n, a[100];

int main()
{
	int i, j, tmp;
	n = ReadInt();
	for (i = 0; i < n; ++i) {
		a[i] = ReadInt();
		//PrintChar('a');
	}
	PrintInt(n); PrintChar('\n');
	for (i = 0; i + 1 < n; ++i)
		for (j = i + 1; j < n; ++j)
			if (a[i] > a[j]) {
				tmp = a[i];
				a[i] = a[j];
				a[j] = tmp;
			}
	for (i = 0; i < n; ++i) {
		PrintInt(a[i]);
		if (i + 1 == n)
			PrintChar('\n');
		else
			PrintChar(' ');
	}
}
