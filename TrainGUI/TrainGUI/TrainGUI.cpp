// APPSTART.cpp : 定义应用程序的入口点。
//

#include "TrainGUI.h"
extern "C" {
#include "train.h"
};


int windowWidth = 960;
int windowHeight = 560;

void cutBoard(HWND window);//将程序窗口剪裁至仅有客户区

//互斥锁
HANDLE hMutex;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	initgraph(windowWidth, windowHeight);
	// 获取窗口句柄
	HWND hWnd = GetHWnd();
	cutBoard(hWnd);
	IMAGE APP_Start;
	loadimage(&APP_Start, _T("./Res/APP_START.jpg"));
	putimage(0, 0, &APP_Start);
	Sleep(500);//等待0.5秒
	
	//建立绘制图形和处理鼠标输入的线程
	HANDLE GUIInputThread, GUIOutputThread;
	hMutex = CreateMutex(NULL, FALSE, NULL);
	GUIInputThread = (HANDLE)_beginthreadex(NULL, 0, GUIInput, NULL, 0, NULL);
	GUIOutputThread = (HANDLE)_beginthreadex(NULL, 0, GUIOutput, NULL, 0, NULL);

	MOUSEMSG m;//等待鼠标点击后退出程序
	//WaitForSingleObject(hMutex, INFINITE);
	while (programStat)
	{
		m = GetMouseMsg();
		//WaitForSingleObject(hMutex, INFINITE);

		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (frameStat == 0)
			{
				frameStat = 1;
			}
			else if (frameStat == 1)
			{
				programStat = 0;
			}
			break;
		default:
			break;
		}

		//ReleaseMutex(hMutex);
	}

	WaitForSingleObject(GUIInput, INFINITE);
	WaitForSingleObject(GUIOutput, INFINITE);
	closegraph();
	return 0;
}

void cutBoard(HWND window)
{
	HRGN rgn;
	RECT winRec, cliRec;
	POINT winP1, winP2;
	GetWindowRect(window, &winRec);
	GetClientRect(window, &cliRec);
	winP1.x = winRec.left;
	winP1.y = winRec.top;
	ScreenToClient(window, &winP1);
	winP2.x = winRec.right;
	winP2.y = winRec.bottom;
	ScreenToClient(window, &winP2);
	winRec.left = winP1.x;
	winRec.top = winP1.y;
	winRec.right = winP2.x;
	winRec.bottom = winP2.y;
	rgn = CreateRectRgn(cliRec.left - winRec.left, cliRec.top - winRec.top, cliRec.right - winRec.left, cliRec.bottom - winRec.top);
	SetWindowRgn(window, rgn, true);
}