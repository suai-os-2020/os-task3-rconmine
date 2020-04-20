#include "lab3.h"
#include <windows.h>
//#include <stdio.h>
#include <iostream>
using namespace std;

#define MAX_SEM_COUNT 1
#define THREADCOUNT 18

HANDLE ghSemaphore;
HANDLE SemF, SemH, SemG;

DWORD WINAPI ThreadProc(LPVOID);
//
// lab3 code should be located here!
//


unsigned int lab3_thread_graph_id()
{
	return 17;
}

const char* lab3_unsynchronized_threads()
{
	return "bcd";
}

const char* lab3_sequential_threads()
{
	return "fgh";
}


DWORD WINAPI ThreadProcF(LPVOID lpParam)
{

	// lpParam not used in this example
	// �������� (��������) lpParam � ���� ������� �� ������������
	UNREFERENCED_PARAMETER(lpParam);

	DWORD dwWaitResult;
	//BOOL bContinue = TRUE;

	for (int i = 0; i < 3; i++) {
		dwWaitResult = WaitForSingleObject(SemF, INFINITE);
		switch (dwWaitResult)
		{
		case WAIT_OBJECT_0:
			cout << 'f';
			
			if (!ReleaseSemaphore(SemG, 1, NULL))
			{
				cout << "ReleaseSemaphore error: \n" << GetLastError();
			}
			computation();
			break;
		}
	}
	return 0;
}

DWORD WINAPI ThreadProcG(LPVOID lpParam)
{

	// lpParam not used in this example
	// �������� (��������) lpParam � ���� ������� �� ������������
	UNREFERENCED_PARAMETER(lpParam);

	DWORD dwWaitResult;
	//BOOL bContinue = TRUE;

	for (int i = 0; i < 3; i++) {
		dwWaitResult = WaitForSingleObject(SemG, INFINITE);
		switch (dwWaitResult)
		{
		case WAIT_OBJECT_0:
			cout << 'g';
			
			if (!ReleaseSemaphore(SemH, 1, NULL))
			{
				cout << "ReleaseSemaphore error: \n" << GetLastError();
			}
			computation();
			break;
		}
	}
	return 0;
}

DWORD WINAPI ThreadProcH(LPVOID lpParam)
{

	// lpParam not used in this example
	// �������� (��������) lpParam � ���� ������� �� ������������
	UNREFERENCED_PARAMETER(lpParam);

	DWORD dwWaitResult;
	//BOOL bContinue = TRUE;

	for (int i = 0; i < 3; i++) {
		dwWaitResult = WaitForSingleObject(SemH, INFINITE);
		switch (dwWaitResult)
		{
		case WAIT_OBJECT_0:
			cout << 'h';
			
			if (!ReleaseSemaphore(SemF, 1, NULL))
			{
				cout << "ReleaseSemaphore error: \n" << GetLastError();
			}
			computation();
			break;
		}
	}
	return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{

	// lpParam not used in this example
	// �������� (��������) lpParam � ���� ������� �� ������������
	UNREFERENCED_PARAMETER(lpParam);
	//char lt = (char)lpParam;

	DWORD dwWaitResult, dwWaitResult1;
	//BOOL bContinue = TRUE;

	for (int i = 0; i < 3; i++) {
		dwWaitResult = WaitForSingleObject(ghSemaphore, INFINITE);
		switch (dwWaitResult)
		{
		case WAIT_OBJECT_0:
			cout << (char)lpParam;
			if (!ReleaseSemaphore(ghSemaphore, 1, NULL))
			{
				cout << "ReleaseSemaphore error: \n" << GetLastError();
			}
			computation();
			break;
		}
	}
	return 0;
}

int lab3_init()
{
	HANDLE aThread[THREADCOUNT];
	DWORD ThreadID;
	int i;

	// Create a semaphore with initial and max counts of MAX_SEM_COUNT
	// �������� �������� � ��������� � ������������ ���������� �������� ������� MAX_SEM_COUNT
	ghSemaphore = CreateSemaphore(
		NULL,           // default security attributes // ��������� ������������ �� ���������
		MAX_SEM_COUNT,  // initial count               // ��������� �������� ��������
		MAX_SEM_COUNT,  // maximum count               // ������������ �������� ��������
		NULL);          // unnamed semaphore           // ���������� �������

	if (ghSemaphore == NULL)
	{
		cout << "CreateSemaphore error: \n" << GetLastError();
		return 1;
	}

	// Create worker threads
	// ������� ������

	aThread[0] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'a',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[0] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	aThread[1] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'b',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[1] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	aThread[2] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'd',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[2] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	// Wait for all threads to terminate
	// ���� ���������� ���� �������

	WaitForSingleObject(aThread[0], INFINITE);
	WaitForSingleObject(aThread[1], INFINITE);
	WaitForSingleObject(aThread[2], INFINITE);
	// Close thread and semaphore handles
	// ��������� ������ � ���������� ��������
	//CloseHandle(aThread[0]);
	CloseHandle(aThread[0]);
	CloseHandle(aThread[1]);
	CloseHandle(aThread[2]);

	aThread[3] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'b',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[3] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	aThread[4] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'd',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[4] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	aThread[5] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'c',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[5] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	WaitForSingleObject(aThread[3], INFINITE);
	WaitForSingleObject(aThread[4], INFINITE);
	WaitForSingleObject(aThread[5], INFINITE);

	CloseHandle(aThread[3]);
	CloseHandle(aThread[4]);
	CloseHandle(aThread[5]);


	aThread[6] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'h',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[6] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	aThread[7] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'f',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[7] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	aThread[8] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'e',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[8] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	aThread[9] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'd',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[9] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	WaitForSingleObject(aThread[6], INFINITE);
	WaitForSingleObject(aThread[7], INFINITE);
	WaitForSingleObject(aThread[8], INFINITE);
	WaitForSingleObject(aThread[9], INFINITE);

	CloseHandle(aThread[6]);
	CloseHandle(aThread[7]);
	CloseHandle(aThread[8]);
	CloseHandle(aThread[9]);


	//SEQUENTAL

	SemF = CreateSemaphore(NULL, 1, 1, NULL);

	if (SemF == NULL)
	{
		cout << "CreateSemaphore error: \n" << GetLastError();
		return 1;
	}

	SemG = CreateSemaphore(NULL, 0, 1, NULL);

	if (SemG == NULL)
	{
		cout << "CreateSemaphore error: \n" << GetLastError();
		return 1;
	}

	SemH = CreateSemaphore(NULL, 0, 1, NULL);

	if (SemH == NULL)
	{
		cout << "CreateSemaphore error: \n" << GetLastError();
		return 1;
	}

	aThread[10] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProcF,
		NULL,       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[10] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	aThread[11] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProcG,
		NULL,       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[11] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	aThread[12] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProcH,
		NULL,       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[12] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	WaitForSingleObject(aThread[10], INFINITE);
	WaitForSingleObject(aThread[11], INFINITE);
	WaitForSingleObject(aThread[12], INFINITE);

	CloseHandle(aThread[10]);
	CloseHandle(aThread[11]);
	CloseHandle(aThread[12]);

	aThread[13] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'h',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[13] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	aThread[14] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'i',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[14] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	aThread[15] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'k',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[15] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	WaitForSingleObject(aThread[13], INFINITE);
	WaitForSingleObject(aThread[14], INFINITE);
	WaitForSingleObject(aThread[15], INFINITE);

	CloseHandle(aThread[13]);
	CloseHandle(aThread[14]);
	CloseHandle(aThread[15]);

	aThread[16] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'k',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[16] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	aThread[17] = CreateThread(
		NULL,       // default security attributes   // ��������� ������������ �� ���������
		0,          // default stack size            // ������ ����� �� ���������
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID) 'm',       // no thread function arguments  // ����������� ��������� ��� �������-���� ������
		0,          // default creation flags        // ����� �������� ������ �� ���������
		&ThreadID); // receive thread identifier     // ������������� ������

	if (aThread[17] == NULL)
	{
		cout << "CreateThread error: \n" << GetLastError();
		return 1;
	}

	WaitForSingleObject(aThread[16], INFINITE);
	WaitForSingleObject(aThread[17], INFINITE);

	CloseHandle(aThread[16]);
	CloseHandle(aThread[17]);

	for (i = 0; i < THREADCOUNT; i++)
		;


	CloseHandle(ghSemaphore);

	return 0;

}
