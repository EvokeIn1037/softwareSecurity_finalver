// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "framework.h"
#include "detours.h"
#include "cstdio"
#include "stdarg.h"
#include "windows.h"
#include <iostream>
#include <mutex>
#include <string.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "detours.lib")

#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

SYSTEMTIME st;

char Temp[256];

char hBuf[256];

//HANDLE HeapAll;
//bool HeapAllocFlag = true;
//int heapAllocTime = 0;

char tempR[70];

int i, j, flagFileCopy;

#pragma data_seg("MySeg")
char targetPath[256] = { };
typedef struct INFO {
    char data[10];
    int dataNum;
    char hookMessage[30] = { };
    char hookLog[40] = { };
    char dataName[3][30] = { };
    char dataValue[3][70] = { };
    char dataContent[10][100] = { };
}info;
volatile int count = 0;
info dataInfo[50] = { };
#pragma data_seg()
#pragma comment(linker, "/section:MySeg,RWS")

std::mutex mtx;

extern "C" __declspec(dllexport)void GetResult(info* result, int count) {
    mtx.lock();
    for (int i = 0; i < count + 1; i++) {
        strcpy_s(result[i].data, 10, dataInfo[i].data);
        result[i].dataNum = dataInfo[i].dataNum;
        strcpy_s(result[i].hookMessage, 30, dataInfo[i].hookMessage);
        strcpy_s(result[i].hookLog, 40, dataInfo[i].hookLog);
        for (int j = 0; j < dataInfo[i].dataNum; j++) {
            strcpy_s(result[i].dataContent[j], 100, dataInfo[i].dataContent[j]);
        }
    }
    mtx.unlock();
}

extern "C" __declspec(dllexport)void GetCount(int& rcount) {
    mtx.lock();
    rcount = count;
    mtx.unlock();
}

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
    GetModuleFileNameA(NULL, hBuf, 256);
    if (strcmp(targetPath, hBuf) != 0)
    {
        ClearDbg(hBuf);
        return OldMeesageBoxA(NULL, "new MessageBoxA", "Hooked", MB_OK);
    }

    if (count == 50)
    {
        count = 0;
    }
    
    dataInfo[count].dataNum = 2;

    strcpy_s(dataInfo[count].data, 10, "msg");

    sprintf_s(dataInfo[count].hookMessage, "MessageBoxA Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    sprintf_s(dataInfo[count].dataContent[0], "lpText : %s", lpText);
    sprintf_s(dataInfo[count].dataContent[1], "lpCaption : %s", lpCaption);
    
    count++;

    ClearDbg(hBuf);

    return OldMeesageBoxA(NULL, "new MessageBoxA", "Hooked", MB_OK);
}

extern "C" __declspec(dllexport)int WINAPI NewMessageBoxW(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType)
{
    GetModuleFileNameA(NULL, hBuf, 256);
    if (strcmp(targetPath, hBuf) != 0)
    {
        ClearDbg(hBuf);
        return OldMeesageBoxW(NULL, L"new MessageBoxW", L"Hooked", MB_OK);
    }

    if (count == 50)
    {
        count = 0;
    }

    dataInfo[count].dataNum = 2;

    strcpy_s(dataInfo[count].data, 10, "msg");

    sprintf_s(dataInfo[count].hookMessage, "MessageBoxW Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    if(lpText)
    {
        // 宽字节转char
        memset(tempR, 0, sizeof(tempR));
        WideCharToMultiByte(CP_ACP, 0, lpText, wcslen(lpText), tempR, sizeof(tempR), NULL, NULL);
    }
    sprintf_s(dataInfo[count].dataContent[0], "lpText : %s", tempR);
    ClearTempR(tempR);

    if (lpCaption)
    {
        // 宽字节转char
        memset(tempR, 0, sizeof(tempR));
        WideCharToMultiByte(CP_ACP, 0, lpCaption, wcslen(lpCaption), tempR, sizeof(tempR), NULL, NULL);
    }
    sprintf_s(dataInfo[count].dataContent[1], "lpCaption : %s", tempR);
    ClearTempR(tempR);

    count++;

    ClearDbg(hBuf);

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
    GetModuleFileNameA(NULL, hBuf, 256);
    if (strcmp(targetPath, hBuf) != 0)
    {
        ClearDbg(hBuf);
        return OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }

    if (count == 50)
    {
        count = 0;
    }

    dataInfo[count].dataNum = 7;

    strcpy_s(dataInfo[count].data, 10, "file");

    sprintf_s(dataInfo[count].hookMessage, "CreateFile Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, pchar, num, NULL, FALSE);
    memset(Temp, 0, sizeof(Temp));

    sprintf_s(dataInfo[count].dataName[0], "lpFileName");
    sprintf_s(dataInfo[count].dataValue[0], "%s", pchar);

    sprintf_s(dataInfo[count].dataContent[0], "lpFileName : %s", pchar);
    sprintf_s(dataInfo[count].dataContent[1], "dwDesiredAccess : %lu", dwDesiredAccess);
    sprintf_s(dataInfo[count].dataContent[2], "dwShareMode : %lu", dwShareMode);
    sprintf_s(dataInfo[count].dataContent[3], "lpSecurityAttributes : %p", lpSecurityAttributes);
    sprintf_s(dataInfo[count].dataContent[4], "dwCreationDisposition : %lu", dwCreationDisposition);
    sprintf_s(dataInfo[count].dataContent[5], "dwFlagsAndAttributes : %lu", dwFlagsAndAttributes);
    sprintf_s(dataInfo[count].dataContent[6], "hTemplateFile : %p", hTemplateFile);

    count++;

    ClearDbg(hBuf);

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
    GetModuleFileNameA(NULL, hBuf, 256);
    if (strcmp(targetPath, hBuf) != 0)
    {
        ClearDbg(hBuf);
        return OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
    }

    if (count == 50)
    {
        count = 0;
    }

    if (GetFileType(hFile) == FILE_TYPE_DISK) {
        dataInfo[count].dataNum = 5;

        strcpy_s(dataInfo[count].data, 10, "file");

        sprintf_s(dataInfo[count].hookMessage, "WriteFile Hooked");

        GetLocalTime(&st);
        sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

        sprintf_s(dataInfo[count].dataContent[0], "hFile : %p", hFile);
        sprintf_s(dataInfo[count].dataContent[1], "lpBuffer : %p", lpBuffer);
        sprintf_s(dataInfo[count].dataContent[2], "nNumberOfBytesToWrite : %08X", nNumberOfBytesToWrite);
        sprintf_s(dataInfo[count].dataContent[3], "lpNumberOfBytesWritten : %p", lpNumberOfBytesWritten);
        sprintf_s(dataInfo[count].dataContent[4], "lpOverlapped : %p", lpOverlapped);

        count++;
    }

    ClearDbg(hBuf);

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
    GetModuleFileNameA(NULL, hBuf, 256);
    if (strcmp(targetPath, hBuf) != 0)
    {
        ClearDbg(hBuf);
        return OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
    }

    if (count == 50)
    {
        count = 0;
    }

    if (GetFileType(hFile) == FILE_TYPE_DISK) {
        dataInfo[count].dataNum = 5;

        strcpy_s(dataInfo[count].data, 10, "file");

        sprintf_s(dataInfo[count].hookMessage, "ReadFile Hooked");

        GetLocalTime(&st);
        sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

        sprintf_s(dataInfo[count].dataContent[0], "hFile : %p", hFile);
        sprintf_s(dataInfo[count].dataContent[1], "lpBuffer : %p", lpBuffer);
        sprintf_s(dataInfo[count].dataContent[2], "nNumberOfBytesToRead : %08X", nNumberOfBytesToRead);
        sprintf_s(dataInfo[count].dataContent[3], "lpNumberOfBytesRead : %p", lpNumberOfBytesRead);
        sprintf_s(dataInfo[count].dataContent[4], "lpOverlapped : %p", lpOverlapped);

        count++;
    }

    ClearDbg(hBuf);

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
    {
        ClearDbg(hBuf);
        return OldCloseHandle(hFile);
    }

    if (count == 50)
    {
        count = 0;
    }

    if (GetFileType(hFile) == FILE_TYPE_DISK) {
        dataInfo[count].dataNum = 1;

        strcpy_s(dataInfo[count].data, 10, "file");

        sprintf_s(dataInfo[count].hookMessage, "CloseHandle Hooked");

        GetLocalTime(&st);
        sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

        sprintf_s(dataInfo[count].dataContent[0], "hFile : %p", hFile);

        count++;
    }

    ClearDbg(hBuf);

    return OldCloseHandle(hFile);
}

// 堆操作 HeapCreate HeapDestroy HeapAlloc HeapFree
static HANDLE(WINAPI* OldHeapCreate)(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) = HeapCreate;

extern "C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
{
    if (count == 50)
    {
        count = 0;
    }
    
    HANDLE hHeap = OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);

    dataInfo[count].dataNum = 4;

    strcpy_s(dataInfo[count].data, 10, "heap");

    sprintf_s(dataInfo[count].hookMessage, "HeapCreate Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    sprintf_s(dataInfo[count].dataContent[0], "fIOoptions : %lu", fIOoptions);
    sprintf_s(dataInfo[count].dataContent[1], "dwInitialSize : %zu", dwInitialSize);
    sprintf_s(dataInfo[count].dataContent[2], "dwMaximumSize : %zu", dwMaximumSize);
    sprintf_s(dataInfo[count].dataContent[3], "hHeap : %p", hHeap);

    count++;

    ClearDbg(hBuf);

    return hHeap;
}

static BOOL(WINAPI* OldHeapDestroy)(HANDLE) = HeapDestroy;

extern "C" __declspec(dllexport)BOOL WINAPI NewHeapDestroy(HANDLE hHeap)
{
    if (count == 50)
    {
        count = 0;
    }
    
    dataInfo[count].dataNum = 1;

    strcpy_s(dataInfo[count].data, 10, "heap");

    sprintf_s(dataInfo[count].hookMessage, "HeapDestroy Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    sprintf_s(dataInfo[count].dataContent[0], "hHeap : %p", hHeap);

    count++;

    ClearDbg(hBuf);

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
    GetModuleFileNameA(NULL, hBuf, 256);
    if (strcmp(targetPath, hBuf) != 0)
    {
        ClearDbg(hBuf);
        return OldHeapFree(hHeap, dwFlags, lpMem);
    }

    if (count == 50)
    {
        count = 0;
    }

    dataInfo[count].dataNum = 3;

    strcpy_s(dataInfo[count].data, 10, "heap");

    sprintf_s(dataInfo[count].hookMessage, "HeapFree Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    sprintf_s(dataInfo[count].dataContent[0], "hHeap : %p", hHeap);
    sprintf_s(dataInfo[count].dataContent[1], "dwFlags :  %ul", dwFlags);
    sprintf_s(dataInfo[count].dataContent[2], "lpMem : %p", lpMem);

    count++;

    ClearDbg(hBuf);

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
    if (count == 50)
    {
        count = 0;
    }
    
    dataInfo[count].dataNum = 9;

    strcpy_s(dataInfo[count].data, 10, "reg");

    sprintf_s(dataInfo[count].hookMessage, "RegCreateKey Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    // 宽字节转char
    memset(tempR, 0, sizeof(tempR));
    WideCharToMultiByte(CP_ACP, 0, lpSubKey, wcslen(lpSubKey), tempR, sizeof(tempR), NULL, NULL);

    sprintf_s(dataInfo[count].dataContent[0], "hKey : %p", hKey);
    sprintf_s(dataInfo[count].dataContent[1], "lpSubKey : %s", tempR);
    sprintf_s(dataInfo[count].dataContent[2], "Reserved : %08x", Reserved);
    sprintf_s(dataInfo[count].dataContent[3], "lpClass : %p", lpClass);
    sprintf_s(dataInfo[count].dataContent[4], "dwOptions : %08X", dwOptions);
    sprintf_s(dataInfo[count].dataContent[5], "samDesired : %08x", samDesired);
    sprintf_s(dataInfo[count].dataContent[6], "lpSecurityAttributes : %p", lpSecurityAttributes);
    sprintf_s(dataInfo[count].dataContent[7], "phkResult : %p", phkResult);
    sprintf_s(dataInfo[count].dataContent[8], "lpdwDisposition : %p", lpdwDisposition);

    ClearTempR(tempR);

    count++;

    ClearDbg(hBuf);

    return OldRegCreateKeyEx(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}

static LSTATUS(WINAPI* OldRegOpenKeyEx)(
    HKEY    hKey,
    LPCWSTR lpSubKey,
    DWORD   ulOptions,
    REGSAM  samDesired,
    PHKEY   phkResult
    ) = RegOpenKeyEx;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegOpenKeyEx(
    HKEY    hKey,
    LPCWSTR lpSubKey,
    DWORD   ulOptions,
    REGSAM  samDesired,
    PHKEY   phkResult)
{
    GetModuleFileNameA(NULL, hBuf, 256);
    if (strcmp(targetPath, hBuf) != 0)
    {
        ClearDbg(hBuf);
        return OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
    }

    if (count == 50)
    {
        count = 0;
    }
    
    dataInfo[count].dataNum = 5;

    strcpy_s(dataInfo[count].data, 10, "reg");

    sprintf_s(dataInfo[count].hookMessage, "RegOpenKey Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    // 宽字节转char
    memset(tempR, 0, sizeof(tempR));
    WideCharToMultiByte(CP_ACP, 0, lpSubKey, wcslen(lpSubKey), tempR, sizeof(tempR), NULL, NULL);

    sprintf_s(dataInfo[count].dataContent[0], "hKey : %p", hKey);
    sprintf_s(dataInfo[count].dataContent[1], "lpSubKey : %s", tempR);
    sprintf_s(dataInfo[count].dataContent[2], "ulOptions : %08x", ulOptions);
    sprintf_s(dataInfo[count].dataContent[3], "samDesired : %08x", samDesired);
    sprintf_s(dataInfo[count].dataContent[4], "phkResult : %p", phkResult);

    ClearTempR(tempR);

    count++;

    ClearDbg(hBuf);

    return OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

static LSTATUS(WINAPI* OldRegCloseKey)(HKEY hKey) = RegCloseKey;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCloseKey(HKEY hKey)
{
    GetModuleFileNameA(NULL, hBuf, 256);
    if (strcmp(targetPath, hBuf) != 0)
    {
        ClearDbg(hBuf);
        return OldRegCloseKey(hKey);
    }

    if (count == 50)
    {
        count = 0;
    }
    
    dataInfo[count].dataNum = 1;

    strcpy_s(dataInfo[count].data, 10, "reg");

    sprintf_s(dataInfo[count].hookMessage, "RegCloseKey Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    sprintf_s(dataInfo[count].dataContent[0], "hKey : %p", hKey);

    count++;

    ClearDbg(hBuf);

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
    if (count == 50)
    {
        count = 0;
    }
    
    dataInfo[count].dataNum = 6;

    strcpy_s(dataInfo[count].data, 10, "reg");

    sprintf_s(dataInfo[count].hookMessage, "RegSetValue Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    // 宽字节转char
    memset(tempR, 0, sizeof(tempR));
    WideCharToMultiByte(CP_ACP, 0, lpValueName, wcslen(lpValueName), tempR, sizeof(tempR), NULL, NULL);

    sprintf_s(dataInfo[count].dataContent[0], "hKey : %p", hKey);
    sprintf_s(dataInfo[count].dataContent[1], "lpValueContent : %s", tempR);
    sprintf_s(dataInfo[count].dataContent[2], "Reserved : %08x", Reserved);
    sprintf_s(dataInfo[count].dataContent[3], "dwType : %08x", dwType);
    sprintf_s(dataInfo[count].dataContent[4], "lpData : %p", lpData);
    sprintf_s(dataInfo[count].dataContent[5], "cbData : %08x", cbData);

    ClearTempR(tempR);

    count++;

    ClearDbg(hBuf);
    
    return OldRegSetValueEx(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}

static LSTATUS(WINAPI* OldRegDeleteValue)(
    HKEY    hKey,
    LPCWSTR lpValueName
    ) = RegDeleteValue;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegDeleteValue(
    HKEY    hKey,
    LPCWSTR lpValueName)
{
    if (count == 50)
    {
        count = 0;
    }
    
    dataInfo[count].dataNum = 2;

    strcpy_s(dataInfo[count].data, 10, "reg");

    sprintf_s(dataInfo[count].hookMessage, "RegDeleteValue Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    // 宽字节转char
    memset(tempR, 0, sizeof(tempR));
    WideCharToMultiByte(CP_ACP, 0, lpValueName, wcslen(lpValueName), tempR, sizeof(tempR), NULL, NULL);

    sprintf_s(dataInfo[count].dataContent[0], "hKey : %p", hKey);
    sprintf_s(dataInfo[count].dataContent[1], "lpValueName : %s", tempR);

    ClearTempR(tempR);

    count++;

    ClearDbg(hBuf);

    return OldRegDeleteValue(hKey, lpValueName);
}

static BOOL(WINAPI* OldCopyFile)(
    LPCWSTR lpExistingFileName,
    LPCWSTR lpNewFileName,
    BOOL    bFailIfExists
    ) = CopyFileW;

extern "C" __declspec(dllexport)BOOL WINAPI NewCopyFile(
    LPCWSTR lpExistingFileName,
    LPCWSTR lpNewFileName,
    BOOL    bFailIfExists
)
{
    if (count == 50)
    {
        count = 0;
    }
    
    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpExistingFileName, -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpExistingFileName, -1, pchar, num, NULL, FALSE);
    memset(Temp, 0, sizeof(Temp));

    flagFileCopy = 0;
    for (i = 0; i < 50; i++) 
    {
        for (j = 0; j < 3; j++)
        {
            if (strcmp(dataInfo[i].dataName[j], "lpFileName") == 0)
            {
                if (strcmp(pchar, dataInfo[i].dataValue[j]) == 0)
                {
                    flagFileCopy = 1;
                    break;
                    break;
                }
            }
        }
    }

    if(flagFileCopy)
    {
        dataInfo[count].dataNum = 3;

        strcpy_s(dataInfo[count].data, 10, "fileError");

        sprintf_s(dataInfo[count].hookMessage, "CopyFile Hooked");

        GetLocalTime(&st);
        sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

        int num1 = WideCharToMultiByte(CP_OEMCP, NULL, lpNewFileName, -1, NULL, 0, NULL, FALSE);
        char* pchar1 = new char[num1];
        WideCharToMultiByte(CP_OEMCP, NULL, lpNewFileName, -1, pchar1, num1, NULL, FALSE);
        memset(Temp, 0, sizeof(Temp));

        sprintf_s(dataInfo[count].dataContent[0], "lpExistingFileName : %s", pchar);
        sprintf_s(dataInfo[count].dataContent[1], "lpNewFileName : %p", pchar1);
        sprintf_s(dataInfo[count].dataContent[2], "bFailIfExists : %08X", bFailIfExists);

        count++;
    }

    ClearDbg(hBuf);

    return OldCopyFile(lpExistingFileName, lpNewFileName, bFailIfExists);
}


static SOCKET(WINAPI* OldSocket)(
    int af,
    int type,
    int protocol
    ) = socket;
extern "C" __declspec(dllexport)SOCKET WINAPI NewSocket(int af, int type, int protocol) 
{
    if (count == 50)
    {
        count = 0;
    }
    
    dataInfo[count].dataNum = 3;

    strcpy_s(dataInfo[count].data, 10, "socket");

    sprintf_s(dataInfo[count].hookMessage, "Socket Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    sprintf_s(dataInfo[count].dataContent[0], "af : %08x", af);
    sprintf_s(dataInfo[count].dataContent[1], "type :  %08x", type);
    sprintf_s(dataInfo[count].dataContent[2], "protocol : %08x", protocol);

    count++;

    ClearDbg(hBuf);
    
    return OldSocket(af, type, protocol);
}

static int (WINAPI* OldConnect)(
    SOCKET         s,
    const sockaddr* name,
    int            namelen
    ) = connect;
extern "C" __declspec(dllexport)int WINAPI NewConnect(SOCKET s, const sockaddr * name, int namelen) 
{
    if (count == 50)
    {
        count = 0;
    }
    
    dataInfo[count].dataNum = 5;

    strcpy_s(dataInfo[count].data, 10, "socket");

    sprintf_s(dataInfo[count].hookMessage, "Connect Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    struct sockaddr_in* sock = (struct sockaddr_in*)name;
    int port = ntohs(sock->sin_port);

    sprintf_s(dataInfo[count].dataContent[0], "s : %08X", s);
    sprintf_s(dataInfo[count].dataContent[1], "name : %p", name);
    sprintf_s(dataInfo[count].dataContent[2], "namelen : %08x", namelen);
    sprintf_s(dataInfo[count].dataContent[3], "IP : %s", inet_ntoa(sock->sin_addr));
    sprintf_s(dataInfo[count].dataContent[4], "port : %d", port);

    count++;

    ClearDbg(hBuf);

    return OldConnect(s, name, namelen);
}

static int (WINAPI* OldRecv)(
    SOCKET     s,
    char* buf,
    int        len,
    int        flags
    ) = recv;
extern "C" __declspec(dllexport)int WINAPI NewRecv(SOCKET s, char* buf, int len, int flags) 
{
    if (count == 50)
    {
        count = 0;
    }
    
    dataInfo[count].dataNum = 4;

    strcpy_s(dataInfo[count].data, 10, "socket");

    sprintf_s(dataInfo[count].hookMessage, "Recieve Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    sprintf_s(dataInfo[count].dataContent[0], "s : %08X", s);
    sprintf_s(dataInfo[count].dataContent[1], "buf : %p", buf);
    sprintf_s(dataInfo[count].dataContent[2], "len : %08X", len);
    sprintf_s(dataInfo[count].dataContent[3], "flags : %08X", flags);

    count++;

    ClearDbg(hBuf);

    return OldRecv(s, buf, len, flags);
}

static int (WINAPI* OldBind)(
    SOCKET         s,
    const sockaddr* name,
    int            namelen
    ) = bind;
extern "C" __declspec(dllexport)int WINAPI NewBind(SOCKET s, const sockaddr * name, int namelen) 
{
    if (count == 50)
    {
        count = 0;
    }
    
    dataInfo[count].dataNum = 5;

    strcpy_s(dataInfo[count].data, 10, "socket");

    sprintf_s(dataInfo[count].hookMessage, "Bind Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    struct sockaddr_in* sock = (struct sockaddr_in*)name;
    int port = ntohs(sock->sin_port);

    sprintf_s(dataInfo[count].dataContent[0], "s : %08X", s);
    sprintf_s(dataInfo[count].dataContent[1], "name : %p", name);
    sprintf_s(dataInfo[count].dataContent[2], "namelen : %08x", namelen);
    sprintf_s(dataInfo[count].dataContent[3], "IP : %s", inet_ntoa(sock->sin_addr));
    sprintf_s(dataInfo[count].dataContent[4], "port : %d", port);

    count++;

    ClearDbg(hBuf);

    return OldBind(s, name, namelen);
}

static int (WINAPI* OldSend)(
    SOCKET     s,
    const char* buf,
    int        len,
    int        flags
    ) = send;
extern "C" __declspec(dllexport)int WINAPI NewSend(SOCKET s, const char* buf, int len, int flags)
{
    if (count == 50)
    {
        count = 0;
    }
    
    dataInfo[count].dataNum = 5;

    strcpy_s(dataInfo[count].data, 10, "socket");

    sprintf_s(dataInfo[count].hookMessage, "Send Hooked");

    GetLocalTime(&st);
    sprintf_s(dataInfo[count].hookLog, "DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    sprintf_s(dataInfo[count].dataContent[0], "s : %08X", s);
    sprintf_s(dataInfo[count].dataContent[1], "buf : %p", buf);
    sprintf_s(dataInfo[count].dataContent[2], "len : %08x", len);
    sprintf_s(dataInfo[count].dataContent[3], "flags : %08x", flags);
    sprintf_s(dataInfo[count].dataContent[4], "data : %s", buf);

    count++;

    ClearDbg(hBuf);

    return OldSend(s, buf, len, flags);
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
        DetourAttach(&(PVOID&)OldCopyFile, NewCopyFile);
        DetourAttach(&(PVOID&)OldSocket, NewSocket);
        DetourAttach(&(PVOID&)OldConnect, NewConnect);
        DetourAttach(&(PVOID&)OldRecv, NewRecv);
        DetourAttach(&(PVOID&)OldBind, NewBind);
        DetourAttach(&(PVOID&)OldSend, NewSend);
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
        DetourDetach(&(PVOID&)OldCopyFile, NewCopyFile);
        DetourDetach(&(PVOID&)OldSocket, NewSocket);
        DetourDetach(&(PVOID&)OldConnect, NewConnect);
        DetourDetach(&(PVOID&)OldRecv, NewRecv);
        DetourDetach(&(PVOID&)OldBind, NewBind);
        DetourDetach(&(PVOID&)OldSend, NewSend);
        DetourTransactionCommit();
        break;
    }
    return TRUE;
 }

