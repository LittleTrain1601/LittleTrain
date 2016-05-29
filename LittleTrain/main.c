//
//  main.c
//  LittleTrain
//
//  Created by 左昊东 on 16/5/11.
//  modifyied by 杨林青 on 16/5/29
//  Copyright © 2016年 1601. All rights reserved.
//

#include <stdio.h>
#include<stdlib.h>
#include "train.h"
#include <time.h>
clock_t inputtime;
clock_t minuswhiletime;
clock_t minussystemtime;
queue mainMission;
main(){
	logwrite();
   clock_t systemtime;clock_t systemcurrent;
   clock_t whiletime;clock_t whilecurrent=0;
   systemtime=clock();
    clock_t Rtime;  
      trainQueueNode trainptr;
      trainptr=(struct _trainQueueNode*)malloc(sizeof(trainQueueNode));//小火车新数据域指针
      trainQueueNode traindata;
      traindata=(struct _trainQueueNode*)malloc(sizeof(trainQueueNode));//小火车数据域变量指针
        mainQueueNode mainptr;
        mainptr=(struct _mainQueueNode*)malloc(sizeof(mainQueueNode));//主任务队列数据域指针
        mainptr=(mainQueueNode)pop(mainMission);
       	FILE * outputlog=fopen("outputlog.c","w+");
        while(mainptr->type!=LOCK){
        	Rtime=RUN_TIME;
            whiletime=clock();
            minuswhiletime+=whiletime-whilecurrent;
            whilecurrent=whiletime;//while 循环时间记录
             int trainid=mainptr->station.train;
            switch(mainptr->type){
		             case 'MSTATION':{
		        	updateTrain(mainptr->station.train);
                    trainptr->station=mainptr->station.id;
                    trainptr->time=mainptr->station.time;
                   
                    append(trainList[trainid]->mission,(void *)trainptr);
                     mainptr=(mainQueueNode)pop(mainMission);
                     fprintf(outputlog,"at%dms train%d stop at %d for %d",RUN_TIME,trainid,trainptr->station,trainptr->time); 
                         break; }
               case 'MTRAIN':{
                   trainList[mainptr->train.id]->v=mainptr->train.speed;
                   trainList[mainptr->train.id]->direction=mainptr->train.direction;
                   mainptr=(mainQueueNode)pop(mainMission);
                   fprintf(outputlog,"at%dms train%d change speed to%d change direction to%d",RUN_TIME,trainid,mainptr->train.speed,mainptr->train.direction);
               		break;
			   }
               case 'MSWITCHMETHOD':{
                   if(mainptr->switchmethod.method==MANUAL)
                     controlPolicy=MANUAL;
                   else
                      controlPolicy=AUTO; 
                	fprintf(outputlog,"at%dms  change switchmethod to %u",RUN_TIME,controlPolicy);
                   mainptr=(mainQueueNode)pop(mainMission);
				   break;
               }
            }
            if(RUN_TIME-Rtime>=1000)
            		viewer();
    }
    systemcurrent=clock();
    minussystemtime=systemtime-systemcurrent;
}
  
