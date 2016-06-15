#include "TrainGUI.h"

int programStat = 1;
int frameStat = 0;
char currentmode = 'T';  //指示右侧模块变更:T为火车模块，S为站点模块，P为公共轨道模块 
int trainnumber; //序号为0,1,2的三辆车
int stationnumber;//序号从3开始的站点
int totalstation;//站点总数
int branchnumber;// 序号从3+totalstation-1开始
int totalbranch;//分叉节点总数 
//用作图层
IMAGE captain;
IMAGE track(694, 516);
IMAGE info;
IMAGE statLayer;
IMAGE quit;
IMAGE ask(301, 159);
void alertQuit();
void alertAsk();

unsigned __stdcall GUIOutput(void* pArguments) {
	//为每一个图层载入图像，修改图层时注意备份
	loadimage(&captain, _T("./Res/CAPTAIN.jpg"));
	loadimage(&info, _T("./Res/TRAIN_MODE.jpg"));
	loadimage(&statLayer, _T("./Res/POLICY.jpg"));
	loadimage(&quit, _T("./Res/APP_QUIT.jpg"));
	SetWorkingImage(&track); //绘制轨道区背景
	setfillcolor(WHITE);
	setlinecolor(WHITE);
	fillrectangle(0, 0, 694, 516);
	SetWorkingImage(&ask);//绘制询问窗口，不包含文字
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