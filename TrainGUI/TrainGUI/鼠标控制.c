#include<easyx.h> 
#include<graphics.h>
#include<tchar.h>
#include<train.h>
#include<TrainGUI.h>
#include<stdlib.h>
MOUSEMSG m;		// ���������Ϣ
int i,k;
int servertrain=0;
int stopTime;


//�ж�ĳվ���Ƿ���ĳС�𳵹���ϵ�վ�㣬�Ƿ���1���񷵻�0
int cangivemission(int stationid, int trainid)
{
	
	for (i=0; i < 50; i++)
	{
		if (trainList[trainid]->nodeList[i] == stationid)
			return 1;
	}
	return 0;
}

//�ж��û��ڹ�����Ƿ�����վ����߹�����������������վ��id�򹫹������Ӧ�ķֲ�ڵ�id,��δ���վ��򹫹����������-1
int pointnodeid(MOUSEMSG m)
{
	if (m.uMsg == WM_LBUTTONDOWN)
	{
		for (i = 0; i < 50; i++)
		{
			if (m.x >= (trackNodeList[i]->x) && (m.x <= (trackNodeList[i]->x + trackNodeList[i]->width)) && (m.y >= (trackNodeList[i]->y)) && (m.y <= (trackNodeList[i]->y + trackNodeList[i]->height)))
			{
				if (trackNodeList[i]->type == BRANCH || trackNodeList[i]->type == STATION)
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
unsigned __stdcall GUIInput(void* pArguments)
{// ��ȡһ�������Ϣ
	m = GetMouseMsg();

	if (m.uMsg == WM_LBUTTONDOWN)
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
		{//�µ��˹����ԣ�
			frameStat = 0;
		}

		else if (frameStat == 0)
		{
			if (m.x >= 0 && m.x <= 694 && m.y >= 44 && m.y <= 560)             //����ڹ����
			{
				k = pointnodeid(m);
				if (k != -1)
				{
					if (trackNodeList[k]->type == STATION)
					{
						currentmode = S;
						stationnumber = k;
					}
					else if (trackNodeList[k]->type == BRANCH)
					{
						currentmode = P;
						branchnumber = k;
						
					}

				}
			}
			else              //������ڹ����
			{
case(m.x >= 916 && m.x <= 960 && m.y >= 0 && m.y <= 44) :
	mainData = calloc(1, sizeof(struct _mainQueueNode));    //����ر�
	mainData->type = MLOCK;
	append(mainMission, mainData);
	frameStat == 1;                              //��ʾӦ�õ����˳�����
	break;

case(m.x >= 711 && m.x <= 740 && m.y >= 473 && m.y <= 502) :
	if (servicePolicy == BYTHEWAY)
	{
		servicePolicy = SEQUENCING;
		RECT r = { 751, 478, 848, 498 };
		drawtext(_T("˳�����"), &r);
	}
	else
	{
		servicePolicy = BYTHEWAY;
		RECT r = { 751, 478, 848, 498 };
		drawtext(_T("˳�����"), &r);
	}
														   break;

case(m.x >= 862 && m.x <= 891 && m.y >= 473 && m.y <= 502) :
	if (servicePolicy == BYTHEWAY)
	{
		servicePolicy = SEQUENCING;
		RECT r = { 751, 478, 848, 498 };
		drawtext(_T("˳�����"), &r);
	}
	else
	{
		servicePolicy = BYTHEWAY;
		RECT r = { 751, 478, 848, 498 };
		drawtext(_T("˳�����"), &r);
	}
														   break;

case(m.x >= 711 && m.x <= 740 && m.y >= 511 && m.y <= 540) :
	if (controlPolicy == AUTO)
	{
		controlPolicy = MANUAL;
		RECT r = { 751, 516, 848, 536 };
		drawtext(_T("�˹�"), &r);
	}
	else
	{
		controlPolicy = AUTO;
		RECT r = { 751, 516, 848, 536 };
		drawtext(_T("�Զ�"), &r);
	}
														   break;

case(m.x >= 862 && m.x <= 891 && m.y >= 511 && m.y <= 540) :
	if (controlPolicy == AUTO)
	{
		controlPolicy = MANUAL;
		RECT r = { 751, 516, 848, 536 };
		drawtext(_T("�˹�"), &r);
	}
	else
	{
		controlPolicy = AUTO;
		RECT r = { 751, 516, 848, 536 };
		drawtext(_T("�Զ�"), &r);
	}
														   break;

case(m.x >= 709 && m.x <= 819 && m.y >= 68 && m.y <= 94) : //ȫ����ʼ
	if (currentmode == T)
	{
		for (i = 0; i < totaltrain; i++)
		{
			mainData = (mainQueueNode)calloc(1, sizeof(struct _mainQueueNode));
			mainData->type = MRESUME;
			mainData->train.id = i;
			append(mainMission, mainData);
		}
	}
														   break;


case(m.x >= && m.x <= && m.y >= && m.y < ) :  //ȫ����ͣ?��ť
	if (currentmode == T)
	{
		for (i = 0; i < totaltrain; i++)
		{
			mainData = (mainQueueNode)calloc(1, sizeof(struct _mainQueueNode));
			mainData->type = MPAUSE;
			mainData->train.id = i;
			append(mainMission, mainData);
		}
	}
											  break;

case(m.x >= 709 && m.x <= 738 && m.y >= 107 & m.y <= 136) :
	if (currentmode == T)
	{
		if (trainnumber > 0)
		{
			trainnumber--;
			TCHAR tid[20];
			_stprintf(tid, _T("%d"), trainnumber);
			RECT r = { 785, 107, 814, 136 };
			drawtext(tid, &r);
		}
	}
														  break;

case(m.x >= 860 && m.x <= 889 && m.y >= 107 && m.y <= 136) :
	if (currentmode == T)
	{
		if (trainnumber < totaltrain - 1)
		{
			trainnumber++;
			TCHAR tid[20];
			_stprintf(tid, _T("%d", trainnumber);
			RECT r = { 785, 107, 814, 136 };
			drawtext(tid, &r);
		}
	}
														   break;

case(m.x >= 711 && m.x <= 801 && m.y >= 153 && m.y <= 178) : //��ͣ������
	if (currentmode == T)
	{
		mainData = (mainQueueNode)calloc(1, sizeof(struct _mainQueueNode));
		mainData->type = MPAUSE;
		mainData->train.id = trainnumber;
		append(mainMission, mainData);
	}
															 break;

case(m.x >= && m.x <= && m.y >= && m.y <= ) : //��ʼ����������ť
	if (currentmode == T)
	{
		mainData = (mainQueueNode)calloc(1, sizeof(struct _mainQueueNode));
		mainData->type = MRESUME;
		mainData->train.id = trainnumber;
		append(mainMission, mainData);
	}
											  break;

case(m.x >= 748 && m.x <= 777 && m.y >= 191 && m.y <= 220) :
	if (currentmode == T)
	{
		if (trainList[trainnumber]->v > 0)
		{
			trainList[trainnumber]->v--;
			TCHAR tid[20];
			_stprintf(tid, _T("%dm/s"), trainList[trainnumber]->v);
			RECT r = { 787, 195, 891, 216 };
			drawtext(tid, &r);
		}
	}
														   break;

case(m.x >= 899 && m.x <= 928 && m.y >= 191 && m.y <= 220) :
	if (currentmode == T)
	{
		if (trainList[trainnumber]->v < 10)
		{
			trainList[trainnumber]->v++;
			TCHAR tid[20];
			_stprintf(tid, _T("%dm/s"), trainList[trainnumber]->v);
			RECT r = { 787, 195, 891, 216 };
			drawtext(tid, &r);
		}
	}
														   break;

case(m.x >= 748 && m.x <= 777 && m.y >= 224 && m.y <= 253) :
	if (currentmode == T)
	{
		if (trainList[trainnumber]->direction == clockwise)
		{
			trainList[trainnumber]->direction = anticlockwise;
			RECT r = { 785, 228, 889, 249 };
			drawtext(_T("ANTICLOCKWISE"), &r);
		}
		else
		{
			trainList[trainnumber]->direction = clockwise;
			RECT r = { 785, 228, 889, 249 };
			drawtext(_T("CLOCKWISE"), &r);
		}
	}
														   break;

case(m.x >= 899 && m.x <= 928 && m.y >= 224 && m.y <= 253) :
	if (currentmode == T)
	{
		if (trainList[trainnumber]->direction == clockwise)
		{
			trainList[trainnumber]->direction = anticlockwise;
			RECT r = { 785, 228, 889, 249 };
			drawtext(_T("ANTICLOCKWISE"), &r);
		}
		else
		{
			trainList[trainnumber]->direction = clockwise;
			RECT r = { 785, 228, 889, 249 };
			drawtext(_T("CLOCKWISE"), &r);
		}
	}
														   break;

case(m.x >= 709 && m.x <= 779 && m.y >= 71 && m.y <= 93) :
	if (currentmode == S || currentmode == P)
	{
		currentmode = T;
        							
	}
														 break;

case(m.x >= 711 && m.x <= 739 && m.y >= 349 && m.y <= 378) :
	if (currentmode == S)
	{
		if (servertrain > 0)
		{
			servertrain--;
			TCHAR tid[20];
			_stprintf(tid, _T("%d"), servertrain);
			RECT r = { 784, 356, 801, 374 };
			drawtext(tid, &r);
		}
	}
														   break;


case(m.x >= 862 && m.x <= 890 && m.y >= 349 && m.y <= 378) :
	if (currentmode == S)
	{
		if (servertrain < totaltrain - 1)
		{
			servertrain++;
			TCHAR tid[20];
			_stprintf(tid, _T("%d"), servertrain);
			RECT r = { 784, 356, 801, 374 };
			drawtext(tid, &r);
		}
	}
														   break;

case(m.x >= 712 && m.x <= 822 && m.y >= 381 && m.y <= 404) :
	if (currentmode == S)
	{
		if (cangivemission(stationnumber, servertrain) == 1)
		{
			stopTime = (rand() % 9) + 2;//(rand() % (10-2+1))+ a;
			mainData = calloc(1, sizeof(struct _mainQueueNode));
			mainData->type = MSTATION;
			mainData->station.id = stationnumber;
			mainData->station.time = stopTime;
			mainData->station.train = servertrain;
			append(mainMission, mainData);
		}
	}
														   break;

			}
		}
	}
}