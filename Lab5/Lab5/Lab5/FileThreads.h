#pragma once
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

struct FileInfo
{
	HANDLE headerOfFile;
	DWORD numberOfBytes;
	CHAR  buffer[100];
	DWORD  positionInFile;
	DWORD  positionOutFile;
	OVERLAPPED Overlapped;
} fileInfo;

HINSTANCE dynamicLibrary;
HANDLE completedReader;
HANDLE stopReader;
HANDLE completedWriter;

DWORD WINAPI threadWriter(PVOID);
DWORD WINAPI threadReader(PVOID);

HANDLE createEvents();
void closeEvents(HANDLE);

DWORD WINAPI threadWriter(PVOID path)
{
	HANDLE outFileHandle = CreateFile((const char*)path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	BOOL(*Write)(FileInfo*) = (BOOL(*)(FileInfo*))GetProcAddress(dynamicLibrary, "writeToFile");
	HANDLE eventsOfReadAndStopRead[2] = { completedReader, stopReader };
	int stopEvent = 1;
	while (true)
	{
		int currentEvent = WaitForMultipleObjects(2, eventsOfReadAndStopRead, FALSE, INFINITE) - WAIT_OBJECT_0;
		if (currentEvent == stopEvent) break;
		fileInfo.headerOfFile = outFileHandle;
		(Write)(&fileInfo);
		SetEvent(completedWriter);
	}
	CloseHandle(outFileHandle);
	return EXIT_SUCCESS;
}

DWORD WINAPI threadReader(PVOID path)
{
	WIN32_FIND_DATA findFileData;
	HANDLE readFileHandle = NULL;
	BOOL resultofRead = false;
	string folder(((const char*)path));
	folder.append("\\");
	string fileMask = folder + "*.txt";
	char readFilePath[MAX_PATH];
	HANDLE findHandle = FindFirstFile(fileMask.c_str(), &findFileData);
	BOOL(*Read)(FileInfo*) = (BOOL(*)(FileInfo*))GetProcAddress(dynamicLibrary, "readFromFile");

	if (findHandle == INVALID_HANDLE_VALUE)
		return EXIT_FAILURE;
	while (true)
	{
		WaitForSingleObject(completedWriter, INFINITE);
		if (resultofRead == false)
		{
			fileInfo.positionInFile = 0;
			strcpy_s(readFilePath, folder.c_str());
			strcat_s(readFilePath, findFileData.cFileName);
			readFileHandle = CreateFile(readFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		}
		fileInfo.headerOfFile = readFileHandle;
		resultofRead = (Read)(&fileInfo);
		if (!resultofRead && GetLastError() == ERROR_HANDLE_EOF)
		{
			if (FindNextFile(findHandle, &findFileData))
			{
				CloseHandle(readFileHandle);
				SetEvent(completedWriter);
				continue;
			}
			else break;
		}

		SetEvent(completedReader);
	}
	FindClose(findHandle);
	CloseHandle(readFileHandle);
	SetEvent(stopReader);
	return EXIT_SUCCESS;
}

void closeEvents(HANDLE eventHandler)
{
	CloseHandle(completedWriter);
	CloseHandle(completedReader);
	CloseHandle(stopReader);
	CloseHandle(eventHandler);
}

HANDLE createEvents()
{
	HANDLE eventHandler = CreateEvent(NULL, FALSE, TRUE, TEXT("eventMain")); ;
	completedWriter = CreateEvent(NULL, FALSE, TRUE, NULL);
	completedReader = CreateEvent(NULL, FALSE, FALSE, NULL);
	stopReader = CreateEvent(NULL, TRUE, FALSE, NULL);

	return eventHandler;
}

