#pragma once

#include "resource.h"
#include <easyx.h>
#include <windows.h>
#include <process.h>

unsigned __stdcall GUIInput(void* pArguments);
unsigned __stdcall GUIOutput(void* pArguments);

//��������״̬��1Ϊ�������У�0Ϊֹͣ����
extern int programStat;
//����״̬��0��ʾ�޵�����1��ʾ�˳�������2��ʾѡ��𳵵�����3��ʾ����С���������׼���˳��ĵ���
extern int frameStat;
extern char currentmode;  //ָʾ�Ҳ�ģ����:TΪ��ģ�飬SΪվ��ģ�飬PΪ�������ģ�� 
extern int trainnumber; //�Ҳ�ģ����ʾ��������������Ϣ
extern int totaltrain;//������ 
extern int stationnumber;//�û������վ��id
extern int branchnumber;// �û�����Ĺ��������Ӧ��ĳһ�ֲ�ڵ�id

extern HANDLE hMutex;