#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlconv.h>
#pragma comment(lib, "user32.lib")

#define BUF_SIZE 256
TCHAR szName[] = TEXT("Local\\mapfile");

int main()
{
	HANDLE hMapFile;
	LPCTSTR pBuf;
	LPSTR lpszAscii = NULL;

	volatile HANDLE sem, print_sem;
	sem = OpenSemaphore(SYNCHRONIZE, TRUE, L"SEMAFOR");

	print_sem = CreateSemaphore(NULL, 0, 1, L"SEMAWHILE");

	USES_CONVERSION;


	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		szName);               // name of mapping object

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
			GetLastError());
		return -1;
	}

	pBuf = (LPTSTR)MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		BUF_SIZE);

	if (pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return -1;
	}

	while (TRUE)
	{
		WaitForSingleObject(sem, INFINITE);

		lpszAscii = T2A((LPTSTR)pBuf);
		int len = strlen(lpszAscii);
		char something[256];
		strcpy(something, lpszAscii);

		printf("%s", something);

		ReleaseSemaphore(print_sem, 0, NULL);
	}
	


	Sleep(100000);

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}