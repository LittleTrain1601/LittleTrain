#include "TrainGUI.h"


int programStat = 1;
int frameStat = 0;
char currentmode = 'T';  //ָʾ�Ҳ�ģ����:TΪ��ģ�飬SΪվ��ģ�飬PΪ�������ģ�� 
int trainnumber = 0; //�Ҳ�ģ����ʾ��������������Ϣ
int totaltrain;//������ 
int stationnumber;//�û������վ��id
int totalstation;//վ������
int branchnumber;// �û�����Ĺ��������Ӧ��ĳһ�ֲ�ڵ�id
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
		for (int i = 0; trainList[i] != -1; i++) 
		{
			int j ;
			for (j = 0; nodeList[j] != -1;j++)
			{
				int id = nodeList[j];//idΪ�ڵ�id
				line(trackNodeList[id]->x, trackNodeList[id]->y, trackNodeList[id+1]->x, trackNodeList[id+1]->y);//����Ļ���

			}
		}
			
		
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

 POINT getlocation(double distance, int x1, int y1, int x2, int y2) {
	POINT current;
	 current->x=  x2  -(x2-x1)*(int) distance / (sqr((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)));
	current->y=y2 -(y2-y1)*(int)distance / (sqr((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)));
	return current;
}