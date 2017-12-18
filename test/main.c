#include "syscall.h"
int
main()
{	
	OpenFileID input, output, sinhvien, result;
	SpaceID sv;
	int n, tmp, writed;
	char c[1];

	//Khoi tao cac file can thiet, neu khong thanh cong Exit
	if (CreateFile("output.txt") == -1)
		Exit(-1);
	if (CreateFile("sinhvien.txt") == -1)
		Exit(-1);
	if (CreateFile("voinuoc.txt") == -1)
		Exit(-1);
	if (CreateFile("result.txt") == -1)
		Exit(-1);
	if (CreateSemaphore("sinhvien",0) == -1)
		Exit(-1);
	if (CreateSemaphore("voinuoc",0) == -1)
		Exit(-1);

	//Mo file input va output
	input = OpenFileSyscall("input.txt",0);
	output = OpenFileSyscall("output.txt",0);
	if (input == -1)
		Exit(-1);
	if (output == -1)
		Exit(-1);

	//Doc so luot sinh vien vao lay nuoc
	n = 0;
	while (ReadFile(c,1,input)!=0) {
		if (c[0] == '\n')
			break;
		if (c[0] >='0' && c[0] <='9')
			n = 10*n + c[0] - '0';
	}

	//Ung voi tung luot sinh vien vao lay nuoc
	while (n--) {
		//Mo file sinh vien va ghi cac sinh vien lay nuoc dot nay
		sinhvien = OpenFileSyscall("sinhvien.txt",0);
		writed = 0;
		while (ReadFile(c,1,input)>0) {
			if (c[0] == '\n') {
				WriteFile(c,1,sinhvien);
				break;
			}
			WriteFile(c,1,sinhvien);
			writed++;
		}
		CloseFile(sinhvien);
		
		//Goi thuc thi chuong trinh sinh vien, nhan thong bao loi va thoat
		sv = Exec("../test/sinhvien");
		if (sv!=-1) 
			tmp = Join(sv);
		if (tmp == -1) {
			CloseFile(input);
			Exit(-1);		
		}	
		
		//Dat lai noi dung file sinh vien
		sinhvien = OpenFileSyscall("sinhvien.txt",0);	
		while (writed--) {
			WriteFile("\n",1,sinhvien);
		}
		CloseFile(sinhvien);
		
		//Lay ket qua tu file result va ghi vao file output
		writed = 0;
		result = OpenFileSyscall("result.txt",1);
		while (ReadFile(c,1,result)>0) {
			WriteFile(c,1,output);
			writed++;
			if (c[0] == '\n')
				break;		
		}

		//Dat lai noi dung file result
		SeekFile(0,result);
		while (writed--) {
			WriteFile("\n",1,result);
		}
		CloseFile(result);
	}
	CloseFile(input);
	CloseFile(output);
	Exit(0);
}
