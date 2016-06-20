//
//  main.c
//  LittleTrain
//
//  Created by 左昊东 on 16/5/11.
//  modifyied by 杨林青 on 16/5/29
//  Copyright ? 2016年 1601. All rights reserved.
//

#include <stdio.h>
#include<stdlib.h>
#include "train.h"
#include <time.h>
#include <windows.h>
#include <process.h>

extern int frameStat;
extern int programStat;
HANDLE hMutex;

trackNode trackNodeList[MAXITEM];//以节点ID为下标
train trainList[MAXITEM]; //以小火车ID为下标

clock_t minuswhiletime;
clock_t minussystemtime;
extern clock_t minusinputtime;
extern clock_t dt;
queue mainMission;

int trainid;

int secure() {
	//检查小火车是否都处于LOCk状态。若是，则返回1，程序将结束运行
	for (int i = 0; i<MAXITEM; i++) {
		if (trainList[i]) {
			if (trainList[i]->status != LOCK) {
				return 0;
			}
		}
	}
	return 1;
}
int prevIndex(int trainID) {
	int i;
	int nodeid = trainList[trainID]->nextNode;
	for (i = 0; nodeid != trainList[trainID]->nodeList[i]; i++) {}
	if (trainList[trainID]->direction == anticlockwise) {
		if (trainList[trainID]->nodeList[++i] == -1) {
			i = 0;
		}
	}
	else {
		if (i == 0) {
			for (; trainList[trainID]->nodeList[i] != -1; i++) {}
			i--;
		}
		else {
			i--;
		}
	}
	return i;
}
void changeDirection() {
	//与原来方向相反的情况：找上一个节点id记为“i”，根据上一节点类型更改小火车diatance
	int i = trainList[trainid]->nodeList[prevIndex(trainid)];
	if (trackNodeList[i]->type == STATION) {
		if (trackNodeList[i]->station.left->id == trainList[trainid]->nextNode) {
			trainList[trainid]->distance = trackNodeList[i]->station.ldistance - trainList[trainid]->distance;
		}
		else {
			trainList[trainid]->distance = trackNodeList[i]->station.rdistance - trainList[trainid]->distance;
		}

	}
	else if (trackNodeList[i]->type == BRANCH) {
		if (trackNodeList[i]->branch.left->id == trainList[trainid]->nextNode) {
			trainList[trainid]->distance = trackNodeList[i]->branch.ldistance - trainList[trainid]->distance;
		}
		else if (trackNodeList[i]->branch.right->id == trainList[trainid]->nextNode) {
			trainList[trainid]->distance = trackNodeList[i]->branch.rdistance - trainList[trainid]->distance;
		}
		else
			trainList[trainid]->distance = trackNodeList[i]->branch.ddistance - trainList[trainid]->distance;

	}
	else {
		if (trackNodeList[i]->traffic.left->id == trainList[trainid]->nextNode) {
			trainList[trainid]->distance = trackNodeList[i]->branch.ldistance - trainList[trainid]->distance;
		}
		else if (trackNodeList[i]->traffic.right->id == trainList[trainid]->nextNode) {
			trainList[trainid]->distance = trackNodeList[i]->traffic.rdistance - trainList[trainid]->distance;
		}
		else if (trackNodeList[i]->traffic.down->id == trainList[trainid]->nextNode) {
			trainList[trainid]->distance = trackNodeList[i]->traffic.ddistance - trainList[trainid]->distance;
		}
		else {
			trainList[trainid]->distance = trackNodeList[i]->traffic.udistance - trainList[trainid]->distance;
		}
	}
	trainList[trainid]->nextNode = i;
}

unsigned __stdcall main1(void* pArguments) {
	mainMission = newQueue();
	logWriter();
	//build();
	clock_t whiletime; clock_t whilecurrent = 0;
	RUN_TIME = clock();
	clock_t Rtime = RUN_TIME;
	trainQueueNode trainptr;//小火车新数据域指针
	mainQueueNode mainptr;
	//queueNode traincurrent;// 小火车任务队列当前指针
	while (programStat && !secure()) {
		WaitForSingleObject(hMutex, INFINITE);
		whiletime = clock();
		minuswhiletime = whiletime - whilecurrent;
		dt = minuswhiletime + minusinputtime;
		RUN_TIME += dt;
		minusinputtime = 0;
		whilecurrent = whiletime;//while 循环时间记录
		if (RUN_TIME - Rtime >= CLOCKS_PER_SEC) {
			//input(NULL);
		}
		while ((mainptr = (mainQueueNode)pop(mainMission)) != NULL) {
			switch (mainptr->type) {
			case MSTATION:
				trainid = mainptr->station.train;
				trainptr = (trainQueueNode)calloc(1, sizeof(struct _trainQueueNode));
				trainptr->station = mainptr->station.id;
				trainptr->time = mainptr->station.time;
				append(trainList[trainid]->mission, (void *)trainptr);
				fprintf(outputLog, "at %lums station%d requires train%d to stop for %d s.\n", RUN_TIME, mainptr->station.id, trainid, mainptr->station.time);
				break;
			case MTRAIN:
				trainid = mainptr->train.id;
				trainList[trainid]->v = mainptr->train.speed;
				if (mainptr->train.direction == MCLOCLWISE&&trainList[trainid]->direction == clockwise) {
					fprintf(outputLog, "at %lums train%d changes speed to %d.\n", RUN_TIME, trainid, mainptr->train.speed);
				}
				else if (mainptr->train.direction == MCLOCLWISE&&trainList[trainid]->direction == anticlockwise) {
					changeDirection();
					trainList[trainid]->direction = clockwise;
					fprintf(outputLog, "at %lums train%d changes speed to %d, changes direction to clockwise.\n", RUN_TIME, trainid, mainptr->train.speed);
				}
				else if (mainptr->train.direction == MANTICLOCKWISE&&trainList[trainid]->direction == anticlockwise) {
					fprintf(outputLog, "at %lums train%d changes speed to %d.\n", RUN_TIME, trainid, mainptr->train.speed);
				}
				else {
					changeDirection();
					trainList[trainid]->direction = anticlockwise;
					fprintf(outputLog, "at %lums train%d changes speed to %d, changes direction to anticlockwise.\n", RUN_TIME, trainid, mainptr->train.speed);
				}

				break;
			case MLOCK:
				for (int k = 0; k<MAXITEM; k++) {
					if (trainList[k]) {
						trainptr = (trainQueueNode)calloc(1, sizeof(struct _trainQueueNode));
						trainptr->type = TLOCK;
						append(trainList[k]->mission, (void *)trainptr);
					}
				}
				fprintf(outputLog, "at %lums receive lock command.\n", RUN_TIME);
				break;
			case MPAUSE:
				trainid = mainptr->train.id;                            //暂停
				trainList[trainid]->status = PAUSE;
				break;
			case MRESUME:
				trainid = mainptr->train.id;                            //开始
				if (trainList[trainid]->mission->head != trainList[trainid]->mission->tail)
				{
					trainQueueNode trainData = (trainQueueNode)(trainList[trainid]->mission->head->next->data);
					if(trainData->type==TLOCK)
					trainList[trainid]->status = LOCK;
				 else
					trainList[trainid]->status = RUN;
				}
				else if (trainList[trainid]->mission->head == trainList[trainid]->mission->tail)
					trainList[trainid]->status = FREE;
				
				break;
			default:
				break;
			}

		}

		for (int i = 0; i<MAXITEM; i++) {
			if (trainList[i] != NULL) {
				updateTrain(trainList[i]->id);
			}
		}
		ReleaseMutex(hMutex);
		/*
		if (RUN_TIME - Rtime >= CLOCKS_PER_SEC) {
			//viewer();
			Rtime = RUN_TIME;
		}
		*/
	}
	frameStat = 3;
	_endthreadex(0);
	return 0;
}
