/* createfile syscall demo */
#include "syscall.h"
#include "copyright.h"

int main()
{
	int isCreate = CreateFile("hello");
	int write;
	int read;
	int seek;
	char mess[255];
	OpenFileID open; 
	if (isCreate)
	{
		PrintString("Can not create file \'hello.txt\'\n");
	} 
	else
	{
		PrintString("Successfully create file\n");
		PrintString("Open File: \n");
		open = OpenFileSyscall("hello", 0);
		if (open==-1) 
			PrintString("Can not open file\n");
		else 
			PrintString("Open file succesfully\n"); 
		PrintString("Write File: \n");
		write = WriteFile("alskdjflasjdf", 13, open);
		if (write == -1)
			PrintString("Can not write\n");
		else
			PrintString("Write File Succesfully\n");
		PrintString("Seek File To Pos 2: \n");
		seek = SeekFile(2, open);
		if (seek == -1)
			PrintString("Can not seek\n");
		else
			PrintString("Seek File Successfully\n");
		//PrintString("Read File To Variable Mess: \n");
		read = ReadFile(mess, 3, open);
		if (read == -1)
			PrintString("Can not read\n");
		else {
			PrintString("Read File Successfully\n");
			PrintString("Variable Mess now: \n");
			PrintString(mess);
		}
		CloseFile(open);
	}

	Halt();
}
