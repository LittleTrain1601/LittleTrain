#include<easyx.h>
#include<graphics.h>
MOUSEMSG m,n;		// ���������Ϣ
IMAGE quitimg,confirmimg;



	// ��ȡһ�������Ϣ
	m = GetMouseMsg();

	if(m.uMsg== WM_LBUTTONDOWN)
	{case(m.x>=916&&m.x<=960&&m.y>=0&&m.y<=44):
		 //��ͣ����
		loadimage(&quitimg, "APP_QUIT.png");
	     putimage(330, 211, &quitimg);
		 loadimage(&confirmimg, "confirm.png");
		 putimage(347, 302, &confirmimg);
		 settextcolor(BLUE);
		 char s[] = "Ҫ��������������������"��
			 outtext(347, 258, s);
		 settextcolor(BLACK);
		 n = GetMouseMsg();
		 if (n.uMsg == WM_LBUTTONDOWN)
		 {
			 if (n.x >= 347 && n.x <= 478 && n.y >= 302 && n.y <= 330)
			 {
				 mainData = calloc(1, sizeof(struct _mainQueueNode));
				 mainData->type = MLOCK;
				 append(mainMission, mainData);
				 //�Ի�����ʧ���������
			 }
			 else if(n.x>=347&&n.x<=521&&n.y>=258&&n.x<=280)
			 {//��������
		     }
	     }
		 break;
		
		 case(m.x >= 711 && m.x <= 740 && m.y >= 473 && m.y <= 502)��
		   if (servicePolicy == BYTHEWAY)
				 ;
			 else
			 {
				 servicePolicy = BYTHEWAY;
				 RECT r1 = { 751, 478, 848, 498 };
				 drawtext(_T("˳�����"), &r1);
             }
		 break;
		
		 case(m.x >= 862 && m.x <= 891 && m.y >= 473 && m.y <= 502) :
			 if (servicePolicy == SEQUENCING)
				 ;
			 else
			 {
				 servicePolicy =SEQUENCING;
				 RECT r2 = { 751, 478, 848, 498 };
				 drawtext(_T("˳�����"), &r2);
			 }
		 break;

		 case(m.x >= 711 && m.x <= 740&& m.y >= 511 && m.y <= 540) :
			 if (controlPolicy==AUTO)
				 ;
			 else
			    { 
				 controlPolicy == AUTO��
					 RECT r3 = { 751, 516, 848, 536 };
				 drawtext(_T("�Զ�"), &r3);
				  }
		break;

		 case(m.x >= 862 && m.x <= 891 && m.y >= 511 && m.y <= 540) :
			 if (controlPolicy == MANUAL)
				 ;
			 else
			 {
				 controlPolicy == MANUAL��
					 RECT r4 = { 751, 516, 848, 536 };
				 drawtext(_T("�˹�"), &r4);
			 }
		break;

		case(m.x>=709&&m.x<=819&&m.y>=68&&m.y<=94��:
			//����ʼ
			break;

		case��m.x>=709&&m.x<=738&&m.y>=107&m.y<=136��:
		if (trainnumber != 0)
		{
			trainnumber--;
			char tid[20];
			sprintf(tid, "��%d"��trainnumber);
			RECT r5 = { 785, 107, 814, 136 };
			drawtext(tid, &r4);
		}

		case��m.x >= 806 && m.x <= 889 && m.y >= 107 & m.y <= 136��:
		if (trainnumber != totaltrain-1)
		{
			trainnumber++;
			char tid[20];
			sprintf(tid, "��%d"��trainnumber);
			RECT r5 = { 785, 107, 814, 136 };
			drawtext(tid, &r5);
		}

		case(m.x)