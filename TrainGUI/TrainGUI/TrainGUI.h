#pragma once

#include "resource.h"
#include <easyx.h>
#include <windows.h>
#include <process.h>
extern "C" {
#include "train.h";
unsigned __stdcall main1(void* pArguments);
};

unsigned __stdcall GUIInput(void* pArguments);
unsigned __stdcall GUIOutput(void* pArguments);

//��������״̬��1Ϊ�������У�0Ϊֹͣ����
extern int programStat;
//����״̬��0��ʾ�޵�����1��ʾ�˳�������2��ʾѡ��𳵵�����3��ʾ����С���������׼���˳��ĵ���
extern "C" int frameStat;
extern int pauseStat;//�Ƿ�����ȫ����ͣ��ť,1Ϊ�ѵ��
extern char currentmode;  //ָʾ�Ҳ�ģ����:TΪ��ģ�飬SΪվ��ģ�飬PΪ�������ģ�� 
extern int trainnumber; //�Ҳ�ģ����ʾ��������������Ϣ
extern "C" int totaltrain;//������ 
extern int stationnumber;//�û������վ��id
extern int branchnumber;// �û�����Ĺ��������Ӧ��ĳһ�ֲ�ڵ�id
extern "C" HANDLE hMutex;

extern HANDLE hMutex;