#pragma once

#include "resource.h"
#include <easyx.h>
#include <windows.h>
#include <process.h>

unsigned __stdcall GUIInput(void* pArguments);
unsigned __stdcall GUIOutput(void* pArguments);

//程序运行状态，1为正常运行，0为停止运行
extern int programStat;
//窗口状态，0表示无弹窗，1表示退出弹窗，2表示选择火车弹窗
extern int frameStat;