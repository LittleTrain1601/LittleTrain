#include "TrainGUI.h"

int programStat = 1;
int frameStat = 0;

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


		SetWorkingImage();
		putimage(0, 44, &track);
		putimage(694, 44, &info);
		putimage(694, 424, &statLayer);
		putimage(0, 0, &captain);
		if (frameStat == 1)
		{
			//alertQuit();
		}
		else if (frameStat == 2)
		{
			//alertAsk();
		}
	}

	_endthreadex(0);
	return 0;
}