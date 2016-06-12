//
//  coreDataAPI.c
//  LittleTrain
//
//  Created by ����Զ on 16/5/20.
//  Copyright ? 2016�� 1601. All rights reserved.
//


#include "train.h"
#include <stdlib.h>

//�½�һ��ͨ��������� 
queue newQueue() {
	queueNode newqueueNode = NULL;
	newqueueNode = (queueNode)malloc(sizeof(struct _queueNode));
	newqueueNode->data = NULL;
	newqueueNode->next = NULL;
	queue newqueue = NULL;
	newqueue = (queue)malloc(sizeof(struct _queue));
	newqueue->head = newqueueNode;
	newqueue->tail = newqueueNode;
	return newqueue;
}

//��������׷�ӵ�����ĩβ,���ز���Ľڵ�ĵ�ַ 
queueNode append(queue waitingqueue, void * data) {
	queueNode newqueueNode = NULL;
	newqueueNode = (queueNode)malloc(sizeof(struct _queueNode));
	newqueueNode->next = NULL;
	newqueueNode->data = data;
	waitingqueue->tail->next = newqueueNode;
	waitingqueue->tail = newqueueNode;
	return newqueueNode;
}

//ɾ��ptr��ָ���������һ������
void deleteAfter(queue waitingqueue, queueNode ptr) {
	queueNode q = NULL;
	void * r;
	if (ptr->next == NULL)  //ptrָ������е����һ������
		return;
	q = ptr->next;
	ptr->next = q->next;
	if (ptr->next == NULL)    //��ɾ����Ϊ�����е����һ�����������βָ��
		waitingqueue->tail = ptr;
	r = q->data;
	free(r);
	free(q);
	return;
}


//�Ӷ����ж�����ǰ���񲢽���ɾ��������dataָ��
void * pop(queue waitingqueue) {
	if (waitingqueue->head == waitingqueue->tail) // ���������ֻ��ͷ�ڵ� 
		return NULL;
	else
	{
		queueNode p = waitingqueue->head->next;
		waitingqueue->head->next = p->next;
		void * q = p->data;
		free(p);
		if (waitingqueue->head->next == NULL)  //��������г�ͷ�ڵ���ֻ��һ������ 
			waitingqueue->tail = waitingqueue->head;
		return q;
	}
}

train newTrain() {
	int i;
	train newtrain = NULL;
	newtrain = (train)malloc(sizeof(struct _train));
	newtrain->id = -1;
	newtrain->v = 0;
	newtrain->direction = clockwise;
	newtrain->nextNode = -1;
	newtrain->distance = 0;
	newtrain->flag = permitted;
	newtrain->status = FREE;
	newtrain->passTimes = 0;
	for (i = 0; i <= MAXITEM - 1; i++)
	{
		newtrain->nodeList[i] = -1;
	}
	newtrain->mission = newQueue();
	return newtrain;
}

trackNode newTrackNode() {
	trackNode newtracknode = NULL;
	newtracknode = (trackNode)calloc(1, sizeof(struct _trackNode));//��ʼ���ڴ�ռ� 
	return newtracknode;
}

