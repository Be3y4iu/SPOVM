#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>

#define COUNT 10

using namespace std;

string s_numbers[10]
{
	{ "Number 1" },{ "Number 2" },{ "Number 3" },{ "Number 4" },{ "Number 5" },
	{ "Number 6" },{ "Number 7" },{ "Number 8" },{ "Number 9" },{ "Number 10" }
};
  
using namespace std;

PROCESS_INFORMATION CreateNewProcess(char* name, char *comline);
void Create(char *name);
void Print(int ProccessNumber);

void main(int argc, char* argv[])
{
	if (argc == 2)
	{
		Print(atoi(argv[1]));
	}
	else
		Create(argv[0]);
	return;
}

void Create(char *name)
{
	int activeProcess = 0;
	HANDLE hCanWriteEvent = CreateEvent(NULL, FALSE, TRUE, "WriteProcess");
	char buffer[30];
	char choose;
	PROCESS_INFORMATION mas[COUNT];
	HANDLE hCanClose[30];
	while (choose = _getch())
	{
		if (choose == 'q') break;
		if (choose == '-')
			if (activeProcess)
			{
				SetEvent(hCanClose[activeProcess]);
				SetEvent(hCanWriteEvent);
				activeProcess--;

				
			}
		if ((choose == '+') && (activeProcess < COUNT))
		{
			sprintf(buffer, " %d", ++activeProcess);
			hCanClose[activeProcess] = CreateEvent(NULL, FALSE, FALSE, buffer);
			mas[activeProcess-1] = CreateNewProcess(name, buffer);
		}
		Sleep(300);
	}
	if (activeProcess)
	{
		while (activeProcess>0)
		{
			SetEvent(hCanClose[activeProcess]);
			SetEvent(hCanWriteEvent);
			activeProcess--;
		}
	}
	system("pause");
}

PROCESS_INFORMATION CreateNewProcess(char* name, char *comline)
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	CreateProcess(name, comline, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);
	return pi;
}

void Print(int numofProccess)
{
	HANDLE hCanWriteEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, "WriteProcess");
	char buffer[30];
	sprintf(buffer, " %d", numofProccess);
	HANDLE hCanClose = OpenEvent(EVENT_ALL_ACCESS, FALSE, buffer);

	while (1)
	{
		if (WaitForSingleObject(hCanWriteEvent, INFINITE) == WAIT_OBJECT_0)
		{
			if (WaitForSingleObject(hCanClose, 100) == WAIT_OBJECT_0)
			{
				SetEvent(hCanWriteEvent);
				return;
			}
			cout<< s_numbers[numofProccess - 1];
				Sleep(300);
			printf(" \n| ");
			SetEvent(hCanWriteEvent);
		}
	}
	return;
}