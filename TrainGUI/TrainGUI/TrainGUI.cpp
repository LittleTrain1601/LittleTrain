// APPSTART.cpp : ����Ӧ�ó������ڵ㡣
//

#include "TrainGUI.h"
#include <io.h>



int windowWidth = 960;
int windowHeight = 560;

void cutBoard(HWND window);//�����򴰿ڼ��������пͻ���
int selectTrack();//����1��ʾĬ�Ϲ��������2��ʾ�Զ�����

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
	frameStat = 0;
	//ѡ�����ù������init.txt�еĹ��
	selectTrack();


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

int selectTrack()
{
	SetWorkingImage();
	IMAGE pannel;
	if (access("./init.txt", 0))
	{
		loadimage(&pannel, _T("./Res/SELECT_TRACK.jpg"));
	}
	else
	{
		loadimage(&pannel, _T("./Res/SELECT_TRACK2.jpg"));
	}
	putimage(0, 0, &pannel);
	IMAGE backup;
	IMAGE closeHover;
	loadimage(&closeHover, _T("./Res/QUIT_HOVER.jpg"));
	getimage(&backup, 0, 0, 960, 560);
	int chosen = 0;
	MOUSEMSG m;
	while (!chosen)
	{
		// ��ȡһ�������Ϣ
		m = GetMouseMsg();

		switch (m.uMsg)
		{
		case WM_MOUSEMOVE:
			if (m.x > 916 && m.x < 960 && m.y > 0 && m.y < 44)
			{
				BeginBatchDraw();
				putimage(0, 0, &backup);
				putimage(916, 0, &closeHover);
				FlushBatchDraw();
				EndBatchDraw();
			}
			else
			{
				putimage(0, 0, &backup);
			}
			break;

		case WM_LBUTTONDOWN:
			if (m.x > 916 && m.x < 960 && m.y > 0 && m.y < 44)
			{
				programStat = 0;;
			}
			else if (m.x > 414 && m.y > 291 && m.x < 546 && m.y < 335)
			{
				chosen = 1;
			}
			else if (m.x > 414 && m.y > 380 && m.x < 546 && m.y < 424)
			{
				chosen = 2;
			}
			break;
		}

	}
	buildDefault();
	return chosen;
}