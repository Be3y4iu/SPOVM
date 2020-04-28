#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <atlstr.h>

#define SIZE 256
TCHAR file_name[] = TEXT("Local\\mapfile");


int main()
{
	HANDLE file_handler;
	LPCTSTR buffer_pointer;

	TCHAR string[SIZE];
	char buffer[SIZE];
	
	volatile HANDLE sem, print_sem;
	sem = CreateSemaphore(NULL, 0, 1, L"SEMAFOR");
	print_sem = OpenSemaphore(SYNCHRONIZE, TRUE, L"SEMAWHILE");


	file_handler = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		SIZE,
		file_name
		);

	if (NULL == file_handler)
	{
		perror("CreateFileMapping");
		return -1;
	}

	buffer_pointer = (LPTSTR)MapViewOfFile(
		file_handler,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		SIZE
		);

	if (NULL == buffer_pointer)
	{
		perror("MapViewMapping");
		CloseHandle(file_handler);
		return -1;
	}

	USES_CONVERSION;

	bool b = false;
	while (TRUE)
	{
		if (b)
			WaitForSingleObject(print_sem, INFINITE);

		printf("Enter the string\n");
		
		fgets(buffer, SIZE, stdin);

		_tcscpy(string, A2T(buffer));

		CopyMemory((PVOID)buffer_pointer, string, (SIZE - 1 * sizeof(TCHAR)));

		ReleaseSemaphore(sem, 1, NULL);
		b = true;
	}

	

	Sleep(1000000000);

	UnmapViewOfFile(buffer_pointer);

	CloseHandle(file_handler);

	return 0;

}
