#include "syscall.h"

int
main()
{	
	OpenFileID voinuoc, output;
	char c[1];
	int  next, currentSinhVien, voi1, voi2;

	//Mo file result
	output = OpenFileSyscall("result.txt",0);
	if (output == -1)
		Exit(-1);

	//Khoi tao gia tri 2 voi nuoc
	voi1 = 0;
	voi2 = 0;
	while (1) {
		//Doi tien trinh sinhvien cho phep thuc hien
		Wait("voinuoc");

		//Mo file voinuoc, lay gia tri cua sinh vien hien tai
		//Neu voi nuoc nao co gia tri thap hon thi cong vao, ghi ket qua vao file result
		//Neu sinhvien thong bao dung thi Exit
		voinuoc = OpenFileSyscall("voinuoc.txt",1);
		currentSinhVien = 0;
		next = 0;
		while (ReadFile(c,1,voinuoc)>0) {
			if (c[0] == '~') {
				next = 1;
				break;
			}
			if (c[0] == ' ' || c[0] == '\n') {
				break;
			}
			currentSinhVien = currentSinhVien*10 + (c[0]-'0');
		}	
		if (voi2 < voi1) {
			WriteFile("2 ",2,output);
			voi2 = voi2 + currentSinhVien;
		} else {
			WriteFile("1 ",2,output);
			voi1 = voi1 + currentSinhVien;
		}
		if (next == 1) {
			WriteFile("\n",1,output);
			CloseFile(voinuoc);
			break;
		}
		CloseFile(voinuoc);
		Signal("sinhvien");
	}
	CloseFile(output);
	PrintString("voinuoc exiting...\n");
	Signal("sinhvien");
	Exit(0);
}
