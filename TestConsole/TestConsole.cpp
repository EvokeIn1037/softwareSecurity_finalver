// TestConsole.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cstdio>
#include <windows.h>
#include <detours.h>
#include <string.h>

#pragma comment(lib, "detours.lib")

typedef void(*ptrtarget)(char*);
HMODULE hMod = LoadLibrary(L"F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\InnjectDll.dll");
ptrtarget  AimTarget = (ptrtarget)GetProcAddress(hMod, "AimTarget");

int main()
{
    char aimPath[256];
    AimTarget(aimPath);
    std::cout << "Hello World!\n";
    printf("Test!!!!!!!\n");
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    si.cb = sizeof(STARTUPINFO);
    WCHAR DirPath[MAX_PATH + 1];
    wcscpy_s(DirPath, MAX_PATH, L"F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug");  //Dll的文件夹
    char DLLPath[MAX_PATH + 1] = "F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\InnjectDll.dll";  //Dll的地址
    WCHAR EXE[MAX_PATH + 1] = { 0 };
    wchar_t ws[100];
    swprintf(ws, 100, L"%hs", aimPath);
    wcscpy_s(EXE, MAX_PATH, ws);  //需要注入的地址
    //wcscpy_s(EXE, MAX_PATH, L"F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\TestApp.exe");  //需要注入的地址

    if (DetourCreateProcessWithDllEx(EXE, NULL, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE |
        CREATE_SUSPENDED, NULL, DirPath, &si, &pi, DLLPath, NULL))
    {
        //MessageBoxA(NULL, "INJECT", "INJECT", NULL);
        ResumeThread(pi.hThread);
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    else
    {
        char error[100];
        sprintf_s(error, "%d", GetLastError()); //MessageBoxA(NULL, error, NULL, NULL);
    }

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
