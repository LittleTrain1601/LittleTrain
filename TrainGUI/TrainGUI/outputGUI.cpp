#include "TrainGUI.h"
#include <math.h>


int programStat = 1;
int frameStat = 0;
int pauseStat = 1;
char currentmode = 'T';  //指示右侧模块变更:T为火车模块，S为站点模块，P为公共轨道模块 
int trainnumber = 0; //右侧模块显示的是那辆车的信息
int totaltrain;//火车总数 
int stationnumber;//用户点击的站点id
int totalstation;//站点总数
int branchnumber;// 用户点击的公共轨道对应的某一分叉节点id
int totalbranch;//分叉节点总数 
int nodeWidth = 22;
int nodeHeight = 22;

//TCHAR字符串缓存
TCHAR TBuff[100];

//用作图层
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
IMAGE lightIco;

HANDLE setWorkingImageMutex;

void alertQuit();
void alertAsk();
void alertExit();
void drawTrack(IMAGE *pImg);
void putTrains(IMAGE *pImg);
int lastIndex(int trainID, int currentID);

unsigned __stdcall GUIOutput(void* pArguments) {
	//为每一个图层载入图像，修改图层时注意备份
	setWorkingImageMutex = CreateMutex(NULL, FALSE, NULL);
	loadimage(&captain, _T("./Res/CAPTAIN.jpg"));
	loadimage(&info, _T("./Res/TRAIN_MODE.jpg"));
	loadimage(&statLayer, _T("./Res/POLICY.jpg"));
	loadimage(&quit, _T("./Res/APP_QUIT.jpg"));
	loadimage(&exitLayer, _T("./Res/APP_EXIT.jpg"));
	loadimage(&trainIco1, _T("./Res/GUICHU1.jpg"));
	loadimage(&trainIco2, _T("./Res/GUICHU2.jpg"));
	loadimage(&lightIco, _T("./Res/LIGHT.jpg"));
	loadimage(&stationIco, _T("./Res/STATION.jpg"));
	//绘制轨道区背景
	//WaitForSingleObject(setWorkingImageMutex, INFINITE);
	SetWorkingImage(&track); 
	setfillcolor(WHITE);
	setlinecolor(WHITE);
	fillrectangle(0, 0, 694, 516);
	//绘制询问窗口，不包含文字
	SetWorkingImage(&ask);
	setfillcolor(WHITE);
	setlinecolor(WHITE);
	fillrectangle(0, 0, 301, 159);
	//绘制左侧按钮
	setfillcolor(RGB(196, 196, 196));
	setlinecolor(RGB(196, 196, 196));
	fillrectangle(17, 115, 148, 143);
	//绘制右侧按钮
	setfillcolor(RGB(196, 196, 196));
	setlinecolor(RGB(196, 196, 196));
	fillrectangle(154, 115, 285, 143);
	//ReleaseMutex(setWorkingImageMutex);
	//绘制轨道
	IMAGE trackStill(694, 516);
	drawTrack(&trackStill);
	IMAGE infoTrain;
	loadimage(&infoTrain, _T("./Res/TRAIN_MODE.jpg"));
	IMAGE infoStation;
	loadimage(&infoStation, _T("./Res/STATION_MODE.jpg"));
	IMAGE infoBranch;
	loadimage(&infoBranch, _T("./Res/BRANCH_MODE.jpg"));
	IMAGE infoPolicy;
	loadimage(&infoPolicy, _T("./Res/POLICY.jpg"));



	while (programStat)
	{
		WaitForSingleObject(hMutex, INFINITE);
		//更新图层内容
		putTrains(&trackStill);
		SetWorkingImage(&info); //x=694, y=44
		LOGFONT f;
		switch (currentmode)
		{
		case 'T':
			putimage(0, 0, &infoTrain);
			//RECT trainID = {91, 63, 120, 92};
			_stprintf(TBuff, _T("%d"), trainnumber);
			settextcolor(BLACK);
			gettextstyle(&f);
			f.lfHeight = 18;
			f.lfQuality = ANTIALIASED_QUALITY;
			settextstyle(&f);
			setbkmode(TRANSPARENT);
			outtextxy(91, 67, TBuff);
			//drawtext(TBuff, &trainID, DT_WORDBREAK);
			gettextstyle(&f);
			f.lfHeight = 22;
			f.lfQuality = ANTIALIASED_QUALITY;
			settextstyle(&f);
			settextcolor(RGB(91, 166, 210));
			if (pauseStat)
			{
				_stprintf(TBuff, _T("全部开始"));
			}
			else 
			{
				_stprintf(TBuff, _T("全部暂停"));
			}
			outtextxy(16, 24, TBuff);
			f.lfHeight = 18;
			settextstyle(&f);
			if (trainList[trainnumber]->status == RUN)
			{
				_stprintf(TBuff, _T("暂停这辆车"));
				outtextxy(16, 109, TBuff);
			}
			else if (trainList[trainnumber]->status == PAUSE)
			{
				_stprintf(TBuff, _T("启动这辆车"));
				outtextxy(16, 109, TBuff);
			}
			settextcolor(BLACK);
			_stprintf(TBuff, _T("%d 米/秒"), trainList[trainnumber]->v);
			outtextxy(93, 151, TBuff);
			if (trainList[trainnumber]->direction == clockwise)
			{
				_stprintf(TBuff, _T("顺时针"));
			}
			else
			{
				_stprintf(TBuff, _T("逆时针"));
			}
			outtextxy(93, 185, TBuff);
			_stprintf(TBuff, _T("程序运行时间：%lu 秒"), RUN_TIME/CLOCKS_PER_SEC);
			outtextxy(16, 360, TBuff);
			break;
		case 'S':
		{
			putimage(0, 0, &infoStation);
			_stprintf(TBuff, _T("车站%d"), stationnumber);
			settextcolor(BLACK);
			gettextstyle(&f);
			f.lfHeight = 22;
			f.lfQuality = ANTIALIASED_QUALITY;
			settextstyle(&f);
			setbkmode(TRANSPARENT);
			outtextxy(16, 66, TBuff);
			f.lfHeight = 18;
			settextstyle(&f);
			int i = 0, flag = 1;
			queueNode currentMission;
			for (int j = 0; j < totaltrain; j++)
			{
				currentMission = trainList[j]->mission->head;
				while (currentMission->next)
				{
					currentMission = currentMission->next;
					if (((trainQueueNode)currentMission->data)->type == TSTATION && ((trainQueueNode)currentMission->data)->station == stationnumber && i < 5)
					{
						_stprintf(TBuff, _T("火车%d 停%d秒"), j, ((trainQueueNode)currentMission->data)->time);
						outtextxy(16, 146 + 22 * i, TBuff);
						i++;
					}
					else if (i == 5 && flag)
					{
						_stprintf(TBuff, _T("……"));
						outtextxy(16, 146 + 22 * i, TBuff);
						flag = 0;
						break;
					}
				}
			}
			_stprintf(TBuff, _T("%d"), trainnumber);
			outtextxy(90, 312, TBuff);
		}
		break;
		case 'P':
			putimage(0, 0, &infoBranch);
			int id1, id2, idt=-1;
			id1 = branchnumber;
			id2 = trackNodeList[id1]->branch.pair->id;
			idt = trackNodeList[id1]->branch.innerTrain;
			_stprintf(TBuff, _T("分叉点%d和分叉点%d之间"), id1, id2);
			settextcolor(BLACK);
			gettextstyle(&f);
			f.lfHeight = 18;
			f.lfQuality = ANTIALIASED_QUALITY;
			settextstyle(&f);
			setbkmode(TRANSPARENT);
			outtextxy(16, 71, TBuff);
			//寻找占用轨道的火车
			if (idt != -1)
			{
				if (checkTrack(idt, id1, id2))
				{
					_stprintf(TBuff, _T("被火车%d占用"), idt);
				}
				else
				{
					_stprintf(TBuff, _T("空闲"));
				}
			}
			else
			{
				_stprintf(TBuff, _T("空闲"));
			}
			outtextxy(16, 93, TBuff);
			break;
		}
		SetWorkingImage(&statLayer);
		putimage(0, 0, &infoPolicy);
		if (servicePolicy == BYTHEWAY)
		{
			_stprintf(TBuff, _T("顺便服务"));
		}
		else
		{
			_stprintf(TBuff, _T("顺序服务"));
		}
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);
		outtextxy(57, 54, TBuff);
		if (controlPolicy == MANUAL)
		{
			_stprintf(TBuff, _T("人工控制"));
		}
		else
		{
			_stprintf(TBuff, _T("自动控制"));
		}
		outtextxy(57, 90, TBuff);
		//绘制图层
		
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
		ReleaseMutex(hMutex);
	}

	_endthreadex(0);
	return 0;
}

void patintFullWindowShadow()
{
	// 获取绘图窗口的显存指针
	DWORD* pbWnd = GetImageBuffer();
	// 计算原图片每个点的颜色，实现逐渐变量的效果
	int r, g, b;
	int light = 32;
	for (int i = 0; i < 960 * 560; i++)
	{
		r = (int)(GetRValue(pbWnd[i]) * light >> 6);
		g = (int)(GetGValue(pbWnd[i]) * light >> 6);
		b = (int)(GetBValue(pbWnd[i]) * light >> 6);
		pbWnd[i] = RGB(r, g, b);
	}

	// 使之前的显存操作生效（注：操作指向 IMAGE 的显存时不需要这条语句）
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
	RECT tochooseRgn = { 346, 219, 346+264, 219+85 };
	CharToTchar(tochoose, TBuff);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	drawtext(TBuff, &tochooseRgn, DT_WORDBREAK);
	RECT firstchoiceRgn = {346, 315, 346+131, 315+28 };
	CharToTchar(firstchoice, TBuff);
	drawtext(TBuff, &firstchoiceRgn, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	RECT secondchoiceRgn = {483, 315, 483+131, 315+28};
	CharToTchar(secondchoice, TBuff);
	drawtext(TBuff, &secondchoiceRgn, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}

POINT getlocation(double distance, int x1, int y1, int x2, int y2) {
	//从点1到点2
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
	//WaitForSingleObject(setWorkingImageMutex, INFINITE);
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
		previousX = currentNode->x;
		previousY = currentNode->y;
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
	//ReleaseMutex(setWorkingImageMutex);
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
		j = lastIndex(i, front->id);

		behind = trackNodeList[trainList[i]->nodeList[j]];
		trainCor = getlocation(trainList[i]->distance, behind->x, behind->y, front->x, front->y);
		putimage(trainCor.x - nodeWidth / 2, trainCor.y - nodeHeight / 2, ptrainIco);
		//红绿灯距离交叉点22m
		if (trainList[i]->flag == forbidden)
		{
			trainCor = getlocation(22, behind->x, behind->y, front->x, front->y);
			putimage(trainCor.x - nodeWidth / 2, trainCor.y - nodeHeight / 2, &lightIco);
		}
	}
}

int lastIndex(int trainID, int currentID) {
	int i;
	int nodeid = currentID;
	for (i = 0; nodeid != trainList[trainID]->nodeList[i]; i++) {
		if (trainList[trainID]->nodeList[i] == -1) {
			i = 0;
		}
	}
	if (trainList[trainID]->direction == anticlockwise) {
		if (trainList[trainID]->nodeList[++i] == -1) {
			i = 0;
		}
	}
	else {
		if (i == 0) {
			for (; trainList[trainID]->nodeList[i] != -1; i++) {}
			i--;
		}
		else {
			i--;
		}
	}
	return i;
}

void CharToTchar(const char * _char, TCHAR * tchar)
{
	int iLength;

	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}
