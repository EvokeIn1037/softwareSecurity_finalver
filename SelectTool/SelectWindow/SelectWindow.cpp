#include <stdio.h>
#include <windows.h>
#include <string.h>

typedef void(*ptrtarget)(char*);
HMODULE hMod = LoadLibrary(L"F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\InnjectDll.dll");
ptrtarget  GetTarget = (ptrtarget)GetProcAddress(hMod, "GetTarget");

int main()
{
	char inputPath[256];
	int num;

	printf("--------------------------------Here are the files for testing--------------------------------\n");
	printf("1、MyTestApp                            2、TestCode-wrc\n");
	printf("3、TestApp-xmk                          4、TestApp-zzy\n");
	printf("5、TestApp-yhz\n\n");
	printf("\n\nPlease select the number of the file that you want to inject : ");
	scanf_s("%d", &num);

	switch(num)
	{
		case 1:
			strcpy_s(inputPath, 256, "F:\\softwareSecurity\\TestInitialNullForStudent\\InjectDll\\Debug\\TestApp.exe");
			GetTarget(inputPath);
			break;
		case 2:
			strcpy_s(inputPath, 256, "F:\\softwareSecurity\\TestInitialNullForStudent\\TestExe\\TestCode-wrc.exe");
			GetTarget(inputPath);
			break;
		case 3:
			strcpy_s(inputPath, 256, "F:\\softwareSecurity\\TestInitialNullForStudent\\TestExe\\TestApp-xmk.exe");
			GetTarget(inputPath);
			break;
		case 4:
			strcpy_s(inputPath, 256, "F:\\softwareSecurity\\TestInitialNullForStudent\\TestExe\\TestApp-zzy.exe");
			GetTarget(inputPath);
			break;
		case 5:
			strcpy_s(inputPath, 256, "F:\\softwareSecurity\\TestInitialNullForStudent\\TestExe\\TestApp-yhz.exe");
			GetTarget(inputPath);
			break;
	}

	return 0;
}