// FileShow.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "FileShow.h"
#include "string.h"

#define MAX_LOADSTRING 100

typedef struct INFO {
    char data[10];
    int dataNum;
    char hookMessage[30] = { };
    char hookLog[40] = { };
    char dataName[3][30] = { };
    char dataValue[3][70] = { };
    char dataContent[10][100] = { };
}info;
int count = 0;
info result[50] = { };
char path[256] = { };

typedef void(*ptrresult)(info*, int);
typedef void(*ptrtarget)(char*);
typedef void(*ptrcount)(int&);
HMODULE hMod = LoadLibrary(L"F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\InnjectDll.dll");
ptrresult GetResult = (ptrresult)GetProcAddress(hMod, "GetResult");
ptrtarget AimTarget = (ptrtarget)GetProcAddress(hMod, "AimTarget");
ptrcount GetCount = (ptrcount)GetProcAddress(hMod, "GetCount");

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
    LoadStringW(hInstance, IDC_FILESHOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FILESHOW));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FILESHOW));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FILESHOW);
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
    int i, ii, dnum;
    TEXTMETRIC tm;    //  结构体，用于计算字符的高度、宽度、间距等很多数据
    static int cxChar, cyChar, iVscrollPos, cxClient, cyClient, y;
    int len;
    
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case ID_REFRESH:
                InvalidateRect(hWnd, NULL, TRUE);
                UpdateWindow(hWnd);
                break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_CREATE:  // 窗口创建时，所产生的消息
    {
        HDC hdc;
        hdc = GetDC(hWnd);
        GetTextMetrics(hdc, &tm);
        cxChar = tm.tmAveCharWidth;   //字符平均宽度
        cyChar = tm.tmHeight + tm.tmExternalLeading;  //字符所需的高度
        iVscrollPos = 0;  // 滑块的初始位置
        ReleaseDC(hWnd, hdc);
    }

    case WM_SIZE:  // 窗口大小改变
    {
        cxClient = LOWORD(lParam);   //客户区的宽度
        cyClient = HIWORD(lParam);   // 客户区的高度
        y = cyClient / cyChar;       // 计算一页中有多少行文本
        SetScrollRange(hWnd, SB_VERT, 0, 1000, FALSE);  //创建滚动条的范围
    }

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            AimTarget(path);
            GetCount(count);
            GetResult(result, count);
            // for循环打印文本
            ii = 0;
            for (i = 0; i < count; i++)
            {
                if ((strcmp(result[i].data, "file")) == 0)
                {
                    TextOutA(hdc, 0, (ii - iVscrollPos) * cyChar, " ", 1);
                    ii++;
                    
                    TextOutA(hdc, 0, (ii - iVscrollPos) * cyChar, " ", 1);
                    ii++;
                    
                    TextOutA(hdc, 0, (ii - iVscrollPos) * cyChar, "**********************", 22);
                    ii++;

                    len = strlen(result[i].hookMessage);
                    TextOutA(hdc, 0, (ii - iVscrollPos) * cyChar, result[i].hookMessage, len);
                    ii++;

                    len = strlen(result[i].hookLog);
                    TextOutA(hdc, 0, (ii - iVscrollPos) * cyChar, result[i].hookLog, len);
                    ii++;

                    len = strlen(path);
                    TextOutA(hdc, 0, (ii - iVscrollPos) * cyChar, path, len);
                    ii++;

                    for (dnum = 0; dnum < result[i].dataNum; dnum++) {
                        len = strlen(result[i].dataContent[dnum]);
                        TextOutA(hdc, 0, (ii - iVscrollPos) * cyChar, result[i].dataContent[dnum], len);
                        ii++;
                    }

                    TextOutA(hdc, 0, (ii - iVscrollPos) * cyChar, "**********************", 22);
                    ii++;

                    TextOutA(hdc, 0, (ii - iVscrollPos) * cyChar, " ", 1);
                    ii++;

                    TextOutA(hdc, 0, (ii - iVscrollPos) * cyChar, " ", 1);
                    ii++;
                }
            }

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_VSCROLL:
    {
        switch (LOWORD(wParam))
        {
        case SB_LINEDOWN:
            iVscrollPos += 1;
            break;
        case SB_LINEUP:
            iVscrollPos -= 1;
            break;
        case SB_PAGEDOWN:
            iVscrollPos += y;
            break;
        case SB_PAGEUP:
            iVscrollPos -= y;
            break;
        case SB_THUMBTRACK:
            iVscrollPos = HIWORD(wParam);
            break;
        }
        iVscrollPos = min(1000, max(0, iVscrollPos));  //确保滑块不超出滚动条的范围
        if (iVscrollPos != GetScrollPos(hWnd, SB_VERT))
        {
            SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE); // 设置滚动条的位置
            InvalidateRect(hWnd, NULL, TRUE);   //发送WM_PAINT消息，更新窗口
        }
    }
    break;

    case WM_KEYDOWN:                //若是键盘按下消息
    {
        if (wParam == VK_ESCAPE)    //若是ESC键
            DestroyWindow(hWnd);    //摧毁窗口并发送一条WM_DESTROY消息
        break;
    }

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
