#include "FileThreads.h"

int main()
{
	HANDLE readAndWriteThreads[2];
	HANDLE handleEvent = createEvents();
	fileInfo.Overlapped.Offset = 0;
	fileInfo.Overlapped.OffsetHigh = 0;
	fileInfo.Overlapped.hEvent = handleEvent;
	fileInfo.positionOutFile = 0;
	fileInfo.numberOfBytes = sizeof(fileInfo.buffer);

	dynamicLibrary = LoadLibrary("C:/Users/Lesha/Desktop/Lab5/Dll1/Debug/DLL1.dll");
	if (dynamicLibrary == NULL)
	{
		cout << "Unable to load library!" << endl;
		return 0;
	}
	cout << "Start...\n";
	readAndWriteThreads[0] = CreateThread(NULL, 0, threadWriter, (LPVOID)("result.txt"), 0, NULL);
	readAndWriteThreads[1] = CreateThread(NULL, 0, threadReader, (LPVOID)("C:/Users/Lesha/Desktop/lab5/Files"), 0, NULL);

	WaitForMultipleObjects(2, readAndWriteThreads, TRUE, INFINITE);
	system("pause");
	CloseHandle(readAndWriteThreads[0]);
	CloseHandle(readAndWriteThreads[1]);
	closeEvents(handleEvent);
	FreeLibrary(dynamicLibrary);
	cout << "Complete!";
	cin.get();
	return 0;
}