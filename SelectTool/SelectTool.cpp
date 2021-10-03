// SelectTool.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "SelectTool.h"
#include "windows.h"
#include "ShellAPI.h"
#include "Commdlg.h"
#include "Shlobj.h"

#pragma comment(lib, "Shell32.lib")

#define MAX_LOADSTRING 100

SHELLEXECUTEINFO view;

typedef void(*ptrtarget)(char*);
HMODULE hMod = LoadLibrary(L"F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\InnjectDll.dll");
ptrtarget  GetTarget = (ptrtarget)GetProcAddress(hMod, "GetTarget");

DWORD num;
TCHAR fullPath[MAX_PATH] = { 0 };//用于接收文件名
OPENFILENAME ofn = { 0 };
STARTUPINFO si = { sizeof(si) };
PROCESS_INFORMATION pi;
TCHAR szCommandLine[] = TEXT("F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\ResultView.exe");
TCHAR szCommandLine1[] = TEXT("F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\TestConsole.exe");
TCHAR szCommandLine2[] = TEXT("F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\SelectWindow.exe");

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SELECTTOOL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SELECTTOOL));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SELECTTOOL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            
            case ID_SELECTFILEA:
                ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小
                ofn.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄
                ofn.nFilterIndex = 1;//过滤器索引
                ofn.lpstrFile = fullPath;//接收返回的文件名，注意第一个字符需要为NULL
                ofn.nMaxFile = sizeof(fullPath);//缓冲区长度
                ofn.lpstrInitialDir = NULL;//初始目录为默认
                ofn.lpstrTitle = TEXT("请选择一个文件 ");//使用系统默认标题留空即可
                ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项
                GetOpenFileName(&ofn);
                char path[256];
                num = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, NULL, 0, NULL, 0);
                WideCharToMultiByte(CP_ACP, 0, fullPath, -1, path, num, NULL, 0);
                //取出对话框中用来显示结果的控件
                GetTarget(path);
                //WinExec("F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\TestConsole.exe", SW_SHOWNORMAL);
                CreateProcess(NULL, szCommandLine1, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
                break;
            
            case ID_SELECTFILE:
                CreateProcess(NULL, szCommandLine2, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
                break;
            case ID_RESULTVIEW:
                /*memset(&view, 0, sizeof(SHELLEXECUTEINFO));
                view.cbSize = sizeof(SHELLEXECUTEINFO);
                //view.lpVerb = "runas";
                view.lpFile = L"F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\ResultView.exe"; //要打开的进程路径
                //view.lpParameters = "Administrator"; //管理员权限
                view.nShow = SW_HIDE;//打开的进程不显示界面
                view.fMask = 0;//不要进程句柄
                ShellExecuteEx(&view);*/
                CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
                break;
            case ID_TESTCONSOLE:
                CreateProcess(NULL, szCommandLine1, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(hWnd, &ps);
            HBITMAP hbm;
            BITMAP  bminfo;
            LPRECT Rect = (LPRECT)malloc(sizeof(RECT));
            HDC memdc = CreateCompatibleDC(dc);

            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            if(Rect)
            {
                GetWindowRect(hWnd, Rect);
            }
            hbm = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));
            GetObject(hbm, sizeof(bminfo), &bminfo);
            SelectObject(memdc, hbm);
            if(Rect)
            {
                StretchBlt(dc, 0, 0, Rect->right - Rect->left, Rect->bottom - Rect->top,
                    memdc, 0, 0, bminfo.bmWidth, bminfo.bmHeight, SRCCOPY);
            }
            DeleteDC(memdc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
