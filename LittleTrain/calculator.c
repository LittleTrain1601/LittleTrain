//
//  calculator.c
//  LittleTrain
//
//  Created by 左昊东 on 16/5/20.
//  Copyright © 2016年 1601. All rights reserved.
//

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include "train.h"
extern clock_t inputtime;
extern clock_t minussystemtime;
extern clock_t minuswhiletime;
clock_t minusinputtime;
 clock_t RUN_TIME=0;
clock_t dt;


void updateTrain(int id){//循环里计算一辆小火车的位置并更新。 	Int id 小火车的ID，即trainList的下标。
     enum{station, branch, traffic}type;
	int x;
    RUN_TIME+=minussystemtime-minuswhiletime-minusinputtime;
    dt=minussystemtime-minuswhiletime-minusinputtime;
    x=dt*trainList[id]->v;
    int nodeid=trainList[id]->nextNode;
    queueNode traincurrentptr;
	traincurrentptr=(struct _queueNode*)malloc(sizeof(queueNode));//小火车链表当前指针1
	trainQueueNode traindata;
      traindata=(struct _trainQueueNode*)malloc(sizeof(trainQueueNode));
    if(trainList[id]->status!=RUN){
    	int distance=trainList[id]->distance=trainList[id]->distance-x;
    
    	if(distance<=0){
    		if(trackNodeList[nodeid]->type==STATION){
    				int a=0;
    				if(servicePolicy==BYTHEWAY){
                  			traincurrentptr=trainList[id]->mission->head->next;
                         	while(traincurrentptr!=NULL){
                                     traindata=(trainQueueNode) traincurrentptr->data;
                                     if(nodeid==traindata->station){
                                        trackNodeList[nodeid]->station.stop=RUN_TIME+distance/trainList[id]->v;
                                        distance=0;a=1;
                                        traincurrentptr=traincurrentptr->next;
										break;}
                                    }
                                
                        	if(a==0)
									if(trainList[id]->nodeList[nodeid+1]==-1){
            							trainList[id]->distance=trackNodeList[0]->station.ldistance-(x-trainList[id]->distance);
           								trainList[id]->nextNode=0;
       									}
      				 	else{
           							trainList[id]->distance=trackNodeList[nodeid+1]->station.ldistance-(x-trainList[id]->distance);
           							trainList[id]->nextNode=nodeid+1;
       									}
					
							}
                    else{
                    	traincurrentptr=trainList[id]->mission->head->next;
                    	traindata=(trainQueueNode) traincurrentptr->data;
                                     if(nodeid==traindata->station){
                                        trackNodeList[nodeid]->station.stop=RUN_TIME+distance/trainList[id]->v;
                                          distance=0;
										  a=1;}
									if(a==0)//jijihohogofjf
											if(trainList[id]->nodeList[nodeid+1]==-1){
            									trainList[id]->distance=trackNodeList[0]->station.ldistance-(x-trainList[id]->distance);
           										trainList[id]->nextNode=0;
       									}
       										else{
           										trainList[id]->distance=trackNodeList[nodeid+1]->station.ldistance-(x-trainList[id]->distance);
           										trainList[id]->nextNode=nodeid+1;
       									}
                        						  
					}                 			
                                       
                    }
    		if(trackNodeList[nodeid]->type==BRANCH){
    				if(trainList[id]->nodeList[nodeid+1]==-1){
            				trainList[id]->distance=trackNodeList[0]->branch.ldistance-(x-trainList[id]->distance);
           					trainList[id]->nextNode=0;
       									}
       				else{
           					trainList[id]->distance=trackNodeList[nodeid+1]->branch.ldistance-(x-trainList[id]->distance);
           					trainList[id]->nextNode=nodeid+1;
       									}
			}
    		else{
    				if(trainList[id]->nodeList[nodeid+1]==-1){
            				trainList[id]->distance=trackNodeList[0]->traffic.ldistance-(x-trainList[id]->distance);
           					trainList[id]->nextNode=0;
       									}
       				else{
           					trainList[id]->distance=trackNodeList[nodeid+1]->traffic.ldistance-(x-trainList[id]->distance);
           					trainList[id]->nextNode=nodeid+1;
       									}
			}	
					
			}
    				
		}
	
	else{
		trainStatusSwitcher(id);
	}
}


   int checkTrack(int branch1, int branch2){
    int a=0;
    trackNode trackptr;
    trackptr=(struct _trackNode*)malloc(sizeof(trackNode));
    trackptr=trackNodeList[branch1];
    while(trackptr->branch.down==trackNodeList[branch2]){
        switch(trackptr->type){
				 case 'STATION':{
                    trackptr=trackptr->station.right;break;
                }
                 case'TRAFFIC':{
                     trackptr=trackptr->traffic.down;break;
                 }   
                 case 'BRANCH':{
                     if(trackptr->branch.flag==1){
					 		 a=1;
                            break;break;}
                     else{
                         trackptr=trackptr->branch.down;
                         break;
                     }
                 }
            }
			}
       return a;
    
     }
    //检查两个节点之间的轨道是否占用 	两个节点的ID 	占用返回1，无占用返回0 
int judge(int train1, int train2){
    clock_t inputcurrent;
    if(controlPolicy==MANUAL){
        inputtime=clock();
        int a;
        printf("please choose the train");
        printf("%d %d",&train1,&train2);
        scanf("%d",&a);
        trainList[a]->flag=permitted;
        if(a==train1)
                  trainList[train2]->flag=forbidden;
        else
                    trainList[train1]->flag=forbidden;
        inputcurrent=clock();
        minusinputtime+=inputtime-inputcurrent;
        return a;
    }
    else{
        if(trainList[train1]-> passTimes>trainList[train2]->passTimes){
                trainList[train2]=permitted; trainList[train1]=forbidden;
          		return train2;
        }
        else{
            trainList[train1]=permitted;  trainList[train2]=forbidden;
            return train1;
        }
    }
} //判定竞争的小火车通过顺序。根据人工干预和交替模式选择不同判定方法.要判定的两个火车的ID允许通过的火车ID 
