// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <string>

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

struct header
{
	int addr;
	int size;
};

HANDLE mutex = CreateMutex(NULL, false, L"Mutex");


extern "C" {

	__declspec(dllexport) std::wstring mapreceive(header& h)
	{
		WaitForSingleObject(mutex, INFINITE);
		HANDLE hFile = CreateFile(L"filemap.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);

		HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, sizeof(header), L"MyMap");

		LPVOID buff = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(header));

		h = *((header*)buff);
		UnmapViewOfFile(buff);
		CloseHandle(hFileMap);

		int n = h.size + sizeof(header);
		hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, n, L"MyMap");
		buff = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, n);

		std::wstring s((wchar_t*)((BYTE*)buff + sizeof(header)), h.size / 2);

		UnmapViewOfFile(buff);
		CloseHandle(hFileMap);

		CloseHandle(hFile);
		ReleaseMutex(mutex);
		return s;
	}

	__declspec(dllexport) void mapsend(int addr, const wchar_t* str)
	{
		WaitForSingleObject(mutex, INFINITE);
		header h = { addr, int(wcslen(str) * 2) };
		HANDLE hFile = CreateFile(L"filemap.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);

		HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, h.size + sizeof(header), L"MyMap");
		BYTE* buff = (BYTE*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, h.size + sizeof(header));

		memcpy(buff, &h, sizeof(header));
		memcpy(buff + sizeof(header), str, h.size);

		UnmapViewOfFile(buff);

		CloseHandle(hFileMap);
		CloseHandle(hFile);
		ReleaseMutex(mutex);
	}
}