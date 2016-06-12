//
//  calculator.c
//  LittleTrain
//
//  Created by ��껶� on 16/5/20.
//  modifyied by ������ on 16/5/29
//  Copyright ? 2016�� 1601. All rights reserved.
//

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include "train.h"

extern clock_t minuswhiletime;
clock_t minusinputtime;
clock_t RUN_TIME = 0;
clock_t dt;

int nextIndex(int trainID, int currentID) {
	int i;
	int nodeid = currentID;
	for (i = 0; nodeid != trainList[trainID]->nodeList[i]; i++) {
		if (trainList[trainID]->nodeList[i] == -1) {
			i = 0;
		}
	}
	if (trainList[trainID]->direction == clockwise) {
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
void updateTrain(int id) {//ѭ�������һ��С�𳵵�λ�ò����¡� 	Int id С�𳵵�ID����trainList���±ꡣ
						  //enum{station, branch, traffic}type;
	double x;
	x = ((double)dt)*(trainList[id]->v) / CLOCKS_PER_SEC;
	int nodeid = trainList[id]->nextNode;
	queueNode traincurrentptr;//С������ǰָ��1
	trainQueueNode traindata;
	if (trainList[id]->status == RUN) {
		//if(trainList[id]->direction==)   
		//else{
		trainList[id]->distance = trainList[id]->distance - x;
		if (trainList[id]->distance <= 0) {
			if (trackNodeList[nodeid]->type == STATION) {
				int stopped = 0;
				if (trainList[id]->mission->head->next) {
					if (servicePolicy == BYTHEWAY) {
						traincurrentptr = trainList[id]->mission->head->next;
						while (traincurrentptr != NULL) {
							traindata = (trainQueueNode)traincurrentptr->data;
							if (nodeid == traindata->station) {
								trackNodeList[nodeid]->station.stop = RUN_TIME + (trainList[id]->distance) / (trainList[id]->v)*CLOCKS_PER_SEC;
								trainList[id]->distance = 0;
								stopped = 1;
								break;
							}
							traincurrentptr = traincurrentptr->next;
						}


					}
					else {
						traincurrentptr = trainList[id]->mission->head->next;
						traindata = (trainQueueNode)traincurrentptr->data;
						if (nodeid == traindata->station) {
							trackNodeList[nodeid]->station.stop = RUN_TIME + (trainList[id]->distance) / trainList[id]->v*CLOCKS_PER_SEC;
							trainList[id]->distance = 0;
							stopped = 1;
						}
					}
				}
				if (stopped == 0) {
					trainList[id]->nextNode = trainList[id]->nodeList[nextIndex(id, trainList[id]->nextNode)];
					if (trackNodeList[nodeid]->station.left->id == trainList[id]->nextNode) {
						trainList[id]->distance += trackNodeList[nodeid]->station.ldistance;
					}
					else {
						trainList[id]->distance += trackNodeList[nodeid]->station.rdistance;
					}
				}
			}
			else if (trackNodeList[nodeid]->type == BRANCH) {
				trainList[id]->nextNode = trainList[id]->nodeList[nextIndex(id, trainList[id]->nextNode)];
				if (trackNodeList[nodeid]->branch.left->id == trainList[id]->nextNode) {
					trainList[id]->distance += trackNodeList[nodeid]->branch.ldistance;
				}
				else if (trackNodeList[nodeid]->branch.right->id == trainList[id]->nextNode) {
					trainList[id]->distance += trackNodeList[nodeid]->branch.rdistance;
				}
				else {
					trainList[id]->distance += trackNodeList[nodeid]->branch.ddistance;
				}
			}
			else {
				trainList[id]->nextNode = trainList[id]->nodeList[nextIndex(id, trainList[id]->nextNode)];
				if (trackNodeList[nodeid]->traffic.left->id == trainList[id]->nextNode) {
					trainList[id]->distance += trackNodeList[nodeid]->traffic.ldistance;
				}
				else if (trackNodeList[nodeid]->traffic.right->id == trainList[id]->nextNode) {
					trainList[id]->distance += trackNodeList[nodeid]->traffic.rdistance;
				}
				else if (trackNodeList[nodeid]->traffic.down->id == trainList[id]->nextNode) {
					trainList[id]->distance += trackNodeList[nodeid]->traffic.ddistance;
				}
				else {
					trainList[id]->distance += trackNodeList[nodeid]->traffic.udistance;
				}
			}

		}
		// }			
	}
	trainStatusSwitcher(id);
}


int checkTrack(int trainID, int branch1, int branch2) {
	//��������ڵ�֮��Ĺ���Ƿ�ռ�� 	�����ڵ��ID 	ռ�÷���1����ռ�÷���0
	int occupied = 0;
	trackNode trackptr;
	trackptr = trackNodeList[branch1];
	while (trackptr->id != branch2) {
		switch (trackptr->type) {
		case STATION:
			trackptr = trackNodeList[trainList[trainID]->nodeList[nextIndex(trainID, trackptr->id)]];
			break;
		case TRAFFIC:
			trackptr = trackNodeList[trainList[trainID]->nodeList[nextIndex(trainID, trackptr->id)]];
			break;
		case BRANCH:
			if (trackptr->branch.flag == 1) {
				occupied = 1;
			}
			else {
				trackptr = trackNodeList[trainList[trainID]->nodeList[nextIndex(trainID, trackptr->id)]];
			}
			break;
		default:
			break;
		}
		if (occupied) {
			break;
		}
	}
	return occupied;

}

int judge(int train1, int train2) {
	//�ж�������С��ͨ��˳�򡣸����˹���Ԥ�ͽ���ģʽѡ��ͬ�ж�����.Ҫ�ж��������𳵵�ID����ͨ���Ļ�ID
	clock_t inputtime;
	clock_t inputcurrent;
	if (controlPolicy == MANUAL) {
		inputtime = clock();
		int a;
		printf("please choose the train\n");
		printf("%d %d\n", train1, train2);
		scanf("%d", &a);
		getchar();
		trainList[a]->flag = permitted;
		if (a == train1)
			trainList[train2]->flag = forbidden;
		else
			trainList[train1]->flag = forbidden;
		inputcurrent = clock();
		minusinputtime += inputtime - inputcurrent;
		return a;

	}
	else {
		if (trainList[train1]->passTimes>trainList[train2]->passTimes) {
			trainList[train2]->flag = permitted; trainList[train1]->flag = forbidden;
			return train2;
		}
		else if (trainList[train1]->passTimes<trainList[train2]->passTimes) {
			trainList[train1]->flag = permitted;  trainList[train2]->flag = forbidden;
			return train1;
		}
		else
		{
			if (train1>train2)
			{
				trainList[train1]->flag = permitted;  trainList[train2]->flag = forbidden;
				return train1;
			}
			else
			{
				trainList[train2]->flag = permitted;  trainList[train1]->flag = forbidden;
				return train2;
			}

		}
	}
}
