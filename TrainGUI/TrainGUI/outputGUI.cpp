#include "TrainGUI.h"

int programStat = 1;
int frameStat = 0;
char currentmode = 'T';  //ָʾ�Ҳ�ģ����:TΪ��ģ�飬SΪվ��ģ�飬PΪ�������ģ�� 
int trainnumber = 0; //���Ϊ0,1,2��������
int stationnumber;//��Ŵ�3��ʼ��վ��
int totalstation;//վ������
int branchnumber;// ��Ŵ�3+totalstation-1��ʼ
int totalbranch;//�ֲ�ڵ����� 
//����ͼ��
IMAGE captain;
IMAGE track(694, 516);
IMAGE info;
IMAGE statLayer;
IMAGE quit;
IMAGE ask(301, 159);
void alertQuit();
void alertAsk();

unsigned __stdcall GUIOutput(void* pArguments) {
	//Ϊÿһ��ͼ������ͼ���޸�ͼ��ʱע�ⱸ��
	loadimage(&captain, _T("./Res/CAPTAIN.jpg"));
	loadimage(&info, _T("./Res/TRAIN_MODE.jpg"));
	loadimage(&statLayer, _T("./Res/POLICY.jpg"));
	loadimage(&quit, _T("./Res/APP_QUIT.jpg"));
	SetWorkingImage(&track); //���ƹ��������
	setfillcolor(WHITE);
	setlinecolor(WHITE);
	fillrectangle(0, 0, 694, 516);
	SetWorkingImage(&ask);//����ѯ�ʴ��ڣ�����������
	setfillcolor(WHITE);
	setlinecolor(WHITE);
	fillrectangle(0, 0, 301, 159);
	//������ఴť
	setfillcolor(RGB(196, 196, 196));
	setlinecolor(RGB(196, 196, 196));
	fillrectangle(17, 115, 148, 143);
	//�����Ҳఴť
	setfillcolor(RGB(196, 196, 196));
	setlinecolor(RGB(196, 196, 196));
	fillrectangle(154, 115, 285, 115);

	while (programStat)
	{
		//WaitForSingleObject(hMutex, INFINITE);
		//����ͼ������

		//����ͼ��
		
		SetWorkingImage();
		BeginBatchDraw();
		putimage(0, 44, &track);
		putimage(694, 44, &info);
		putimage(694, 424, &statLayer);
		putimage(0, 0, &captain);
		if (frameStat == 1)
		{
			alertQuit();
		}
		else if (frameStat == 2)
		{
			//alertAsk();
		}
		FlushBatchDraw();
		EndBatchDraw();
		//ReleaseMutex(hMutex);
	}

	_endthreadex(0);
	return 0;
}

void patintFullWindowShadow()
{
	// ��ȡ��ͼ���ڵ��Դ�ָ��
	DWORD* pbWnd = GetImageBuffer();
	// ����ԭͼƬÿ�������ɫ��ʵ���𽥱�����Ч��
	int r, g, b;
	int light = 32;
	for (int i = 0; i < 960 * 560; i++)
	{
		r = (int)(GetRValue(pbWnd[i]) * light >> 6);
		g = (int)(GetGValue(pbWnd[i]) * light >> 6);
		b = (int)(GetBValue(pbWnd[i]) * light >> 6);
		pbWnd[i] = RGB(r, g, b);
	}

	// ʹ֮ǰ���Դ������Ч��ע������ָ�� IMAGE ���Դ�ʱ����Ҫ������䣩
	//FlushBatchDraw();
}
void alertQuit()
{
	patintFullWindowShadow();
	SetWorkingImage();
	putimage(330, 211, &quit);
}

void alertAsk()
{

}