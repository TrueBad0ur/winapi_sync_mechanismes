#include <iostream>
#include <Windows.h>

using namespace std;

DWORD volatile global = 0;
CRITICAL_SECTION critical;
HANDLE mutex;

void forThread() {
	/*
	for (int i = 0; i < 10000; i++) {
		EnterCriticalSection(&critical);
		global++;
		LeaveCriticalSection(&critical);
	}
	*/

	for (int i = 0; i < 10000; i++) {
		WaitForSingleObject(mutex, INFINITE);
		global++;
		ReleaseMutex(mutex);
	}
}

int main() {
	for (int j = 0; j < 10000; j++) {
		mutex = CreateMutex(0, 0, 0);

		HANDLE hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)forThread, NULL, 0, NULL);
		HANDLE hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)forThread, NULL, 0, NULL);
		HANDLE hThread3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)forThread, NULL, 0, NULL);

		WaitForSingleObject(hThread1, INFINITE);
		WaitForSingleObject(hThread2, INFINITE);
		WaitForSingleObject(hThread3, INFINITE);

		CloseHandle(hThread1);
		CloseHandle(hThread2);
		CloseHandle(hThread3);

		CloseHandle(mutex);

		//cout << global;
		if (global != 30000)
			cout << "Global: " << global << endl;

		global = 0;
	}


	return 0;
}