// APPSTART.cpp : ����Ӧ�ó������ڵ㡣
//

#include "TrainGUI.h"
extern "C" {
#include "train.h"
};


int windowWidth = 960;
int windowHeight = 560;

void cutBoard(HWND window);//�����򴰿ڼ��������пͻ���

//������
HANDLE hMutex;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	initgraph(windowWidth, windowHeight);
	// ��ȡ���ھ��
	HWND hWnd = GetHWnd();
	cutBoard(hWnd);
	IMAGE APP_Start;
	loadimage(&APP_Start, _T("./Res/APP_START.jpg"));
	putimage(0, 0, &APP_Start);
	Sleep(500);//�ȴ�0.5��
	
	//��������ͼ�κʹ������������߳�
	HANDLE GUIInputThread, GUIOutputThread;
	hMutex = CreateMutex(NULL, FALSE, NULL);
	GUIInputThread = (HANDLE)_beginthreadex(NULL, 0, GUIInput, NULL, 0, NULL);
	GUIOutputThread = (HANDLE)_beginthreadex(NULL, 0, GUIOutput, NULL, 0, NULL);

	MOUSEMSG m;//�ȴ���������˳�����
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