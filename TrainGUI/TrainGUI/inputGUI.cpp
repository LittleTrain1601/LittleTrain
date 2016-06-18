#include "TrainGUI.h"
#include<easyx.h> 
#include<graphics.h>
#include<tchar.h>

extern "C" {
#include "train.h"
#include<stdlib.h>
#include<time.h>
}; 

MOUSEMSG m;		// 定义鼠标消息
int i, k;
int stopTime;
clock_t mpausetime;
clock_t  mresumetime;
clock_t minusmanualtime;
IMAGE back;
IMAGE closeHover;


//判断某站点是否是某小火车轨道上的站点，是返回1，否返回0
int cangivemission(int stationid, int trainid)
{

	for (i = 0; i < 50; i++)
	{
		if (trainList[trainid]->nodeList[i] == stationid)
			return 1;
	}
	return 0;
}

//判断用户在轨道区是否点击了站点或者公共轨道，返回所点的站点id或公共轨道对应的分叉节点id,若未点击站点或公共轨道，返回-1
int pointnodeid(MOUSEMSG m)
{
	if (m.uMsg == WM_LBUTTONDOWN)
	{
		for (i = 0; i < 50; i++)
		{
			if ((m.x>=((trackNodeList[i]->x)-((trackNodeList[i]->width)/2)))&&( m.x<=((trackNodeList[i]->x)+((trackNodeList[i]->width) / 2)))&&( m.y>=((trackNodeList[i]->y)-((trackNodeList[i]->height) / 2)))&& (m.y<=((trackNodeList[i]->y)+((trackNodeList[i]->width) / 2))))
			 {if (trackNodeList[i]->type == BRANCH || trackNodeList[i]->type == STATION)
					return trackNodeList[i]->id;
				else
					return -1;
			}
		}
		return -1;
	}
	else
		return -1;
}

unsigned  __stdcall GUIInput(void* pArguments)
{
	while (programStat)
	{
		//检测鼠标事件并处理
		// 获取一条鼠标消息
		m = GetMouseMsg();
		if (m.uMsg==WM_MOUSEMOVE)
		 { if (m.x > 916 && m.x < 960 && m.y > 0 && m.y < 44)
			{
				loadimage(&closeHover, _T("./Res/QUIT_HOVER.jpg"));
				getimage(&back, 0, 0, 960, 560);

				BeginBatchDraw();
				putimage(0, 0, &back);
				putimage(916, 0, &closeHover);
				FlushBatchDraw();
				EndBatchDraw();
			} }
		else if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (frameStat == 1)
			{
				if (m.x >= 347 && m.x <= 478 && m.y >= 302 && m.y <= 330)
				{
					frameStat = 0;
				}
				else if (m.x >= 347 && m.x <= 521 && m.y >= 258 && m.x <= 280)
				{
					programStat = 0;
				}
			}

			else if (frameStat == 2)
			{
				if (m.x >= 346 && m.x <= 477 && m.y >= 315 && m.y <= 343)
					choiceflag = 1;
				else if (m.x >= 483 && m.x <= 614 && m.y >= 315 && m.y <= 343)
					choiceflag = 2;
				frameStat = 0;
			}
			
			else if (frameStat == 3)
			{
				if (m.x >= 347 && m.x <= 478 && m.y >= 302 && m.y <= 330)
					programStat = 0;
			}
			
			else if (frameStat == 0)
			{
				if (m.x >= 0 && m.x <= 694 && m.y >= 44 && m.y <= 560)             //点击在轨道区
				{
					k = pointnodeid(m);
					if (k != -1)
					{
						if (trackNodeList[k]->type == STATION)
						{
							currentmode = 'S';
							stationnumber = k;
						}
						else if (trackNodeList[k]->type == BRANCH)
						{
							currentmode = 'P';
							branchnumber = k;

						}

					}
				}
				else              //点击不在轨道区
				{

					if (m.x >= 916 && m.x <= 960 && m.y >= 0 && m.y <= 44)
					{
						mainQueueNode mainData;
						mainData = (mainQueueNode)calloc(1, sizeof(struct _mainQueueNode));    //点击关闭
						mainData->type = MLOCK;
						append(mainMission, mainData);
						frameStat == 1;
					}                            //表示应该弹出退出窗口


					else if (m.x >= 711 && m.x <= 740 && m.y >= 473 && m.y <= 502)
					{
						if (servicePolicy == BYTHEWAY)
						{
							servicePolicy = SEQUENCING;


						}
						else
						{
							servicePolicy = BYTHEWAY;


						}
					}

					else if (m.x >= 862 && m.x <= 891 && m.y >= 473 && m.y <= 502)
					{
						if (servicePolicy == BYTHEWAY)
						{
							servicePolicy = SEQUENCING;


						}
						else
						{
							servicePolicy = BYTHEWAY;


						}
					}

					else if (m.x >= 711 && m.x <= 740 && m.y >= 511 && m.y <= 540)
					{
						if (controlPolicy == AUTO)
						{
							controlPolicy = MANUAL;


						}
						else
						{
							controlPolicy = AUTO;


						}
					}

					else if (m.x >= 862 && m.x <= 891 && m.y >= 511 && m.y <= 540)
					{
						if (controlPolicy == AUTO)
						{
							controlPolicy = MANUAL;


						}
						else
						{
							controlPolicy = AUTO;


						}
					}

					else if (m.x >= 709 && m.x <= 819 && m.y >= 68 && m.y <= 94)  //全部开始
					{
						if (currentmode == 'T')
						{
							if (pauseStat == 1)
							{
								mresumetime = clock();
								minusmanualtime += (mresumetime - mpausetime);
								pauseStat = 0;
								for (i = 0; i < totaltrain; i++)
								{
									mainQueueNode mainData;
									mainData = (mainQueueNode)calloc(1, sizeof(struct _mainQueueNode));
									mainData->type = MRESUME;
									mainData->train.id = i;
									append(mainMission, mainData);
								}

							}
						}
					}


					else if (m.x >= 709 && m.x <= 819 && m.y >= 68 && m.y <= 94)   //全部暂停按钮
					{
						if (currentmode == 'T')
						{
							if (pauseStat == 0)
							{
								mpausetime = clock();
								pauseStat = 1;
								for (i = 0; i < totaltrain; i++)
								{
									mainQueueNode mainData;
									mainData = (mainQueueNode)calloc(1, sizeof(struct _mainQueueNode));
									mainData->type = MPAUSE;
									mainData->train.id = i;
									append(mainMission, mainData);
								}
							}
						}
					}

					else if (m.x >= 709 && m.x <= 738 && m.y >= 107 & m.y <= 136)
					{
						if (currentmode == 'T')
						{
							if (trainnumber > 0)
							{
								trainnumber--;

							}
						}
					}

					else if (m.x >= 860 && m.x <= 889 && m.y >= 107 && m.y <= 136)
					{
						if (currentmode == 'T')
						{
							if (trainnumber < totaltrain - 1)
							{
								trainnumber++;

							}
						}
					}

					else if (m.x >= 711 && m.x <= 801 && m.y >= 153 && m.y <= 178)  //暂停这辆车
					{
						if (currentmode == 'T')
						{
							if (trainList[trainnumber]->status == RUN)
							{
								mainQueueNode mainData;
								mainData = (mainQueueNode)calloc(1, sizeof(struct _mainQueueNode));
								mainData->type = MPAUSE;
								mainData->train.id = trainnumber;
								append(mainMission, mainData);
							}
						}
					}

					else if (m.x >= 711 && m.x <= 801 && m.y >= 153 && m.y <= 178)  //开始这辆车按钮
					{
						if (currentmode == 'T')
						{
							if (trainList[trainnumber]->status == PAUSE)
							{
								mainQueueNode mainData;
								mainData = (mainQueueNode)calloc(1, sizeof(struct _mainQueueNode));
								mainData->type = MRESUME;
								mainData->train.id = trainnumber;
								append(mainMission, mainData);
							}
						}
					}

					else if (m.x >= 748 && m.x <= 777 && m.y >= 191 && m.y <= 220)
					{
						if (currentmode == 'T')
						{
							if (trainList[trainnumber]->v > 0)
							{
								trainList[trainnumber]->v--;

							}
						}
					}

					else if (m.x >= 899 && m.x <= 928 && m.y >= 191 && m.y <= 220)
					{
						if (currentmode == 'T')
						{
							if (trainList[trainnumber]->v < 50)
							{
								trainList[trainnumber]->v++;

							}
						}
					}

					else if (m.x >= 748 && m.x <= 777 && m.y >= 224 && m.y <= 253)
					{
						if (currentmode == 'T')
						{
							if (trainList[trainnumber]->direction == clockwise)
							{
								trainList[trainnumber]->direction = anticlockwise;

							}
							else
							{
								trainList[trainnumber]->direction = clockwise;

							}
						}
					}

					else if (m.x >= 899 && m.x <= 928 && m.y >= 224 && m.y <= 253)
					{
						if (currentmode == 'T')
						{
							if (trainList[trainnumber]->direction == clockwise)
							{
								trainList[trainnumber]->direction = anticlockwise;

							}
							else
							{
								trainList[trainnumber]->direction = clockwise;

							}
						}
					}

					else if (m.x >= 709 && m.x <= 779 && m.y >= 71 && m.y <= 93)
					{
						if (currentmode == 'S' || currentmode == 'P')

							currentmode = 'T';


					}

					else if (m.x >= 711 && m.x <= 739 && m.y >= 349 && m.y <= 378)
					{
						if (currentmode == 'S')
						{
							if (trainnumber > 0)
							{
								trainnumber--;

							}
						}
					}


					else if (m.x >= 862 && m.x <= 890 && m.y >= 349 && m.y <= 378)
					{
						if (currentmode == 'S')
						{
							if (trainnumber < totaltrain - 1)
							{
								trainnumber++;

							}
						}
					}

					else if (m.x >= 712 && m.x <= 822 && m.y >= 381 && m.y <= 404)
					{
						if (currentmode == 'S')
						{
							if (cangivemission(stationnumber, trainnumber) == 1)
							{
								mainQueueNode mainData;
								stopTime = (rand() % 9) + 2;//(rand() % (10-2+1))+ a;
								mainData = (mainQueueNode)calloc(1, sizeof(struct _mainQueueNode));
								mainData->type = MSTATION;
								mainData->station.id = stationnumber;
								mainData->station.time = stopTime;
								mainData->station.train = trainnumber;
								append(mainMission, mainData);
							}
						}
					}


				}
			}
		}
	}
	RUN_TIME = RUN_TIME - minusmanualtime;
	_endthreadex(0);
	return 0;
}