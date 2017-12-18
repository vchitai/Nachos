#include "syscall.h"

int
main()
{	
	OpenFileID sinhvien, voinuoc;
	char c[1];
	SpaceID vn;
	int next, writed;

	//Mo file sinhvien.txt va goi chuong trinh voinuoc bat dau thuc hien va doi sinh vien ra hieu	
	sinhvien = OpenFileSyscall("sinhvien.txt",0);
	vn = Exec("../test/voinuoc");
	if (sinhvien == -1)
		Exit(-1);

	while (1) {
		next = 0;
		//Mo file voinuoc, doc tung so va ghi vao file voinuoc
		voinuoc = OpenFileSyscall("voinuoc.txt",0);
		writed = 0;
		while (ReadFile(c,1,sinhvien) > 0) {
			writed++;
			if (c[0] == '\n') {
				WriteFile("~",1,voinuoc);
				next = 1;
				break;
			}	
			if (c[0] == ' ')
				break;	
			WriteFile(c,1,voinuoc);	
		}
		CloseFile(voinuoc);

		//Thong bao voinuoc thuc hien va doi khi voinuoc hoan thanh va thong bao
		Signal("voinuoc");	
		Wait("sinhvien");

		//Dat lai noi dung file voinuoc
		voinuoc = OpenFileSyscall("voinuoc.txt",0);
		while (writed--) {
			WriteFile("\n",1,voinuoc);
		}
		CloseFile(voinuoc);
		if (next == 1)
			break;
	}
	PrintString("sinhvien exiting...\n");
	CloseFile(sinhvien);
	Exit(0);
}
