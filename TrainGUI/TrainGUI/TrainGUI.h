#pragma once

#include "resource.h"
#include <easyx.h>
#include <windows.h>
#include <process.h>

unsigned __stdcall GUIInput(void* pArguments);
unsigned __stdcall GUIOutput(void* pArguments);

//��������״̬��1Ϊ�������У�0Ϊֹͣ����
extern int programStat;
//����״̬��0��ʾ�޵�����1��ʾ�˳�������2��ʾѡ��𳵵���
extern int frameStat;