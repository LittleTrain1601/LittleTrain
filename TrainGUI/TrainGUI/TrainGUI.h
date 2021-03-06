#pragma once

#include "resource.h"
#include <easyx.h>
#include <windows.h>
#include <process.h>
extern "C" {
#include "train.h";
unsigned __stdcall main1(void* pArguments);
extern int choiceflag;
extern char tochoose[100];
extern char firstchoice[20];
extern char secondchoice[20];
};

unsigned __stdcall GUIInput(void* pArguments);
unsigned __stdcall GUIOutput(void* pArguments);

void CharToTchar(const char * _char, TCHAR * tchar);

//程序运行状态，1为正常运行，0为停止运行
extern "C" int programStat;
//窗口状态，0表示无弹窗，1表示退出弹窗，2表示选择火车弹窗，3表示所有小火车完成任务准备退出的弹窗
extern "C" int frameStat;
extern int pauseStat;//是否点击了全部暂停按钮,1为已点击
extern char currentmode;  //指示右侧模块变更:T为火车模块，S为站点模块，P为公共轨道模块 
extern int trainnumber; //右侧模块显示的是那辆车的信息
extern "C" int totaltrain;//火车总数 
extern int stationnumber;//用户点击的站点id
extern int branchnumber;// 用户点击的公共轨道对应的某一分叉节点id
extern "C" HANDLE hMutex;

extern HANDLE hMutex;