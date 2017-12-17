// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------
/*
void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    if ((which == SyscallException) && (type == SC_Halt)) {
	DEBUG('a', "Shutdown, initiated by user program.\n");
   	interrupt->Halt();
    } else {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }
}*/
#define MAX_LENGTH_INT 20
#define MAX_LENGTH_STRING 255

//Ham tang thanh ghi PC sau khi xu ly xong syscall
void IncrementPC()
{
	//Gan thanh ghi PrevPCReg = PCReg 
  	int pc = machine->ReadRegister(PCReg);
  	machine->WriteRegister(PrevPCReg, pc);
	//Gan thanh ghi PCReg = NextPCReg 
  	pc = machine->ReadRegister(NextPCReg);
  	machine->WriteRegister(PCReg, pc);
	//Gan thanh ghi NextPCReg = NextPCReg + 4
  	pc += 4;
  	machine->WriteRegister(NextPCReg, pc);
	//Sau khi goi ham nay thanh ghi PC se dich chuyen den lenh tiep theo
}

//Ham xu ly Exception
void ExceptionHandler(ExceptionType which)
{
    	int type = machine->ReadRegister(2);
	//printf("Current Thread: %d Unexpected user mode exception %d %d\n",currentThread->processID, which, type);
	//Tao doi tuong gSynchConsole tu class SynchConsole de su dung
	//SynchConsole gSynchConsole;
	switch(which)
	{	
		//Xu ly Syscall Exception
		case SyscallException:
			switch (type) 
			{	
				//Syscall Halt de tat may	
				case SC_Halt:
				{
					DEBUG('a', "Shutdown, initiated by user program.\n");
	   				interrupt->Halt();
					break;
				}
				//Syscall ReadInt doc so nguyen
				case SC_ReadInt:
				{
					//Doc tung char, tinh toan va luu lai o bien number
					int number;
					int lenInt;
					int t;
					t = 1;
					number = 0;
					char bufferInt[MAX_LENGTH_INT];
					//Doc chuoi vao bufferInt
					lenInt = gSynchConsole->Read(bufferInt, MAX_LENGTH_INT);
					int i1;
					//Tinh toan so nguyen number tu bufferInt
					for (i1 = lenInt - 1; i1 >= 1; i1--) {
						number += (int)(bufferInt[i1] - '0') * t;
						t = t * 10;
					}
					if (bufferInt[0] == '-')
						number = -number;
					else
						number += (bufferInt[0] - '0') * t;
					//Kiem tra xem co ki tu nao khong phai so, neu co -> number = 0
					for (i1 = 1; i1 < lenInt; i1++) {
						if (bufferInt[i1] < '0' || bufferInt[i1] > '9') {
							number = 0;
							break;					
						}
					}
					if ((bufferInt[0] < '0' || bufferInt[0] > '9') && (bufferInt[0] != '-')) {
						number = 0;	
					}
					machine->WriteRegister(2, number);
					break;
				}
				//Syscall PrintInt in so nguyen ra man hinh
				case SC_PrintInt:
				{
					//Doi so nguyen thanh chuoi roi xuat chuoi
					char sInt[MAX_LENGTH_INT]; //Chuoi tam de bien doi
					char kqInt[MAX_LENGTH_INT]; //Chuoi ket qua
					int num;
					num = machine->ReadRegister(4);
					int i;
					i = 0;
					int start; //Vi tri bat dau cua so, neu duong la vi tri 0, neu am la vi tri 1
					if (num < 0) {
						start = 1;
						kqInt[0] = '-';
						num = -num;
					}
					else {
						start = 0;
					}
					//Neu num = 0, in ra so 0
					if (num == 0) {
						kqInt[0] = '0';
						gSynchConsole->Write(kqInt, 1);
						break;
					}		
					//Neu num != 0, lay tung chu so cua num vao chuoi sInt
					while (num != 0) {
						sInt[i] = num % 10 + 48;
						num = num / 10;
						i++;
					}
					int len;
					len = i;
					int j;
					j = start;
					//Lay chuoi ket qua tu chuoi sInt
					for (i = len - 1; i >= 0; i--) {
						kqInt[j] = sInt[i];
						j++;
					}
					gSynchConsole->Write(kqInt, len + start);
					break;
				}
				//Syscall ReadChar doc ky tu tu ban phim
				case SC_ReadChar:
				{
					char r3;
					//Dung gSynchConsole de doc mot ky tu
					gSynchConsole->Read(&r3,1);
					//Tra ve ket qua doc duoc 
					machine->WriteRegister(2,r3);
					break;
				}
				//Syscall PrintChar in ky tu ra man hinh
				case SC_PrintChar:
				{
					char r4;
					//Lay tham so tu thanh ghi
					r4 = machine->ReadRegister(4);		
					//Dung gSynchConsole de in mot ky tu ra man hinh
					gSynchConsole->Write(&r4,1);
					break;
				}
				//Syscall ReadString doc chuoi ky tu tu ban phim
				case SC_ReadString:
				{
					//Lay tham so buffer trong UserSpace va do dai buffer tu thanh ghi roi tao buffer tuong ung trong Kernel Space
					int bf1;
					bf1 = machine->ReadRegister(4);
					int l1;
					l1 = machine->ReadRegister(5);
					char* z;	
					z = new char[l1];
					//Dung gSynchConsole de doc chuoi tu ban phim vao buffer trong Kernel Space
					gSynchConsole->Read(z,l1);
					//Chep noi dung chuoi tu buffer trong Kernel Space vao buffer trong User Space
					machine->System2User(bf1,l1,z);
					delete[] z;
					break;
				}
				//Syscall PrintString xuat chuoi ky tu ra man hinh
				case SC_PrintString:
				{
					//Lay tham so buffer trong UserSpace va tinh do dai buffer
					int bf2;
					bf2 = machine->ReadRegister(4);
					char* r6;
					//Chep noi dung chuoi tu buffer trong User Space vao buffer trong Kernel Space					
					r6 = machine->User2System(bf2,MAX_LENGTH_STRING);
					//Dung gSynchConsole de xuat chuoi tu buffer Kernel Space ra man hinh
					gSynchConsole->Write(r6,MAX_LENGTH_STRING);
					if (r6!=NULL)		
						delete[] r6;
					break;
				}
				case SC_CreateFile: 
				{
					char *fileName;
					fileName = new char[MAX_LENGTH_STRING];
					//Doc pointer string
					int inputString;
					inputString = machine->ReadRegister(4);
					fileName = machine->User2System(inputString, MAX_LENGTH_STRING);
					if (fileSystem->Create(fileName, 0) == FALSE) {
						machine->WriteRegister(2, -1);
					}
					else {
						machine->WriteRegister(2, 0);
					}
					delete[] fileName;
					break;
				}
				case SC_OpenFileID:
				{
					char *fileName;
					fileName = new char[MAX_LENGTH_STRING];
					//Doc pointer string
					int inputString;
					inputString = machine->ReadRegister(4);
					fileName = machine->User2System(inputString, MAX_LENGTH_STRING);
					//Doc type
					int readType;
					readType = machine->ReadRegister(5);
					if (fileSystem->id > fileSystem->maxID) {
						machine->WriteRegister(2, -1);
						delete[] fileName;
						break;
					}
					if (strcmp(fileName, "stdin") == 0) {
						machine->WriteRegister(2, 0);
						delete[] fileName;
						break;
					}
					if (strcmp(fileName, "stdout") == 0) {
						machine->WriteRegister(2, 1);
						delete[] fileName;
						break;
					}
					fileSystem->openFile[fileSystem->id] = fileSystem->Open(fileName, readType);
					if (fileSystem->openFile[fileSystem->id - 1] == NULL) {
						machine->WriteRegister(2, -1);
					}
					else {
						machine->WriteRegister(2, fileSystem->id - 1);
					}
					delete[] fileName;
					break;
				}
				case SC_CloseFile:
				{
					int fileID;
					fileID = machine->ReadRegister(4);
					int close;
					close = fileSystem->closeFile(fileID);
					if (close == -1) {
						//Khong dong file duoc (file chua mo)
						machine->WriteRegister(2, -1);
					}
					else {
						//Thanh cong
						machine->WriteRegister(2, 0);
					}
					break;
				}
				case SC_ReadFile:
				{
					int bufferPointer;
					bufferPointer = machine->ReadRegister(4);
					int charcount;
					charcount = machine->ReadRegister(5);
					int fileID;
					fileID = machine->ReadRegister(6);
					char *buffer;
					buffer = new char[charcount];
					//Kiem tra xem file da mo hay chua
					if (fileSystem->isOpened(fileID) == -1) {
						printf("File chua duoc mo!");
						machine->WriteRegister(2, -1);
						delete[] buffer;
						break;
					}
					//Neu la doc tu console
					if (fileSystem->openFile[fileID]->getType() == STDIN_TYPE) {
						//Doc tu input console
						int readSize;
						readSize = gSynchConsole->Read(buffer, charcount);
						machine->System2User(bufferPointer, readSize, buffer);
						machine->WriteRegister(2, readSize);
						delete[] buffer;
						break;
					}
					//Neu doc tu file
					int readSize;
					readSize = fileSystem->openFile[fileID]->Read(buffer, charcount);
					if (readSize > 0) {
						machine->System2User(bufferPointer, readSize, buffer);
						machine->WriteRegister(2, readSize);
						delete[] buffer;
						break;
					}
					else {
						machine->WriteRegister(2, -1);
						delete[] buffer;
						break;
					}
				}
				case SC_WriteFile:
				{
					int bufferPointer;
					bufferPointer = machine->ReadRegister(4);
					int charcount;

					charcount = machine->ReadRegister(5);
					int fileID;
					fileID = machine->ReadRegister(6);
					char *buffer;
					buffer = new char[charcount];
					//Kiem tra xem file mo hay chua
					if (fileSystem->isOpened(fileID) == -1) {
						machine->WriteRegister(2, -1);
						delete[] buffer;
						break;
					}

					buffer = machine->User2System(bufferPointer, charcount);
					//Neu la ghi ra output console
					if (fileSystem->openFile[fileID]->getType() == STDOUT_TYPE) {
						//printf("%d Here %d\n", handleType,STDOUT_TYPE);
						int writeSize;
						writeSize = gSynchConsole->Write(buffer, charcount);
						machine->WriteRegister(2, writeSize);
						delete[] buffer;
						break;
					}
					//Neu la ghi ra file, ap dung voi file co type = 0 (read and write)
					if (fileSystem->openFile[fileID]->getType() == READ_AND_WRITE_TYPE) {
						int writeSize;
						writeSize = fileSystem->openFile[fileID]->Write(buffer, charcount);
						machine->WriteRegister(2, writeSize);
						delete[] buffer;
						break;
					}
					else {
						machine->WriteRegister(2, -1);
						delete[] buffer;
						break;
					}
				}
				case SC_SeekFile:
				{
					int seekPos;
					seekPos = machine->ReadRegister(4);
					int fileID;
					fileID = machine->ReadRegister(5);
					//Kiem tra da mo file hay chua
					if (fileSystem->isOpened(fileID) == -1) {
						machine->WriteRegister(2, -1);
						break;
					}
					int length;
					length = fileSystem->openFile[fileID]->Length();
					if (seekPos >= length) {
						machine->WriteRegister(2, -1);
						break;
					}
					if (seekPos >= 0) {
						fileSystem->openFile[fileID]->Seek(seekPos);
						machine->WriteRegister(2, seekPos);
					}
					else if (seekPos == -1) {
						fileSystem->openFile[fileID]->Seek(length - 1);
						machine->WriteRegister(2, length);
					}
					else {
						printf("Vi tri dich chuyen khong chinh xac!");
						machine->WriteRegister(2, -1);
					}
					break;
				}
				case SC_Exec:
				{
					int name_userS;
					name_userS = machine->ReadRegister(4);
					char* name_kernelS;
					name_kernelS = machine->User2System(name_userS,MAX_LENGTH_STRING);
					if (name_kernelS != NULL) {
						int result;
						result = pTab->ExecUpdate(name_kernelS);
						machine->WriteRegister(2,result);
						delete name_kernelS;
					} else {
						printf("Khong mo duoc File\n");
						machine->WriteRegister(2,-1);
					}
					break;
				}
				case SC_Join:
				{
					int joinID;
					joinID = machine->ReadRegister(4);
					int result;
					result = pTab->JoinUpdate(joinID);
					machine->WriteRegister(2,result);
					break;
				}
				case SC_Exit:
				{
					int exitStatus;
					exitStatus = machine->ReadRegister(4);
					int result;
					result = pTab->ExitUpdate(exitStatus);
					machine->WriteRegister(2,result);
					break;
				}
				case SC_CreateSemaphore:
				{
					int nameAddr;
					nameAddr = machine->ReadRegister(4);
					int semval;
					semval = machine->ReadRegister(5);
					char* name;
					name = machine->User2System(nameAddr,MAX_LENGTH_STRING);

					int result;
					result = semTab->Create(name,semval);
					machine->WriteRegister(2,result);
					break;
				}
				case SC_Wait:
				{
					int nameAddr;
					nameAddr = machine->ReadRegister(4);
					char* name;
					name = machine->User2System(nameAddr,MAX_LENGTH_STRING);

					int result;
					result = semTab->Wait(name);
					machine->WriteRegister(2,result);
					break;
				}
				case SC_Signal:
				{
					int nameAddr;
					nameAddr = machine->ReadRegister(4);
					char* name; 
					name = machine->User2System(nameAddr,MAX_LENGTH_STRING);

					int result;
					result = semTab->Signal(name);
					machine->WriteRegister(2,result);
					break;
				}
				//Xu ly cac syscall chua ho tro
				default:
				{
					printf("Unexpected user mode exception %d %d\n", which, type);
					ASSERT(FALSE);
					break;
				}
			}
			//Tang thanh ghi PC sau khi xu ly Exception
			IncrementPC();
			break;
		//Xu ly PageFaultException
		case PageFaultException:
			printf("No valid translation found %d %d\n", which, type);
			ASSERT(FALSE);
			break;
		//Xu ly ReadOnlyException
		case ReadOnlyException:
			printf("Write attempted to page marked read-only %d %d\n", which, type);
			ASSERT(FALSE);
			break;
		//Xu ly BusErrorException
		case BusErrorException:
			printf("Translation resulted in an invalid physical address %d %d\n", which, type);
			ASSERT(FALSE);
			break;
		//Xu ly AddressErrorException
		case AddressErrorException:
			printf("Unaligned reference or one that was beyond the end of the address space %d %d\n", which, type);
			ASSERT(FALSE);
			break;
		//Xu ly Overflow Exception
		case OverflowException:
			printf("Integer overflow in add or sub. %d %d\n", which, type);
			ASSERT(FALSE);
			break;
		//Xu ly IllegalInstr Exception
		case IllegalInstrException:
			printf("Unimplemented or reserved instr. %d %d\n", which, type);
			ASSERT(FALSE);
			break;
		//Xu ly cac Exception con lai
		default:
			printf("Unexpected user mode exception %d %d\n", which, type);
			ASSERT(FALSE);
			break;
	}
}

