#include "TrainGUI.h"
#include <math.h>


int programStat = 1;
int frameStat = 0;
int pauseStat = 1;
char currentmode = 'T';  //ָʾ�Ҳ�ģ����:TΪ��ģ�飬SΪվ��ģ�飬PΪ�������ģ�� 
int trainnumber = 0; //�Ҳ�ģ����ʾ��������������Ϣ
int totaltrain;//������ 
int stationnumber;//�û������վ��id
int totalstation;//վ������
int branchnumber;// �û�����Ĺ��������Ӧ��ĳһ�ֲ�ڵ�id
int totalbranch;//�ֲ�ڵ����� 
int nodeWidth = 22;
int nodeHeight = 22;

//����ͼ��
IMAGE captain;
IMAGE track(694, 516);
IMAGE info;
IMAGE statLayer;
IMAGE quit;
IMAGE ask(301, 159);
IMAGE exitLayer;
IMAGE stationIco;
IMAGE trainIco1;
IMAGE trainIco2;

void alertQuit();
void alertAsk();
void alertExit();
void drawTrack(IMAGE *pImg);
void putTrains(IMAGE *pImg);

unsigned __stdcall GUIOutput(void* pArguments) {
	//Ϊÿһ��ͼ������ͼ���޸�ͼ��ʱע�ⱸ��
	loadimage(&captain, _T("./Res/CAPTAIN.jpg"));
	loadimage(&info, _T("./Res/TRAIN_MODE.jpg"));
	loadimage(&statLayer, _T("./Res/POLICY.jpg"));
	loadimage(&quit, _T("./Res/APP_QUIT.jpg"));
	loadimage(&exitLayer, _T("./Res/APP_QUIT.jpg"));
	loadimage(&trainIco1, _T("./Res/GUICHU1.jpg"));
	loadimage(&trainIco2, _T("./Res/GUICHU2.jpg"));
	//���ƹ��������
	SetWorkingImage(&track); 
	setfillcolor(WHITE);
	setlinecolor(WHITE);
	fillrectangle(0, 0, 694, 516);
	//����ѯ�ʴ��ڣ�����������
	SetWorkingImage(&ask);
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

	//���ƹ��
	IMAGE trackStill(694, 516);
	drawTrack(&trackStill);
	IMAGE infoTrain;
	loadimage(&infoTrain, _T("./Res/TRAIN_MODE.jpg"));
	IMAGE infoStation;
	loadimage(&infoStation, _T("./Res/STATION_MODE.jpg"));
	IMAGE infoBranch;
	loadimage(&infoBranch, _T("./Res/BRANCH_MODE.jpg"));

	//TCHAR�ַ�������
	TCHAR TBuff[100];

	while (programStat)
	{
		//WaitForSingleObject(hMutex, INFINITE);
		//����ͼ������
		putTrains(&trackStill);
		SetWorkingImage(&info); //x=694, y=44
		switch (currentmode)
		{
		case 'T':
			putimage(0, 0, &infoTrain);
			RECT trainID = {91, 63, 120, 92};
			_stprintf(TBuff, _T("%d"), trainnumber);
			SetTextColor(BLACK);
			outtextxy(91, 63, TBuff);
			//drawtext(TBuff, &trainID, DT_WORDBREAK);
			LOGFONT f;
			gettextstyle(&f);
			f.lfHeight = 22;
			f.lfQuality = ANTIALIASED_QUALITY;
			settextstyle(&f);
			settextcolor(RGB(91, 166, 210));
			if (pauseStat)
			{
				_stprintf(TBuff, _T("ȫ����ʼ"));
			}
			else 
			{
				_stprintf(TBuff, _T("ȫ����ͣ"));
			}
			outtextxy(16, 24, TBuff);
			f.lfHeight = 18;
			settextstyle(&f);
			if (trainList[trainnumber]->status == RUN)
			{
				_stprintf(TBuff, _T("��ͣ������"));
				outtextxy(16, 109, TBuff);
			}
			else if (trainList[trainnumber]->status == PAUSE)
			{
				_stprintf(TBuff, _T("����������"));
				outtextxy(16, 109, TBuff);
			}
			SetTextColor(BLACK);
			_stprintf(TBuff, _T("%d ��/��"), trainList[trainnumber]->v);
			outtextxy(93, 151, TBuff);
			if (trainList[trainnumber]->direction == clockwise)
			{
				_stprintf(TBuff, _T("˳ʱ��"));
			}
			else
			{
				_stprintf(TBuff, _T("��ʱ��"));
			}
			outtextxy(93, 151, TBuff);
			break;
		case 'S':
			putimage(0, 0, &infoStation);
			_stprintf(TBuff, _T("��վ%d"), stationnumber);
			settextcolor(BLACK);
			LOGFONT f;
			gettextstyle(&f);
			f.lfHeight = 22;
			f.lfQuality = ANTIALIASED_QUALITY;
			settextstyle(&f);
			outtextxy(16, 66, TBuff);
			f.lfHeight = 18;
			settextstyle(&f);
			int i = 0, flag=1;
			queueNode currentMission;
			for (int j = 0; j < totaltrain; j++)
			{
				currentMission = trainList[j]->mission->head;
				while (currentMission -> next)
				{
					currentMission = currentMission->next;
					if (((trainQueueNode)currentMission)->type == TSTATION && ((trainQueueNode)currentMission)->station == stationnumber && i<5)
					{
						_stprintf(TBuff, _T("��%d ͣ%d��"), j, ((trainQueueNode)currentMission)->time);
						outtextxy(16, 146 + 22 * i, TBuff);
					}
					else if (i == 5 && flag)
					{
						_stprintf(TBuff, _T("����"));
						outtextxy(16, 146 + 22 * i, TBuff);
						flag = 0;
						break;
					}
				}
			}
			_stprintf(TBuff, _T("%d"), trainnumber);
			outtextxy(90, 312, TBuff);
			break;
		case 'P':
			int id1, id2, idt=-1;
			id1 = branchnumber;
			id2 = trackNodeList[id1]->branch.pair->id;
			_stprintf(TBuff, _T("�ֲ��%d�ͷֲ��%d֮��"), id1, id2);
			settextcolor(BLACK);
			LOGFONT f;
			gettextstyle(&f);
			f.lfHeight = 18;
			f.lfQuality = ANTIALIASED_QUALITY;
			settextstyle(&f);
			outtextxy(16, 71, TBuff);
			//Ѱ��ռ�ù���Ļ�
			int front,behind,j, i;
			for (i = 0; i < totaltrain; i++)
			{
				front = trainList[i]->nextNode;
				if (front == id1 || front == id2)
				{
					for (j = 0; trainList[i]->nodeList[j] != -1; j++)
					{
						if (front == trackNodeList[trainList[i]->nodeList[j]]->id)
						{
							break;
						}
					}
					j--;
					if (j == 0)
					{
						for (j = 0; trainList[i]->nodeList[j] != -1; j++);
						j--;
					}
					behind = j;
					if (behind == id1 || behind == id2)
					{
						idt = i;
					}
				}
			}
			checkTrack();
			break;
		default:
			break;
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
			alertAsk();
		}
		else if (frameStat == 3)
		{
			alertExit();
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
	patintFullWindowShadow();
	SetWorkingImage();
	putimage(329, 200, &ask);
}

POINT getlocation(double distance, int x1, int y1, int x2, int y2) {
	//�ӵ�1����2
	POINT current;
	int l = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	current.x = x2 - (x2 - x1) * distance / l;
	current.y = y2 - (y2 - y1) * distance / l;
	return current;
}

void alertExit()
{
	patintFullWindowShadow();
	SetWorkingImage();
	putimage(330, 211, &exitLayer);
}

void drawTrack(IMAGE *pImg)
{
	SetWorkingImage(pImg);
	setfillcolor(WHITE);
	setlinecolor(WHITE);
	fillrectangle(0, 0, 694, 516);
	setlinecolor(BLACK);
	int previousX, previousY;
	for (int i = 0; i < totaltrain; i++)
	{	
		trackNode currentNode = trackNodeList[trainList[i]->nodeList[0]];
		switch (currentNode->type)
		{
		case STATION:
			putimage(currentNode->x - currentNode->width/2, currentNode->y - currentNode->height/2, &stationIco);
			break;
		}
		for (int j = 1; trainList[i]->nodeList[j] != -1; j++)
		{
			currentNode = trackNodeList[trainList[i]->nodeList[j]];
			switch (currentNode->type)
			{
			case STATION:
				putimage(currentNode->x - currentNode->width / 2, currentNode->y - currentNode->height / 2, &stationIco);
				break;
			default:
				putpixel(currentNode->x, currentNode->y, BLACK);
				break;
			}
			line(previousX, previousY, currentNode->x, currentNode->y);
			previousX = currentNode->x;
			previousY = currentNode->y;
		}
		currentNode = trackNodeList[trainList[i]->nodeList[0]];
		line(previousX, previousY, currentNode->x, currentNode->y);
	}

}

int GUICHUTimes = 0;

void putTrains(IMAGE *pImg)
{
	int j;
	SetWorkingImage(&track);
	putimage(0, 0, pImg);
	POINT trainCor;
	IMAGE *ptrainIco;
	if (GUICHUTimes)
	{
		ptrainIco = &trainIco1;
		GUICHUTimes = 0;
	}
	else
	{
		ptrainIco = &trainIco2;
		GUICHUTimes = 1;
	}

	for	(int i = 0; i < totaltrain; i++)
	{
		trackNode front, behind;
		front = trackNodeList[trainList[i]->nextNode];
		for (j = 0; trainList[i]->nodeList[j] != -1; j++)
		{
			if (front->id == trackNodeList[trainList[i]->nodeList[j]]->id)
			{
				break;
			}
		}
		j--;
		if (j == 0)
		{
			for (j = 0; trainList[i]->nodeList[j] != -1; j++);
			j--;
		}
		behind = trackNodeList[trainList[i]->nodeList[j]];
		trainCor = getlocation(trainList[i]->distance, behind->x, behind->y, front->x, front->y);
		putimage(trainCor.x - nodeWidth / 2, trainCor.y - nodeHeight / 2, ptrainIco);
	}
}