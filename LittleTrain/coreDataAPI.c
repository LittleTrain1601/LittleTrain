//
//  coreDataAPI.c
//  LittleTrain
//
//  Created by 陈曦远 on 16/5/20.
//  Copyright ? 2016年 1601. All rights reserved.
//


#include "train.h"
#include <stdlib.h>

//新建一个通用任务队列 
queue newQueue(){
	queueNode newqueueNode=NULL;
	newqueueNode=(queueNode)malloc(sizeof(struct _queueNode));
	newqueueNode->data=NULL;
	newqueueNode->next=NULL;
	queue newqueue=NULL;
	newqueue=(queue)malloc(sizeof(struct _queue));
	newqueue->head=newqueueNode;
	newqueue->tail=newqueueNode;
	return newqueue;
}

//将新任务追加到队列末尾,返回插入的节点的地址 
queueNode append(queue waitingqueue, void * data){
    queueNode newqueueNode=NULL;
    newqueueNode=(queueNode)malloc(sizeof(struct _queueNode));
    newqueueNode->next=NULL;
    newqueueNode->data=data;
    waitingqueue->tail->next=newqueueNode;
    waitingqueue->tail=newqueueNode;
    return newqueueNode;
}

//删除ptr所指向任务的下一个任务
void deleteAfter(queue waitingqueue, queueNode ptr){
    queueNode q=NULL;
    void * r;
    if(ptr->next==NULL)  //ptr指向队列中的最后一个任务
        return;
    q=ptr->next;
    ptr->next =q->next;
    if(ptr->next==NULL)    //若删除的为队列中的最后一个任务，则更改尾指针
        waitingqueue->tail=ptr;
    r=q->data;
    free(r);
    free(q);
    return;
}


//从队列中读出当前任务并将其删除，返回data指针
void * pop(queue waitingqueue){ 
if(waitingqueue->head==waitingqueue->tail) // 任务队列中只有头节点 
  return NULL;
else
   {queueNode p=waitingqueue->head->next;
       waitingqueue->head->next=p->next;
       void * q=p->data;
       free(p);
       if(waitingqueue->head->next==NULL)  //任务队列中除头节点外只有一个任务 
          waitingqueue->tail=waitingqueue->head;
     return q;
   }}

train newTrain(){
   int i;
   train newtrain=NULL;
   newtrain=(train)malloc(sizeof(struct _train));
   newtrain->id=-1;
   newtrain->v=0;
   newtrain->direction=clockwise;
   newtrain->nextNode=-1;
   newtrain->distance=0;
   newtrain->flag=permitted;
   newtrain->status=FREE;
   newtrain->passTimes=0;
   for(i=0;i<= MAXITEM-1;i++)
     {newtrain->nodeList[i]=-1;
	 }
	newtrain->mission=newQueue();
   return newtrain;
}

trackNode newTrackNode(){
   trackNode newtracknode=NULL;
   newtracknode=(trackNode)calloc(1,sizeof(struct _trackNode));//初始化内存空间 
    return newtracknode;
}

