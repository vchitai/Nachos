#include "syscall.h"
#define MAX_NUM 100
int main() {
	int n;
	int i, j;
	int a[MAX_NUM];
	PrintString("Nhap so n: ");
	n = ReadInt();
	PrintString("Nhap cac phan tu: ");
	for (i = 0; i < n; i++) {
		a[i] = ReadInt();
	}

	for (i = 0; i < n - 1; i++) {
		for (j = i; j < n; j++) {
			if (a[i] > a[j]) {
				int temp;
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
		}
	}

	for (i = 0; i < n; i++) {
		PrintInt(a[i]);
		PrintString(" ");
	}
	Halt();
}
