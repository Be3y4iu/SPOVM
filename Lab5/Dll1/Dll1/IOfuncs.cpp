#include "stdafx.h"  
#include <windows.h>
#include "stdio.h"
#include "string.h"

#ifdef __cplusplus 
extern "C" 
{
#endif

	struct FileInfo
	{
		HANDLE headerOfFile;
		DWORD numberOfBytes;
		CHAR  buffer[100];
		DWORD  positionInFile;
		DWORD  positionOutFile;
		OVERLAPPED overlapped;
	};
	__declspec(dllexport) BOOL writeToFile(FileInfo* file)
	{
		BOOL resultOfWrite;
		DWORD numOfBytesTrasferred;

		file->overlapped.Offset = file->positionOutFile;
		WriteFile(file->headerOfFile, file->buffer, file->overlapped.InternalHigh, NULL, &file->overlapped);
		WaitForSingleObject(file->overlapped.hEvent, INFINITE);
		resultOfWrite = GetOverlappedResult(file->headerOfFile, &file->overlapped, &numOfBytesTrasferred, FALSE);
		if (resultOfWrite) file->positionOutFile = file->positionOutFile + numOfBytesTrasferred;
		return resultOfWrite;
	}
	__declspec(dllexport) BOOL readFromFile(FileInfo* file)
	{
		BOOL resultOfRead;
		DWORD numOfBytesTrasferred;

		file->overlapped.Offset = file->positionInFile;
		ReadFile(file->headerOfFile, file->buffer, file->numberOfBytes, NULL, &file->overlapped);
		WaitForSingleObject(file->overlapped.hEvent, INFINITE);
		resultOfRead = GetOverlappedResult(file->headerOfFile, &file->overlapped, &numOfBytesTrasferred, FALSE);
		if (resultOfRead) file->positionInFile = file->positionInFile + numOfBytesTrasferred;
		return resultOfRead;
	}
	
#ifdef __cplusplus
}
#endif