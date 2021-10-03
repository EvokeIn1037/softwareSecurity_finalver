// TestApp.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "TestApp.h"
#include "string.h"
#include "windows.h"
#include "winbase.h"
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

#define MAX_LOADSTRING 100

HWND hWndMessageBoxW;

//堆变量初始化
const int hHeapSize = 1024;
HANDLE hNewHeap;                 //堆初始化操作
char* szBuf;

//文件变量初始化
HANDLE hNewFile;                 //文件初始化操作
HANDLE hOpenFile;
LPCTSTR lpPath = "F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\lyf.cpp";  //定义文件路径
char fileContent[256];
char* fileBuf;
bool readFlag;
int fileSize = 0;

//注册表变量初始化
HKEY hKey = NULL;
DWORD dwOptions = REG_OPTION_NON_VOLATILE;
long lRet;
size_t lRet1;
size_t lRet2;
LPCTSTR szModule = "%windir%\\system32\\cmd.exe";

//socket变量初始化
WSADATA wsaData;
SOCKET sock;
SOCKET servSock;
sockaddr_in sockAddr;
char szBuffer[MAXBYTE] = { 0 };
int wsaStartup;
SOCKADDR clntAddr;
SOCKET clntSock;
int socketSize = sizeof(SOCKADDR);
char socketstr[32] = "This is a message.";

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
    LoadStringW(hInstance, IDC_TESTAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTAPP));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

            case ID_TEST:
                MessageBox(0,"LYF\r\nTest","Test****",0);
                break;
            case ID_TEST2:
                MessageBoxW(hWndMessageBoxW, L"LYF\r\nTest", L"Test****", MB_OKCANCEL);

            case ID_HEAPCREATE:
                hNewHeap = (PBYTE) :: HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_GENERATE_EXCEPTIONS, hHeapSize, (2 * hHeapSize));
                break;
            case ID_HEAPDESTROY:
                HeapDestroy(hNewHeap);
                break;
            case ID_HEAPALLOC:
                szBuf = (char*)HeapAlloc(hNewHeap, HEAP_ZERO_MEMORY, hHeapSize);
                break;
            case ID_HEAPFREE:
                HeapFree(hNewHeap, 0, szBuf);
                break;

            case ID_CAWFILE:
                hNewFile = (HANDLE)CreateFile(lpPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
                strcpy_s(fileContent, "#include<stdio.h>\nint main()\n{\n\tprintf(\"Hello World!\");\n\treturn 0;\n}");
                WriteFile(hNewFile, fileContent, strlen(fileContent), NULL, NULL);
                CloseHandle(hNewFile);
                break;
            case ID_READFILE:
                hOpenFile = (HANDLE)CreateFile(lpPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
                if (hOpenFile == INVALID_HANDLE_VALUE)
                {
                    hOpenFile = NULL;
                    return 0;
                }
                fileSize = GetFileSize(hOpenFile, NULL);
                fileBuf = (char*)malloc((fileSize + 1) * sizeof(char));
                readFlag = ReadFile(hOpenFile, fileBuf, fileSize, NULL, NULL);
                if(fileBuf){
                    fileBuf[fileSize] = 0;
                }
                free(fileBuf);
                CloseHandle(hOpenFile);
                break;

            case ID_REGCREATESETVALUE:
                hKey = NULL;
                dwOptions = REG_OPTION_NON_VOLATILE;
                DWORD dwDisposition;
                lRet = RegCreateKeyExW(HKEY_CURRENT_USER, L"lyfKey", 0, NULL, dwOptions, KEY_WRITE, NULL, &hKey, &dwDisposition);
                if (lRet != ERROR_SUCCESS)
                {
                    return 0;
                }
                else {
                    szModule = "%windir%\\system32\\cmd.exe";
                    //BYTE* szModule = (BYTE*)"%windir%\\system32\\cmd.exe";
                    lRet2 = RegSetValueExW(hKey, L"mytest", 0, REG_SZ, (BYTE*)szModule, (lstrlen(szModule)) * sizeof(TCHAR));
                    if (lRet2 != ERROR_SUCCESS) {
                        return 0;
                    }
                }
                RegCloseKey(hKey);
                break;
            case ID_REGOPEN:
                //hKey = NULL;
                lRet1 = RegOpenKeyExW(HKEY_CURRENT_USER, L"lyfKey", 0, KEY_ALL_ACCESS, &hKey);
                if (lRet1 != ERROR_SUCCESS)
                {
                    return 0;
                }
                else{
                    lRet2 = RegDeleteValueW(hKey, L"mytest");
                    if (lRet2 != ERROR_SUCCESS) {
                        return 0;
                    }
                }
                RegCloseKey(hKey);
                break;

            case ID_COPYFILE:
                CopyFileW(L"F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\lyf.cpp", L"F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\lyfcopy.cpp", false);
                break;

            case ID_REGSTARTUP:
                //hKey = NULL;
                lRet1 = RegOpenKeyExW(HKEY_CURRENT_USER, (LPWSTR)L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey);
                if (lRet1 != ERROR_SUCCESS)
                {
                    return 0;
                }
                else {
                    lRet2 = RegDeleteValueW(hKey, L"mytest");
                    if (lRet2 != ERROR_SUCCESS) {
                        return 0;
                    }
                }
                RegCloseKey(hKey);
                break;

            case ID_RECVDATA:
                //初始化DLL
                wsaStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
                //创建套接字
                sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
                //向服务器发起请求
                memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
                sockAddr.sin_family = PF_INET;
                sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
                sockAddr.sin_port = htons(1234);
                connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
                Sleep(500);
                //接收服务器传回的数据
                recv(sock, szBuffer, MAXBYTE, NULL);
                /*输出接收到的数据
                printf("Message form server: %s\n", szBuffer);*/
                //关闭套接字
                closesocket(sock);
                //终止使用 DLL
                WSACleanup();
                break;
            case ID_SENDDATA:
                //初始化 DLL
                wsaStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
                //创建套接字
                servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
                //绑定套接字
                memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
                sockAddr.sin_family = PF_INET;  //使用IPv4地址
                sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
                sockAddr.sin_port = htons(1234);  //端口
                bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
                //进入监听状态
                listen(servSock, 20);
                //接收客户端请求
                clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &socketSize);
                //向客户端发送数据
                send(clntSock, socketstr, strlen(socketstr) + sizeof(char), NULL);
                //关闭套接字
                closesocket(clntSock);
                closesocket(servSock);
                //终止 DLL 的使用
                WSACleanup();
                break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
                break;
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
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
