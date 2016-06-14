#include<easyx.h>
#include<graphics.h>
#include<tchar.h>
#include<train.h>
#include<TrainGUI.h>
MOUSEMSG m,n;		// ���������Ϣ

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
			if (trackNodeList[i]->type == BRANCH)
			{
				if (m.x >= (trackNodeList[i]->branch.x) && (m.x <= (trackNodeList[i]->branch.x + trackNodeList[i]->branch.width) && (m.y >= (trackNodeList[i]->branch.y) && (m.y <= (trackNodeList[i]->branch.y + trackNodeList[i]->branch.height))
					return trackNodeList[i]->id;
			}
			else if (trackNodeList[i]->type == STATION)
			{
				if (m.x >= (trackNodeList[i]->station.x) && (m.x <= (trackNodeList[i]->station.x + trackNodeList[i]->station.width) && (m.y >= (trackNodeList[i]->station.y) && (m.y <= (trackNodeList[i]->station.y + trackNodeList[i]->station.height))
					return trackNodeList[i]->id;
			}
		}
		return -1;
	}
	else
	return -1;
}
	// ��ȡһ�������Ϣ
	m = GetMouseMsg();

	if (m.uMsg == WM_LBUTTONDOWN)
	{
		if (programStat == 1)
		{
			if (n.x >= 347 && n.x <= 478 && n.y >= 302 && n.y <= 330)
			{
				programStat = 0;
			}
			else if (n.x >= 347 && n.x <= 521 && n.y >= 258 && n.x <= 280)
			{//����������������
			}
		}

		else if (programStat == 2)
		{//�µ��˹�����
			programStat = 0;
		}

		else if (toexit == 0 && tochoose == 0)
		{
			if (m.x >= 0 && m.x <= 694 && m.y >= 44 && m.y <= 560)             //����ڹ����
			{
				k = pointnodeid(m);
				if (k != -1)
				{
					if (trackNodeList[k]->type == STATION��
					{
					 currentmode = S;
					 stationnumber = k;
					 //�Ҳ���վ���ģ��ͼ(STATION_MODE.jpg);
					}
					else if (trackNodeList[k]->type == BRANCH��
					{
							currentmode = P;
						   branchnumber = k;
						   //�Ҳ������������ģ��ͼ(BRANCH_MODE.jpg);��
					}

				}
			}
			else              //������ڹ����
			{
case(m.x >= 916 && m.x <= 960 && m.y >= 0 && m.y <= 44) :
	mainData = calloc(1, sizeof(struct _mainQueueNode));    //����ر�
	mainData->type = MLOCK;
	append(mainMission, mainData);
	toexit = 1;                              //��ʾӦ�õ����˳�����,�����̣߳�
	break;

	case(m.x >= 711 && m.x <= 740 && m.y >= 473 && m.y <= 502)��
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
			controlPolicy == MANUAL��
				RECT r = { 751, 516, 848, 536 };
			drawtext(_T("�˹�"), &r);
		}
		else
		{
			controlPolicy == AUTO��
				RECT r = { 751, 516, 848, 536 };
			drawtext(_T("�Զ�"), &r);
		}
															   break;

	case(m.x >= 862 && m.x <= 891 && m.y >= 511 && m.y <= 540) :
		if (controlPolicy == AUTO)
		{
			controlPolicy == MANUAL��
				RECT r = { 751, 516, 848, 536 };
			drawtext(_T("�˹�"), &r);
		}
		else
		{
			controlPolicy == AUTO��
				RECT r = { 751, 516, 848, 536 };
			drawtext(_T("�Զ�"), &r);
		}
															   break;

	case(m.x >= 709 && m.x <= 819 && m.y >= 68 && m.y <= 94��:  //ȫ����ͣ����״̬
		if (currentmode == T)
		{//С��ȫ����ʼ                   
		}
		break;

	case(m.x >= 709 && m.x <= 738 && m.y >= 107 & m.y <= 136��:
		if (currentmode == T)
		{
			if (trainnumber != 0)
			{
				trainnumber--;
				TCHAR tid[20];
				_stprintf(tid, _T("%d")��trainnumber);
				RECT r = { 785, 107, 814, 136 };
				drawtext(tid, &r);
			}
		}
		break;

		case��m.x >= 860 && m.x <= 889 && m.y >= 107 && m.y <= 136��:
		if (currentmode == T)
		{
			if (trainnumber != totaltrain - 1)
			{
				trainnumber++;
				TCHAR tid[20];
				_stprintf(tid, _T("%d")��trainnumber);
				RECT r = { 785, 107, 814, 136 };
				drawtext(tid, &r);
			}
		}
		break;

	case(m.x >= 711 && m.x <= 801 && m.y >= 153 && m.y <= 178) :
		if (currentmode == T)
		{
			trainList[trainnumber]->status = PAUSE;
		}
															   break;

	case(m.x >= 748 && m.x <= 777 && m.y >= 191 && m.y <= 220) :
		if (currentmode == T)
		{
			if (trainList[trainnumber]->v > 0)
			{
				trainList[trainnumber]->v--;
				TCHAR tid[20];
				_stprintf(tid, _T("%dm/s")��trainList[trainnumber]->v);
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
				_stprintf(tid, _T("%dm/s")��trainList[trainnumber]->v);
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
			//��С��ģʽͼ(TRAIN_MODE.jpg)							
		}
															 break;

	case(m.x >= 711 && m.x <= 739 && m.y >= 349 && m.y <= 378) :
		if (currentmode == S)
		{
			if (servetrain > 0)
			{
				servertrain--;
				TCHAR tid[20];
				_stprintf(tid, _T("%d")��servertrain);
				RECT r = { 784, 356, 801, 374 };
				drawtext(tid, &r);
			}
		}
															   break;


	case(m.x >= 862 && m.x <= 890 && m.y >= 349 && m.y <= 378) :
		if (currentmode == S)
		{
			if (servetrain < totaltrain - 1)
			{
				servertrain++;
				TCHAR tid[20];
				_stprintf(tid, _T("%d")��servertrain);
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