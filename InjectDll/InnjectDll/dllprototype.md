~~~c++
// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "framework.h"
#include "detours.h"
#include "cstdio"
#include "stdarg.h"
#include "windows.h"
#include <iostream>
#include <mutex>

#pragma comment(lib, "detours.lib")

SYSTEMTIME st;

char Temp[256];

char hBuf[256];

//HANDLE HeapAll;
//bool HeapAllocFlag = true;
int heapCreateTime = 0, heapFreeTime = 0, heapDestroyTime = 0;
//int heapAllocTime = 0;

char tempR[70];

#pragma data_seg("MySeg")
char targetPath[256] = { };
#pragma data_seg()
#pragma comment(linker, "/section:MySeg,RWS")

std::mutex mtx;

extern "C" __declspec(dllexport)void GetTarget(char* inputPath)
{
    mtx.lock();
    strcpy_s(targetPath, 256, inputPath);
    mtx.unlock();
}

extern "C" __declspec(dllexport)void AimTarget(char* aimPath)
{
    mtx.lock();
    strcpy_s(aimPath, 256, targetPath);
    mtx.unlock();
}

// OutputDebugView查看
char dbgOutput[256], dbgTemp[256];
void WriteDbg(const char* content)
{
    va_list args;
    va_start(args, content);
    _vsnprintf_s(dbgOutput, sizeof(dbgOutput) - 1, content, args);
    va_end(args);
    OutputDebugStringA(dbgOutput);
}
void ClearDbg(char* dbg)
{
    int counter = 0;
    for (; counter < 256; counter++)
    {
        dbg[counter] = '\0';
    }
}
void ClearTempR(char* tempr)
{
    int counter = 0;
    for (; counter < 70; counter++)
    {
        tempr[counter] = '\0';
    }
}

//定义和引入需要hook的函数，和替换的函数
static int(WINAPI* OldMeesageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;
static int(WINAPI* OldMeesageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;

extern "C" __declspec(dllexport)int WINAPI NewMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType)
{
    //printf("\n\n**********************\n");
    WriteDbg("\n\n**********************\n");
    ClearDbg(dbgOutput);
    OutputDebugStringA(lpText);
    OutputDebugStringA(lpCaption);
    //printf("MessageBoxA Hooked\n");
    WriteDbg("MessageBoxA Hooked\n");
    ClearDbg(dbgOutput);
    GetLocalTime(&st);
    //printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //printf("******************\n\n");
    WriteDbg("******************\n\n");
    ClearDbg(dbgOutput);

    return OldMeesageBoxA(NULL, "new MessageBoxA", "Hooked", MB_OK);
}

extern "C" __declspec(dllexport)int WINAPI NewMessageBoxW(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType)
{
    //printf("\n\n**********************\n");
    WriteDbg("\n\n**********************\n");
    ClearDbg(dbgOutput);
    OutputDebugStringW(lpText);
    OutputDebugStringW(lpCaption);
    //printf("MessageBoxW Hooked\n");
    WriteDbg("MessageBoxW Hooked\n");
    ClearDbg(dbgOutput);
    GetLocalTime(&st);
    //printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //printf("******************\n\n");
    WriteDbg("******************\n\n");
    ClearDbg(dbgOutput);

    return OldMeesageBoxW(NULL, L"new MessageBoxW", L"Hooked", MB_OK);
}

//文件操作 OpenFile CreateFile

static HANDLE(WINAPI* OldCreateFile)(
    LPCTSTR lpFileName,                          // 文件名
    DWORD dwDesiredAccess,                       // 访问模式
    DWORD dwShareMode,                           // 共享模式
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,  // 安全属性（也即销毁方式）
    DWORD dwCreationDisposition,                 // how to create
    DWORD dwFlagsAndAttributes,                  // 文件属性
    HANDLE hTemplateFile                         // 模板文件句柄
    ) = CreateFile;

extern "C" __declspec(dllexport)HANDLE WINAPI NewCreateFile(
    LPCTSTR lpFileName,                          // 文件名
    DWORD dwDesiredAccess,                       // 访问模式
    DWORD dwShareMode,                           // 共享模式
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,  // 安全属性（也即销毁方式）
    DWORD dwCreationDisposition,                 // how to create
    DWORD dwFlagsAndAttributes,                  // 文件属性
    HANDLE hTemplateFile                         // 模板文件句柄
)
{
    //printf("\n\n**********************\n");
    WriteDbg("\n\n**********************\n");
    ClearDbg(dbgOutput);
    //printf("CreateFile Hooked\n");
    WriteDbg("CreateFile Hooked\n");
    ClearDbg(dbgOutput);
    GetLocalTime(&st);
    //printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, pchar, num, NULL, FALSE);
    memset(Temp, 0, sizeof(Temp));

    //std::cout << "lpFileName : " << pchar << std::endl;
    sprintf_s(dbgTemp, "lpFileName : %s\n", pchar);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //std::cout << "dwDesiredAccess : 0x" << std::hex << dwDesiredAccess << std::endl;
    sprintf_s(dbgTemp, "dwDesiredAccess : %lu\n", dwDesiredAccess);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //std::cout << "dwShareMode : 0x" << std::hex << dwShareMode << std::endl;
    sprintf_s(dbgTemp, "dwShareMode : %lu\n", dwShareMode);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //std::cout << "lpSecurityAttributes : 0x" << std::hex << lpSecurityAttributes << std::endl;
    sprintf_s(dbgTemp, "lpSecurityAttributes : %p\n", lpSecurityAttributes);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //std::cout << "dwCreationDisposition : 0x" << std::hex << dwCreationDisposition << std::endl;
    sprintf_s(dbgTemp, "dwCreationDisposition : %lu\n", dwCreationDisposition);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //std::cout << "dwFlagsAndAttributes : 0x" << std::hex << dwFlagsAndAttributes << std::endl;
    sprintf_s(dbgTemp, "dwFlagsAndAttributes : %lu\n", dwFlagsAndAttributes);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //std::cout << "hTemplateFile : 0x" << std::hex << hTemplateFile << std::endl;
    sprintf_s(dbgTemp, "hTemplateFile : %p\n", hTemplateFile);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);

    //printf("******************\n\n");
    WriteDbg("******************\n\n");
    ClearDbg(dbgOutput);
    return OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

static BOOL(WINAPI* OldWriteFile)(
    HANDLE       hFile,
    LPCVOID      lpBuffer,
    DWORD        nNumberOfBytesToWrite,
    LPDWORD      lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped
    ) = WriteFile;

extern "C" __declspec(dllexport)BOOL WINAPI NewWriteFile(
    HANDLE       hFile,
    LPCVOID      lpBuffer,
    DWORD        nNumberOfBytesToWrite,
    LPDWORD      lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped
)
{
    if (GetFileType(hFile) == FILE_TYPE_DISK) {
        WriteDbg("\n\n**********************\n");
        ClearDbg(dbgOutput);
        WriteDbg("WriteFile Hooked\n");
        ClearDbg(dbgOutput);

        GetLocalTime(&st);
        sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);

        sprintf_s(dbgTemp, "hFile : %p", hFile);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);
        sprintf_s(dbgTemp, "lpBuffer : %p", lpBuffer);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);
        sprintf_s(dbgTemp, "nNumberOfBytesToWrite : %08X", nNumberOfBytesToWrite);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);
        sprintf_s(dbgTemp, "lpNumberOfBytesWritten : %p", lpNumberOfBytesWritten);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);
        sprintf_s(dbgTemp, "lpOverlapped : %p", lpOverlapped);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);

        //printf("******************\n\n");
        WriteDbg("******************\n\n");
        ClearDbg(dbgOutput);
    }
    return OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

static BOOL(WINAPI* OldReadFile)(
    HANDLE       hFile,
    LPVOID       lpBuffer,
    DWORD        nNumberOfBytesToRead,
    LPDWORD      lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped
    ) = ReadFile;

extern "C" __declspec(dllexport)BOOL WINAPI NewReadFile(
    HANDLE       hFile,
    LPVOID       lpBuffer,
    DWORD        nNumberOfBytesToRead,
    LPDWORD      lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped
)
{
    if (GetFileType(hFile) == FILE_TYPE_DISK) {
        WriteDbg("\n\n**********************\n");
        ClearDbg(dbgOutput);
        WriteDbg("ReadFile Hooked\n");
        ClearDbg(dbgOutput);

        GetLocalTime(&st);
        sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);

        sprintf_s(dbgTemp, "hFile : %p", hFile);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);
        sprintf_s(dbgTemp, "lpBuffer : %p", lpBuffer);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);
        sprintf_s(dbgTemp, "nNumberOfBytesToRead : %08X", nNumberOfBytesToRead);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);
        sprintf_s(dbgTemp, "lpNumberOfBytesRead : %p", lpNumberOfBytesRead);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);
        sprintf_s(dbgTemp, "lpOverlapped : %p", lpOverlapped);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);

        WriteDbg("******************\n\n");
        ClearDbg(dbgOutput);
    }

    return OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

static BOOL(WINAPI* OldCloseHandle)(
    HANDLE       hFile
    ) = CloseHandle;

extern "C" __declspec(dllexport)BOOL WINAPI NewCloseHandle(
    HANDLE       hFile
) {
    GetModuleFileNameA(NULL, hBuf, 256);
    if (strcmp(targetPath, hBuf) != 0)
    //if (strcmp("F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\TestApp.exe", hBuf) != 0)
    {
        ClearDbg(hBuf);
        return OldCloseHandle(hFile);
    }
    if (GetFileType(hFile) == FILE_TYPE_DISK) {
        WriteDbg("\n\n**********************\n");
        ClearDbg(dbgOutput);
        WriteDbg("CloseHandle Hooked\n");
        ClearDbg(dbgOutput);

        GetLocalTime(&st);
        sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);

        sprintf_s(dbgTemp, "hFile : %p", hFile);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);

        WriteDbg("******************\n\n");
        ClearDbg(dbgOutput);

        ClearDbg(hBuf);
    }
    return OldCloseHandle(hFile);
}

// 堆操作 HeapCreate HeapDestroy HeapAlloc HeapFree
static HANDLE(WINAPI* OldHeapCreate)(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) = HeapCreate;

extern "C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
{
    heapCreateTime++;
    HANDLE hHeap = OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
    //printf("\n\n**********************\n");
    WriteDbg("\n\n**********************\n");
    ClearDbg(dbgOutput);
    //printf("HeapCreate Hooked\n");
    WriteDbg("HeapCreate Hooked\n");
    ClearDbg(dbgOutput);
    GetLocalTime(&st);
    //printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //std::cout << "fIOoptions : " << fIOoptions << std::endl;
    sprintf_s(dbgTemp, "fIOoptions : %lu\n", fIOoptions);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //std::cout << "dwInitialSize : " << dwInitialSize << std::endl;
    sprintf_s(dbgTemp, "dwInitialSize : %zu\n", dwInitialSize);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //std::cout << "dwMaximumSize : " << dwMaximumSize << std::endl;
    sprintf_s(dbgTemp, "dwMaximumSize : %zu\n", dwMaximumSize);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //std::cout << "hHeap : " << hHeap << std::endl;
    sprintf_s(dbgTemp, "hHeap : %p\n", hHeap);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //printf("******************\n\n");
    WriteDbg("******************\n\n");
    ClearDbg(dbgOutput);
    return hHeap;
}

static BOOL(WINAPI* OldHeapDestroy)(HANDLE) = HeapDestroy;

extern "C" __declspec(dllexport)BOOL WINAPI NewHeapDestroy(HANDLE hHeap)
{
    //printf("\n\n**********************\n");
    WriteDbg("\n\n**********************\n");
    ClearDbg(dbgOutput);
    //printf("HeapDestroy Hooked\n");
    WriteDbg("HeapDestroy Hooked\n");
    ClearDbg(dbgOutput);
    GetLocalTime(&st);
    //printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //std::cout << "hHeap : " << hHeap << std::endl;
    sprintf_s(dbgTemp, "hHeap : %p\n", hHeap);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //printf("******************\n\n");
    WriteDbg("******************\n\n");
    ClearDbg(dbgOutput);

    heapDestroyTime++;
    if (heapDestroyTime > heapCreateTime) {
        MessageBox(0, L"An error occurred.\nInconsistent heap destruction times!", L"Error", 0);
    }

    return OldHeapDestroy(hHeap);
}

/*
static LPVOID(WINAPI* OldHeapAlloc)(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) = HeapAlloc;

extern "C" __declspec(dllexport) LPVOID WINAPI NewHeapAlloc(
    HANDLE hHeap,
    DWORD dwFlags,
    SIZE_T dwBytes
)
{
    if (strcmp(targetPath, hBuf) != 0)
    {
        ClearDbg(hBuf);
        return OldHeapAlloc(hHeap, dwFlags, dwBytes);
    }
    //HeapAll = CreateSemaphore(NULL, 1, 1, 0);
    //if (HeapAllocFlag == true) {
        //WaitForSingleObject(HeapAll, INFINITE);
        //HeapAllocFlag = false;
        
        //printf("\n\n**********************\n");
        WriteDbg("\n\n**********************\n");
        ClearDbg(dbgOutput);
        //printf("HeapAlloc Hooked\n");
        WriteDbg("HeapAlloc Hooked\n");
        ClearDbg(dbgOutput);
        GetLocalTime(&st);
        //printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);
        //printf("hHeap :  %p\n", hHeap);
        sprintf_s(dbgTemp, "hHeap : %p\n", hHeap);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);
        //printf("dwFlags :  %ul\n", dwFlags);
        sprintf_s(dbgTemp, "dwFlags :  %ul\n", dwFlags);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);
        //printf("dwBytes :  %ul Bytes\n", dwBytes);
        sprintf_s(dbgTemp, "dwBytes :  %ul Bytes\n", dwBytes);
        WriteDbg(dbgTemp);
        ClearDbg(dbgOutput);
        ClearDbg(dbgTemp);
        //printf("******************\n\n");
        WriteDbg("******************\n\n");
        ClearDbg(dbgOutput);

        //HeapAllocFlag = true;
        //ReleaseSemaphore(HeapAll, 1, NULL);
    //}
    //CloseHandle(HeapAll);

    return OldHeapAlloc(hHeap, dwFlags, dwBytes);
}
*/

static BOOL(WINAPI* OldHeapFree)(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem) = HeapFree;

extern "C" __declspec(dllexport) BOOL WINAPI NewHeapFree(
    HANDLE hHeap,
    DWORD dwFlags,
    LPVOID lpMem
)
{
    //printf("\n\n**********************\n");
    WriteDbg("\n\n**********************\n");
    ClearDbg(dbgOutput);
    //printf("HeapFree Hooked\n");
    WriteDbg("HeapFree Hooked\n");
    ClearDbg(dbgOutput);
    GetLocalTime(&st);
    //printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //std::cout << "hHeap : 0x" << std::hex << hHeap << std::endl;
    sprintf_s(dbgTemp, "hHeap : %p\n", hHeap);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //printf("dwFlags :  %ul\n", dwFlags);
    sprintf_s(dbgTemp, "dwFlags :  %ul\n", dwFlags);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //std::cout << "lpMem : 0x" << std::hex << lpMem << std::endl;
    sprintf_s(dbgTemp, "lpMem : %p\n", lpMem);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    //printf("******************\n\n");
    WriteDbg("******************\n\n");
    ClearDbg(dbgOutput);
    return OldHeapFree(hHeap, dwFlags, lpMem);
}

static LSTATUS(WINAPI* OldRegCreateKeyEx)(
    HKEY                        hKey,
    LPCWSTR                     lpSubKey,
    DWORD                       Reserved,
    LPWSTR                      lpClass,
    DWORD                       dwOptions,
    REGSAM                      samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY                       phkResult,
    LPDWORD                     lpdwDisposition) = RegCreateKeyEx;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCreateKeyEx(
    HKEY                        hKey,
    LPCWSTR                     lpSubKey,
    DWORD                       Reserved,
    LPWSTR                      lpClass,
    DWORD                       dwOptions,
    REGSAM                      samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY                       phkResult,
    LPDWORD                     lpdwDisposition
) {
    WriteDbg("\n\n**********************\n");
    ClearDbg(dbgOutput);
    WriteDbg("RegCreateKey Hooked\n");
    ClearDbg(dbgOutput);

    GetLocalTime(&st);
    sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);

    sprintf_s(dbgTemp, "hKey : %p\n", hKey);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    // 宽字节转char
    memset(tempR, 0, sizeof(tempR));
    WideCharToMultiByte(CP_ACP, 0, lpSubKey, wcslen(lpSubKey), tempR, sizeof(tempR), NULL, NULL);
    sprintf_s(dbgTemp, "lpSubKey : %s\n", tempR);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    ClearTempR(tempR);
    sprintf_s(dbgTemp, "Reserved : %08x\n", Reserved);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    sprintf_s(dbgTemp, "lpClass : %p\n", lpClass);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    sprintf_s(dbgTemp, "dwOptions : %08x\n", dwOptions);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    sprintf_s(dbgTemp, "samDesired : %08x\n", samDesired);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    sprintf_s(dbgTemp, "lpSecurityAttributes : %p\n", lpSecurityAttributes);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    sprintf_s(dbgTemp, "phkResult : %p\n", phkResult);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    sprintf_s(dbgTemp, "lpdwDisposition : %p\n", lpdwDisposition);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);

    WriteDbg("******************\n\n");
    ClearDbg(dbgOutput);

    return OldRegCreateKeyEx(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}

static LSTATUS(WINAPI* OldRegOpenKeyEx)(
    HKEY    hKey,
    LPCWSTR lpSubKey,
    DWORD   ulOptions,
    REGSAM  samDesired,
    PHKEY   phkResult
    ) = RegOpenKeyExW;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegOpenKeyEx(
    HKEY    hKey,
    LPCWSTR lpSubKey,
    DWORD   ulOptions,
    REGSAM  samDesired,
    PHKEY   phkResult)
{
    if (strcmp(targetPath, hBuf) != 0)
    {
        ClearDbg(hBuf);
        return OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
    }
    
    WriteDbg("\n\n**********************\n");
    ClearDbg(dbgOutput);
    WriteDbg("RegCreateKey Hooked\n");
    ClearDbg(dbgOutput);

    GetLocalTime(&st);
    sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);

    sprintf_s(dbgTemp, "hKey : %p\n", hKey);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    // 宽字节转char
    memset(tempR, 0, sizeof(tempR));
    WideCharToMultiByte(CP_ACP, 0, lpSubKey, wcslen(lpSubKey), tempR, sizeof(tempR), NULL, NULL);
    sprintf_s(dbgTemp, "lpSubKey : %s\n", tempR);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    ClearTempR(tempR);
    sprintf_s(dbgTemp, "ulOptions : %08x\n", ulOptions);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    sprintf_s(dbgTemp, "samDesired : %08x\n", samDesired);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    sprintf_s(dbgTemp, "phkResult : %p\n", phkResult);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);

    WriteDbg("******************\n\n");
    ClearDbg(dbgOutput);

    return OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

static LSTATUS(WINAPI* OldRegCloseKey)(HKEY hKey) = RegCloseKey;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCloseKey(HKEY hKey)
{
    WriteDbg("\n\n**********************\n");
    ClearDbg(dbgOutput);
    WriteDbg("RegCloseKey Hooked\n");
    ClearDbg(dbgOutput);

    GetLocalTime(&st);
    sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);

    sprintf_s(dbgTemp, "hKey : %p\n", hKey);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);

    WriteDbg("******************\n\n");
    ClearDbg(dbgOutput);

    return OldRegCloseKey(hKey);
}

static LSTATUS(WINAPI* OldRegSetValueEx)(
    HKEY       hKey,
    LPCWSTR    lpValueName,
    DWORD      Reserved,
    DWORD      dwType,
    const BYTE* lpData,
    DWORD      cbData
    ) = RegSetValueEx;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegSetValueEx(
    HKEY       hKey,
    LPCWSTR    lpValueName,
    DWORD      Reserved,
    DWORD      dwType,
    const BYTE * lpData,
    DWORD      cbData)
{
    WriteDbg("\n\n**********************\n");
    ClearDbg(dbgOutput);
    WriteDbg("RegSetValue Hooked\n");
    ClearDbg(dbgOutput);

    GetLocalTime(&st);
    sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);

    sprintf_s(dbgTemp, "hKey : %p\n", hKey);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    // 宽字节转char
    memset(tempR, 0, sizeof(tempR));
    WideCharToMultiByte(CP_ACP, 0, lpValueName, wcslen(lpValueName), tempR, sizeof(tempR), NULL, NULL);
    sprintf_s(dbgTemp, "lpValueName : %s\n", tempR);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    ClearTempR(tempR);
    sprintf_s(dbgTemp, "Reserved : %08x\n", Reserved);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    sprintf_s(dbgTemp, "dwType : %08x\n", dwType);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    sprintf_s(dbgTemp, "lpData : %p\n", lpData);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    sprintf_s(dbgTemp, "cbData : %08x\n", cbData);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);

    WriteDbg("******************\n\n");
    ClearDbg(dbgOutput);
    
    return OldRegSetValueEx(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}

static LSTATUS(WINAPI* OldRegDeleteValue)(
    HKEY    hKey,
    LPCWSTR lpValueName
    ) = RegDeleteValueW;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegDeleteValue(
    HKEY    hKey,
    LPCWSTR lpValueName)
{
    if (strcmp(targetPath, hBuf) != 0)
    {
        ClearDbg(hBuf);
        return OldRegDeleteValue(hKey, lpValueName);
    }

    WriteDbg("\n\n**********************\n");
    ClearDbg(dbgOutput);
    WriteDbg("RegDeleteValue Hooked\n");
    ClearDbg(dbgOutput);

    GetLocalTime(&st);
    sprintf_s(dbgTemp, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);

    sprintf_s(dbgTemp, "hKey : %p\n", hKey);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    // 宽字节转char
    memset(tempR, 0, sizeof(tempR));
    WideCharToMultiByte(CP_ACP, 0, lpValueName, wcslen(lpValueName), tempR, sizeof(tempR), NULL, NULL);
    sprintf_s(dbgTemp, "lpValueName : %s\n", tempR);
    WriteDbg(dbgTemp);
    ClearDbg(dbgOutput);
    ClearDbg(dbgTemp);
    ClearTempR(tempR);

    WriteDbg("******************\n\n");
    ClearDbg(dbgOutput);

    return OldRegDeleteValue(hKey, lpValueName);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)OldMeesageBoxW, NewMessageBoxW);
        DetourAttach(&(PVOID&)OldMeesageBoxA, NewMessageBoxA);
        DetourAttach(&(PVOID&)OldCreateFile, NewCreateFile);
        DetourAttach(&(PVOID&)OldWriteFile, NewWriteFile);
        DetourAttach(&(PVOID&)OldReadFile, NewReadFile);
        DetourAttach(&(PVOID&)OldCloseHandle, NewCloseHandle);
        DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
        DetourAttach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
        //DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
        DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);
        DetourAttach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
        DetourAttach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
        DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        DetourAttach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        DetourAttach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);
        DetourTransactionCommit();
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)OldMeesageBoxW, NewMessageBoxW);
        DetourDetach(&(PVOID&)OldMeesageBoxA, NewMessageBoxA);
        DetourDetach(&(PVOID&)OldCreateFile, NewCreateFile);
        DetourDetach(&(PVOID&)OldWriteFile, NewWriteFile);
        DetourDetach(&(PVOID&)OldReadFile, NewReadFile);
        DetourDetach(&(PVOID&)OldCloseHandle, NewCloseHandle);
        DetourDetach(&(PVOID&)OldHeapCreate, NewHeapCreate);
        DetourDetach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
        //DetourDetach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
        DetourDetach(&(PVOID&)OldHeapFree, NewHeapFree);
        DetourDetach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
        DetourDetach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
        DetourDetach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        DetourDetach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        DetourDetach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);
        DetourTransactionCommit();
        break;
    }
    return TRUE;
 }


~~~

