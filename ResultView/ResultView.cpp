// ResultView.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "ResultView.h"
#include "windows.h"

#define MAX_LOADSTRING 100

STARTUPINFO si = { sizeof(si) };
PROCESS_INFORMATION pi;
TCHAR szCommandLine1[] = TEXT("F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\MsgShow.exe");
TCHAR szCommandLine2[] = TEXT("F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\HeapShow.exe");
TCHAR szCommandLine3[] = TEXT("F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\FileShow.exe");
TCHAR szCommandLine4[] = TEXT("F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\RegShow.exe");
TCHAR szCommandLine5[] = TEXT("F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\HeapAnalysisShow.exe");
TCHAR szCommandLine6[] = TEXT("F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\FileAnalysisShow.exe");
TCHAR szCommandLine7[] = TEXT("F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\RegAnalysisShow.exe");
TCHAR szCommandLine8[] = TEXT("F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\SocketShow.exe");

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
    LoadStringW(hInstance, IDC_RESULTVIEW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RESULTVIEW));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RESULTVIEW));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_RESULTVIEW);
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
    HWND hStatic1, hStatic2, hStatic3;
    static HWND hBtn1, hBtn2, hBtn3, hBtn4, hBtn5, hBtn6, hBtn7, hBtn8;
    static HFONT hFont;
    
    switch (message)
    {
    case WM_CREATE:
        //创建逻辑字体
        hFont = CreateFontW(
            -15/*高度*/, -8/*宽度*/, 0, 0, 400 /*一般这个值设为400*/,
            FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
            DEFAULT_CHARSET, //使用默认字符集
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, //这行参数不用管
            DEFAULT_QUALITY, //默认输出质量
            FF_DONTCARE, //不指定字体族
            TEXT("Times New Roman") //字体名
        );

        hStatic1 = CreateWindow(TEXT("static"), L"Normal operation:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
            20, 20, 150, 20, hWnd,
            (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(hStatic1, WM_SETFONT, (WPARAM)hFont, NULL);

        hBtn1 = CreateWindow(TEXT("button"), //类别
            L"MessageBoxResult",//按钮上的文本
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//样式
            100, 50,//相对的起始坐标
            180, 25,//宽，高
            hWnd, (HMENU)2,//可以通过LOWORD (wParam)来获取，识别哪个按钮发送消息，当然还有其它方式
            ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(hBtn1, WM_SETFONT, (WPARAM)hFont, NULL);//设置按钮字体
        
        hBtn2 = CreateWindow(TEXT("button"), //类别
            L"HeapResult",//按钮上的文本
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//样式
            300, 50,//相对的起始坐标
            180, 25,//宽，高
            hWnd, (HMENU)3,//可以通过LOWORD (wParam)来获取，识别哪个按钮发送消息，当然还有其它方式
            ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(hBtn2, WM_SETFONT, (WPARAM)hFont, NULL);//设置按钮字体

        hBtn3 = CreateWindow(TEXT("button"), //类别
            L"FileResult",//按钮上的文本
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//样式
            500, 50,//相对的起始坐标
            180, 25,//宽，高
            hWnd, (HMENU)4,//可以通过LOWORD (wParam)来获取，识别哪个按钮发送消息，当然还有其它方式
            ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(hBtn3, WM_SETFONT, (WPARAM)hFont, NULL);//设置按钮字体

        hBtn4 = CreateWindow(TEXT("button"), //类别
            L"RegResult",//按钮上的文本
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//样式
            700, 50,//相对的起始坐标
            180, 25,//宽，高
            hWnd, (HMENU)5,//可以通过LOWORD (wParam)来获取，识别哪个按钮发送消息，当然还有其它方式
            ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(hBtn4, WM_SETFONT, (WPARAM)hFont, NULL);//设置按钮字体

        hStatic2 = CreateWindow(TEXT("static"), L"Malicious operation:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
            20, 90, 170, 20, hWnd,
            (HMENU)6, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(hStatic2, WM_SETFONT, (WPARAM)hFont, NULL);

        hBtn5 = CreateWindow(TEXT("button"), //类别
            L"HeapAnalysis",//按钮上的文本
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//样式
            100, 120,//相对的起始坐标
            180, 25,//宽，高
            hWnd, (HMENU)7,//可以通过LOWORD (wParam)来获取，识别哪个按钮发送消息，当然还有其它方式
            ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(hBtn5, WM_SETFONT, (WPARAM)hFont, NULL);//设置按钮字体

        hBtn6 = CreateWindow(TEXT("button"), //类别
            L"FileAnalysis",//按钮上的文本
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//样式
            300, 120,//相对的起始坐标
            180, 25,//宽，高
            hWnd, (HMENU)8,//可以通过LOWORD (wParam)来获取，识别哪个按钮发送消息，当然还有其它方式
            ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(hBtn6, WM_SETFONT, (WPARAM)hFont, NULL);//设置按钮字体

        hBtn7 = CreateWindow(TEXT("button"), //类别
            L"RegAnalysis",//按钮上的文本
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//样式
            500, 120,//相对的起始坐标
            180, 25,//宽，高
            hWnd, (HMENU)9,//可以通过LOWORD (wParam)来获取，识别哪个按钮发送消息，当然还有其它方式
            ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(hBtn7, WM_SETFONT, (WPARAM)hFont, NULL);//设置按钮字体

        hStatic3 = CreateWindow(TEXT("static"), L"Extra operation:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
            20, 160, 135, 20, hWnd,
            (HMENU)10, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(hStatic3, WM_SETFONT, (WPARAM)hFont, NULL);

        hBtn8 = CreateWindow(TEXT("button"), //类别
            L"SocketResult",//按钮上的文本
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//样式
            100, 190,//相对的起始坐标
            180, 25,//宽，高
            hWnd, (HMENU)11,//可以通过LOWORD (wParam)来获取，识别哪个按钮发送消息，当然还有其它方式
            ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        SendMessage(hBtn8, WM_SETFONT, (WPARAM)hFont, NULL);//设置按钮字体

        break;
    
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            /*case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;*/
            case 2:
                CreateProcess(NULL, szCommandLine1, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
                break;
            case 3:
                CreateProcess(NULL, szCommandLine2, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
                break;
            case 4:
                CreateProcess(NULL, szCommandLine3, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
                break;
            case 5:
                CreateProcess(NULL, szCommandLine4, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
                break;
            case 7:
                CreateProcess(NULL, szCommandLine5, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
                break;
            case 8:
                CreateProcess(NULL, szCommandLine6, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
                break;
            case 9:
                CreateProcess(NULL, szCommandLine7, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
                break;
            case 11:
                CreateProcess(NULL, szCommandLine8, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_KEYDOWN:                //若是键盘按下消息
    {
        if (wParam == VK_ESCAPE)    //若是ESC键
            DestroyWindow(hWnd);    //摧毁窗口并发送一条WM_DESTROY消息
        break;
    }
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
