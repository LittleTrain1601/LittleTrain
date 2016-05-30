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

trackNode trackNodeList[MAXITEM];//以节点ID为下标
train trainList[MAXITEM]; //以小火车ID为下标

clock_t inputtime;
clock_t minuswhiletime;
clock_t minussystemtime;
extern clock_t minusinputtime;
extern clock_t dt;
queue mainMission;
int secure() {
    //检查小火车是否都处于LOCk状态。若是，则返回1，程序将结束运行
    for (int i=0; i<MAXITEM; i++) {
        if (trainList[i]) {
            if (trainList[i]->status != LOCK) {
                return 0;
            }
        }
    }
    return 1;
}

int main(){
    mainMission = newQueue();
    logWriter();
    build();
    clock_t whiletime;clock_t whilecurrent=0;
    RUN_TIME = clock();
    clock_t Rtime = RUN_TIME;
    trainQueueNode trainptr;//小火车新数据域指针
    mainQueueNode mainptr;
    FILE * outputlog=fopen("outputlog.c","w+");
    int trainid;
    while(!secure()){
        whiletime=clock();
        minuswhiletime=whiletime-whilecurrent;
        dt = minuswhiletime - minusinputtime;
        RUN_TIME += dt;
        minusinputtime = 0;
        whilecurrent=whiletime;//while 循环时间记录
        
        input(NULL);
        while ((mainptr = (mainQueueNode)pop(mainMission)) != NULL) {
            switch (mainptr->type) {
                case MSTATION:
                    trainid = mainptr->station.train;
                    trainptr = calloc(1, sizeof(struct _trainQueueNode));
                    trainptr->station=mainptr->station.id;
                    trainptr->time=mainptr->station.time;
                    append(trainList[trainid]->mission,(void *)trainptr);
                    break;
                case MTRAIN:
                    trainid = mainptr->train.id;
                    trainList[trainid]->v=mainptr->train.speed;
                    if (mainptr->train.direction == MCLOCLWISE) {
                        trainList[trainid]->direction = clockwise;
                    } else {
                        trainList[trainid]->direction = anticlockwise;
                    }
                    fprintf(outputlog,"at%lums train%d change speed to%d change direction to%d",RUN_TIME,trainid,mainptr->train.speed,mainptr->train.direction);
                    break;
                default:
                    break;
            }
            
        }

        for (int i=0; i<MAXITEM; i++) {
            if (trainList[i] != NULL) {
                updateTrain(trainList[i]->id);
            }
        }
        if(RUN_TIME-Rtime>=CLOCKS_PER_SEC){
            viewer();
            Rtime = RUN_TIME;
        }
    }
    fclose(outputlog);
}

