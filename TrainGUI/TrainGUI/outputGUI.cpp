#include "TrainGUI.h"


int programStat = 1;
int frameStat = 0;
char currentmode = 'T';  //指示右侧模块变更:T为火车模块，S为站点模块，P为公共轨道模块 
int trainnumber = 0; //右侧模块显示的是那辆车的信息
int totaltrain;//火车总数 
int stationnumber;//用户点击的站点id
int totalstation;//站点总数
int branchnumber;// 用户点击的公共轨道对应的某一分叉节点id
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
		//WaitForSingleObject(hMutex, INFINITE);
		//更新图层内容
		for (int i = 0; trainList[i] != -1; i++) 
		{
			int j ;
			for (j = 0; nodeList[j] != -1;j++)
			{
				int id = nodeList[j];//id为节点id
				line(trackNodeList[id]->x, trackNodeList[id]->y, trackNodeList[id+1]->x, trackNodeList[id+1]->y);//轨道的绘制

			}
		}
			
		
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

}

 POINT getlocation(double distance, int x1, int y1, int x2, int y2) {
	POINT current;
	 current->x=  x2  -(x2-x1)*(int) distance / (sqr((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)));
	current->y=y2 -(y2-y1)*(int)distance / (sqr((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)));
	return current;
}