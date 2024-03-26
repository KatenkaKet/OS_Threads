#include <iostream>
#include <algorithm>
#include <cstring>
#include <Windows.h>

using namespace std;
//Задание:
//1.	Разработать и отладить программу, выполняющую следующие функции :
//1.1.Заполнение некоторого массива случайными числами;
//1.2.Создание в основном потоке двух дополнительных потоков, выполняющих 
// сортировку массива двумя различными методами и сохраняющих результаты 
// в отдельных массивах;
//1.3.Основной поток ожидает завершения сортировки, сравнивает результаты 
// сортировки, получает значения временных характеристик дополнительных потоков, 
// основного потока и всего процесса, результаты сохраняет в файл или выводит 
// на экран
//2.	Используя реализацию п. 1, создать программу с учётом следующих требований :
//2.1.После создания дополнительных потоков, изменить их базовые приоритеты 
// в максимально большую сторону для одного и в меньшую для другого;
//2.2.Сравнить полученные временные характеристики выполнения потоков 
// с результатами, полученными п. 1.3;
//2.3.Выполнить п. 2.1 и 2.2 поменяв более приоритетный и менее приоритетный 
// потоки местами.
//3.	Дополнить программу, разработанную согласно п.2, блокированием 
// динамических изменений приоритетов потоков после их запуска и сравнить 
// полученные временные характеристики выполнения потоков с полученными 
// ранее результатами.
//4.	По результатам работы заполнить таблицу и построить временные диаграммы, 
// схематично иллюстрирующие псевдопараллельное выполнение потоков при различных 
// уровнях приоритетов.

//HANDLE CreateThread(
//	[in, optional]  LPSECURITY_ATTRIBUTES   lpThreadAttributes,
//	[in]            SIZE_T                  dwStackSize,
//	[in]            LPTHREAD_START_ROUTINE  lpStartAddress,
//	[in, optional]  __drv_aliasesMem LPVOID lpParameter,
//	[in]            DWORD                   dwCreationFlags,
//	[out, optional] LPDWORD                 lpThreadId
//);

void FillRand(int* A, int N, int min, int max);
void print_array(int* A, int N);
void bubbleSort(int list[], int listLength);
int partition(int* A, int start, int end); 
void quicksort(int* A, int start, int end);
int Less(int A, int B);
int CheckSort(int* A, int N);
DWORD WINAPI bubbleSortShell(LPVOID lpParameter);
DWORD WINAPI quicksortShell(LPVOID lpParameter);


int N = 100000;
int minn = 0;
int maxn = 10000000;
int *B = new int[N];
int *C = new int[N];



int main()
{
	
	FillRand(B, N, minn, maxn);
	memcpy(C, B, N*sizeof(int));

	DWORD th_id1, th_id2;
	cout << "\tEqual" << endl;
	HANDLE thread1 = CreateThread(NULL, 0, bubbleSortShell, C, CREATE_SUSPENDED, &th_id1);
	HANDLE thread2 = CreateThread(NULL, 0, quicksortShell, B, CREATE_SUSPENDED, &th_id2);

	SetThreadPriorityBoost(thread1, false);
	SetThreadPriorityBoost(thread2, false);

	SetThreadPriority(thread1, THREAD_PRIORITY_NORMAL);
	SetThreadPriority(thread2, THREAD_PRIORITY_NORMAL);

	ResumeThread(thread1);
	ResumeThread(thread2);

	WaitForSingleObject(thread1, INFINITE);
	WaitForSingleObject(thread2, INFINITE);

	FILETIME CreationTime, ExitTime, KernelTime, UserTime;
	if (GetThreadTimes(thread1, &CreationTime, &ExitTime, &KernelTime, &UserTime)) {
		SYSTEMTIME SystemTime, SystemTime1, SystemTime2;
		cout << "\t1st thread" << endl;
		if (FileTimeToSystemTime(&CreationTime, &SystemTime)) {
			cout << "Created date: " << SystemTime.wYear << '.' << SystemTime.wMonth << '.' << SystemTime.wDay << endl;
			cout << "Created time: " << SystemTime.wHour << ':' << SystemTime.wMinute << ':' << SystemTime.wSecond << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
		if (FileTimeToSystemTime(&KernelTime, &SystemTime1)) {
			cout << "KernelTime date: " << SystemTime1.wYear << '.' << SystemTime1.wMonth << '.' << SystemTime1.wDay << endl;
			cout << "KernelTime time: " << SystemTime1.wHour << ':' << SystemTime1.wMinute << ':' << SystemTime1.wSecond << ':' << SystemTime1.wMilliseconds << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
		if (FileTimeToSystemTime(&UserTime, &SystemTime2)) {
			cout << "UserTime date: " << SystemTime2.wYear << '.' << SystemTime2.wMonth << '.' << SystemTime2.wDay << endl;
			cout << "UserTime time: " << SystemTime2.wHour << ':' << SystemTime2.wMinute << ':' << SystemTime2.wSecond << ':' << SystemTime2.wMilliseconds << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
	}
	else {
		cout << "GetProcessTimes faild. Error: " << GetLastError() << endl;
		return 1;
	}

	if (GetThreadTimes(thread2, &CreationTime, &ExitTime, &KernelTime, &UserTime)) {
		cout << "\t2nd thread" << endl;
		SYSTEMTIME SystemTime, SystemTime1, SystemTime2;
		if (FileTimeToSystemTime(&CreationTime, &SystemTime)) {
			cout << "Created date: " << SystemTime.wYear << '.' << SystemTime.wMonth << '.' << SystemTime.wDay << endl;
			cout << "Created time: " << SystemTime.wHour << ':' << SystemTime.wMinute << ':' << SystemTime.wSecond << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
		if (FileTimeToSystemTime(&KernelTime, &SystemTime1)) {
			cout << "KernelTime date: " << SystemTime1.wYear << '.' << SystemTime1.wMonth << '.' << SystemTime1.wDay << endl;
			cout << "KernelTime time: " << SystemTime1.wHour << ':' << SystemTime1.wMinute << ':' << SystemTime1.wSecond << ':' << SystemTime1.wMilliseconds << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
		if (FileTimeToSystemTime(&UserTime, &SystemTime2)) {
			cout << "UserTime date: " << SystemTime2.wYear << '.' << SystemTime2.wMonth << '.' << SystemTime2.wDay << endl;
			cout << "UserTime time: " << SystemTime2.wHour << ':' << SystemTime2.wMinute << ':' << SystemTime2.wSecond << ':' << SystemTime2.wMilliseconds << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
	}
	else {
		cout << "GetProcessTimes faild. Error: " << GetLastError() << endl;
		return 1;
	}
	//////////////////////////////////////////////////////////////////////////////////////
	cout << "\tlow/high" << endl;
	FillRand(B, N, minn, maxn);
	memcpy(C, B, N * sizeof(int));

	HANDLE thread3 = CreateThread(NULL, 0, bubbleSortShell, C, CREATE_SUSPENDED, &th_id1);
	HANDLE thread4 = CreateThread(NULL, 0, quicksortShell, B, CREATE_SUSPENDED, &th_id2);

	SetThreadPriorityBoost(thread3, false);
	SetThreadPriorityBoost(thread4, false);

	SetThreadPriority(thread3, THREAD_PRIORITY_LOWEST);
	SetThreadPriority(thread4, THREAD_PRIORITY_HIGHEST);

	ResumeThread(thread3);
	ResumeThread(thread4);

	WaitForSingleObject(thread3, INFINITE);
	WaitForSingleObject(thread4, INFINITE);


	if (GetThreadTimes(thread3, &CreationTime, &ExitTime, &KernelTime, &UserTime)) {
		SYSTEMTIME SystemTime, SystemTime1, SystemTime2;
		cout << "\t1st thread" << endl;
		if (FileTimeToSystemTime(&CreationTime, &SystemTime)) {
			cout << "Created date: " << SystemTime.wYear << '.' << SystemTime.wMonth << '.' << SystemTime.wDay << endl;
			cout << "Created time: " << SystemTime.wHour << ':' << SystemTime.wMinute << ':' << SystemTime.wSecond << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
		if (FileTimeToSystemTime(&KernelTime, &SystemTime1)) {
			cout << "KernelTime date: " << SystemTime1.wYear << '.' << SystemTime1.wMonth << '.' << SystemTime1.wDay << endl;
			cout << "KernelTime time: " << SystemTime1.wHour << ':' << SystemTime1.wMinute << ':' << SystemTime1.wSecond << ':' << SystemTime1.wMilliseconds << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
		if (FileTimeToSystemTime(&UserTime, &SystemTime2)) {
			cout << "UserTime date: " << SystemTime2.wYear << '.' << SystemTime2.wMonth << '.' << SystemTime2.wDay << endl;
			cout << "UserTime time: " << SystemTime2.wHour << ':' << SystemTime2.wMinute << ':' << SystemTime2.wSecond << ':' << SystemTime2.wMilliseconds << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
	}
	else {
		cout << "GetProcessTimes faild. Error: " << GetLastError() << endl;
		return 1;
	}

	if (GetThreadTimes(thread4, &CreationTime, &ExitTime, &KernelTime, &UserTime)) {
		cout << "\t2nd thread" << endl;
		SYSTEMTIME SystemTime, SystemTime1, SystemTime2;
		if (FileTimeToSystemTime(&CreationTime, &SystemTime)) {
			cout << "Created date: " << SystemTime.wYear << '.' << SystemTime.wMonth << '.' << SystemTime.wDay << endl;
			cout << "Created time: " << SystemTime.wHour << ':' << SystemTime.wMinute << ':' << SystemTime.wSecond << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
		if (FileTimeToSystemTime(&KernelTime, &SystemTime1)) {
			cout << "KernelTime date: " << SystemTime1.wYear << '.' << SystemTime1.wMonth << '.' << SystemTime1.wDay << endl;
			cout << "KernelTime time: " << SystemTime1.wHour << ':' << SystemTime1.wMinute << ':' << SystemTime1.wSecond << ':' << SystemTime1.wMilliseconds << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
		if (FileTimeToSystemTime(&UserTime, &SystemTime2)) {
			cout << "UserTime date: " << SystemTime2.wYear << '.' << SystemTime2.wMonth << '.' << SystemTime2.wDay << endl;
			cout << "UserTime time: " << SystemTime2.wHour << ':' << SystemTime2.wMinute << ':' << SystemTime2.wSecond << ':' << SystemTime2.wMilliseconds << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
	}
	else {
		cout << "GetProcessTimes faild. Error: " << GetLastError() << endl;
		return 1;
	}

	CloseHandle(thread3);
	CloseHandle(thread4);
	///////////////////////////////////////////////////////////////////////////////////////
	cout << "\thigh/low" << endl;
	FillRand(B, N, minn, maxn);
	memcpy(C, B, N * sizeof(int));

	HANDLE thread5 = CreateThread(NULL, 0, bubbleSortShell, C, CREATE_SUSPENDED, &th_id1);
	HANDLE thread6 = CreateThread(NULL, 0, quicksortShell, B, CREATE_SUSPENDED, &th_id2);

	SetThreadPriorityBoost(thread5, false);
	SetThreadPriorityBoost(thread6, false);

	SetThreadPriority(thread5, THREAD_PRIORITY_HIGHEST);
	SetThreadPriority(thread6, THREAD_PRIORITY_LOWEST);

	ResumeThread(thread5);
	ResumeThread(thread6);

	WaitForSingleObject(thread5, INFINITE);
	WaitForSingleObject(thread6, INFINITE);

	if (GetThreadTimes(thread5, &CreationTime, &ExitTime, &KernelTime, &UserTime)) {
		SYSTEMTIME SystemTime, SystemTime1, SystemTime2;
		cout << "\t1st thread" << endl;
		if (FileTimeToSystemTime(&CreationTime, &SystemTime)) {
			cout << "Created date: " << SystemTime.wYear << '.' << SystemTime.wMonth << '.' << SystemTime.wDay << endl;
			cout << "Created time: " << SystemTime.wHour << ':' << SystemTime.wMinute << ':' << SystemTime.wSecond << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
		if (FileTimeToSystemTime(&KernelTime, &SystemTime1)) {
			cout << "KernelTime date: " << SystemTime1.wYear << '.' << SystemTime1.wMonth << '.' << SystemTime1.wDay << endl;
			cout << "KernelTime time: " << SystemTime1.wHour << ':' << SystemTime1.wMinute << ':' << SystemTime1.wSecond << ':' << SystemTime1.wMilliseconds << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
		if (FileTimeToSystemTime(&UserTime, &SystemTime2)) {
			cout << "UserTime date: " << SystemTime2.wYear << '.' << SystemTime2.wMonth << '.' << SystemTime2.wDay << endl;
			cout << "UserTime time: " << SystemTime2.wHour << ':' << SystemTime2.wMinute << ':' << SystemTime2.wSecond << ':' << SystemTime2.wMilliseconds << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
	}
	else {
		cout << "GetProcessTimes faild. Error: " << GetLastError() << endl;
		return 1;
	}

	if (GetThreadTimes(thread6, &CreationTime, &ExitTime, &KernelTime, &UserTime)) {
		cout << "\t2nd thread" << endl;
		SYSTEMTIME SystemTime, SystemTime1, SystemTime2;
		if (FileTimeToSystemTime(&CreationTime, &SystemTime)) {
			cout << "Created date: " << SystemTime.wYear << '.' << SystemTime.wMonth << '.' << SystemTime.wDay << endl;
			cout << "Created time: " << SystemTime.wHour << ':' << SystemTime.wMinute << ':' << SystemTime.wSecond << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
		if (FileTimeToSystemTime(&KernelTime, &SystemTime1)) {
			cout << "KernelTime date: " << SystemTime1.wYear << '.' << SystemTime1.wMonth << '.' << SystemTime1.wDay << endl;
			cout << "KernelTime time: " << SystemTime1.wHour << ':' << SystemTime1.wMinute << ':' << SystemTime1.wSecond << ':' << SystemTime1.wMilliseconds << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
		if (FileTimeToSystemTime(&UserTime, &SystemTime2)) {
			cout << "UserTime date: " << SystemTime2.wYear << '.' << SystemTime2.wMonth << '.' << SystemTime2.wDay << endl;
			cout << "UserTime time: " << SystemTime2.wHour << ':' << SystemTime2.wMinute << ':' << SystemTime2.wSecond << ':' << SystemTime2.wMilliseconds << endl;
		}
		else {
			cout << "Not information. Error: " << GetLastError() << endl;
		}
	}
	else {
		cout << "GetProcessTimes faild. Error: " << GetLastError() << endl;
		return 1;
	}

	
	CloseHandle(thread5);
	CloseHandle(thread6);
	return 0;
}

void print_array(int* A, int N) {
	for (int i = 0; i < N; i++) {
		cout << A[i] << ' ';
	}
	cout << endl;
}

void FillRand(int* A, int N, int min, int max) {
	for (int i = 0; i < N; i++) {
		A[i] = ((rand() << 15) | rand()) % (max - min) + min;
	}
}

void bubbleSort(int list[], int listLength)
{
	while (listLength--)
	{
		bool swapped = false;

		for (int i = 0; i < listLength; i++)
		{
			if (list[i] > list[i + 1])
			{
				swap(list[i], list[i + 1]);
				swapped = true;
			}
		}

		if (swapped == false)
			break;
	}
}

int Less(int A, int B) {
	return A < B;
}

int partition(int* A, int start, int end)
{
	int V = A[end];
	int i = start - 1;
	int j = end;
	for (;;) {
		//    	while(V> A[++i]);
		while (Less(A[++i], V));
		while (Less(V, A[--j])) if (j == start) break;
		if (i >= j) break;
		swap(A[i], A[j]);
	}
	swap(A[end], A[i]);
	return i;
}

void quicksort(int* A, int start, int end)
{
	int P;
	if (end - start <= 0) return;

	swap(A[(start + end) / 2], A[end - 1]);
	if (Less(A[end - 1], A[start])) swap(A[start], A[end - 1]);
	if (Less(A[end], A[start])) swap(A[start], A[end]);
	if (Less(A[end], A[end - 1])) swap(A[end - 1], A[end]);

	P = partition(A, start, end);
	quicksort(A, start, P - 1);
	quicksort(A, P + 1, end);
}

int CheckSort(int* A, int N) {
	for (int i = 1; i < N; i++) {
		if (A[i] < A[i - 1]) {
			return 1;
		}
	}
	return 0;
}


DWORD WINAPI bubbleSortShell(LPVOID lpParameter) {
	bubbleSort((int *)lpParameter, N);
	return 0;
}

DWORD WINAPI quicksortShell(LPVOID lpParameter) {
	quicksort((int *)lpParameter, 0, N);
	return 0;
}
