//
//  traingui.h
//  LittleTrain
//
//  Created by 陈曦远 on 16/6/13.
//  Copyright ? 2016年 1601. All rights reserved.
//

#ifndef traingui_h
#define traingui_h


//图形化
char currentmode = T;  //指示右侧模块变更:T为火车模块，S为站点模块，P为公共轨道模块 
int trainnumber = 0; //序号为0,1,2的三辆车
int totaltrain;//火车总数 
int stationnumber;//序号从3开始的站点
int  totalstation;//站点总数
int branchnumber;// 序号从3+totalstation开始
int totalbranch;
int tochoose = 0;  //弹出人工选择窗口为1，未弹出为0

#endif /* traingui_h */