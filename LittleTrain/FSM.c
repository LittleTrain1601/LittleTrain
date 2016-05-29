//
//  FSM.c
//  LittleTrain
//
//  Created by 陈曦远 on 16/5/16.
//  Copyright © 2016年 1601. All rights reserved.
//


#include "train.h"
#include <stdio.h>

enum _servicePolicy servicePolicy;
enum policy controlPolicy;
 
int firstgo;


    	
//判断id号火车是否到达分叉节点或十字路节点的响应区间 ，是返回1，否返回0 
int arriveresponddomain(int id)
{train currenttrain = NULL;
trackNode nexttrackNode = NULL;
int lastNode;//小火车走过的上一个节点 
 int i,k,m,n;
 int range; //下一分叉节点或十字路节点的响应区间长度 
 for(i=0;i<=(MAXITEM-1);i++) 
     {if(trainList[i]->id==id)
        currenttrain=trainList[i];}
 for(k=0;k<=(MAXITEM-1);k++) 
        {if(trackNodeList[k]->id==currenttrain->nextNode)
        nexttrackNode=trackNodeList[k];}
 for(m=0;m<=(MAXITEM-1)&&(currenttrain->nodeList[m]!=currenttrain->nextNode);m++) 
        {;
        }
if(nexttrackNode->type!=BRANCH&&nexttrackNode->type!=TRAFFIC)
      return 0;
if(nexttrackNode->type==BRANCH)
    {{if(currenttrain->direction==clockwise)
        {if(m!=0)                                 //同一小火车轨道上的节点按顺时针顺序储存 
		   lastNode=currenttrain->nodeList[m-1]; 
		 else    //小火车即将到达的节点在小火车轨道数组中的下标为0 
		   {for(n=0;n<=(MAXITEM-1)&&(currenttrain->nodeList[n]!=-1);n++)
		       {;}
		       lastNode=currenttrain->nodeList[n-1];}}
       else
          {if(currenttrain->nodeList[m+1]!=-1)
		   lastNode=currenttrain->nodeList[m+1];
		   else //小火车即将到达的节点是小火车轨道数组中的最后一个元素 
		   lastNode=currenttrain->nodeList[0];}}
        {if(lastNode==nexttrackNode->branch.left->id)
          range=nexttrackNode->branch.lrange;
        else if(lastNode==nexttrackNode->branch.right->id)
          range=nexttrackNode->branch.rrange;
        else
           return 0;}
    if(1<currenttrain->distance<=range)   //小火车进入响应区间 
       return 1;
     else
       return 0;}
if(nexttrackNode->type==TRAFFIC)
    {{if(currenttrain->direction==clockwise)
        {if(m!=0)
		   lastNode=currenttrain->nodeList[m-1];
		 else
		   {for(n=0;n<=(MAXITEM-1)&&(currenttrain->nodeList[n]!=-1);n++)
		       {;}
		       lastNode=currenttrain->nodeList[n-1];}}
       else
          {if(currenttrain->nodeList[m+1]!=-1)
		   lastNode=currenttrain->nodeList[m+1];
		   else
		   lastNode=currenttrain->nodeList[0];}}
        if(lastNode==nexttrackNode->traffic.down->id)
          range=nexttrackNode->traffic.drange;
        else if(lastNode==nexttrackNode->traffic.left->id)
          range=nexttrackNode->traffic.lrange;
        else if(lastNode==nexttrackNode->traffic.right->id)
          range=nexttrackNode->traffic.rrange;
        else if(lastNode==nexttrackNode->traffic.up->id)
          range=nexttrackNode->traffic.urange;
     if(currenttrain->distance<=range)
       return 1;
     else
       return 0;}
else
    return 0;}  
  
//若分叉节点是小火车当前运行方向下进入公共轨道的节点返回1，若分叉节点是小火车当前运行方向下出公共轨道的节点返回0 
int branchtype(int trainid,int tracknodeid){
    int i,k,m,n;
    train currenttrain = NULL;
    trackNode  tracknode = NULL;
    int lastNode,nextNode;
    for(i=0;i<=(MAXITEM-1);i++)
    {if(trainList[i]->id==trainid)
        currenttrain=trainList[i];}
    for(k=0;k<=(MAXITEM-1);k++)
    {if(trackNodeList[k]->id==tracknodeid)
        tracknode=trackNodeList[k];}
    for(m=0;m<=(MAXITEM-1)&&(currenttrain->nodeList[m]!=tracknodeid);m++)
    {;}
    //顺时针寻找一辆小火车轨道上编号为tracknodeid的节点的前一个和后一个节点
    if(m!=0)
        lastNode=currenttrain->nodeList[m-1];
    else
    {
        for(n=0;n<=(MAXITEM-1)&&(currenttrain->nodeList[n]!=-1);n++)
        {;}
        lastNode=currenttrain->nodeList[n-1];
    }
    if(currenttrain->nodeList[m+1]!=-1)
        nextNode=currenttrain->nodeList[m+1];
    else
        nextNode=currenttrain->nodeList[0];
    if(currenttrain->direction==clockwise)
	   {
           if(tracknode->branch.down->id==nextNode)
               return 1;
           else
               return 0;
       }
    else
	   {
           if(tracknode->branch.down->id==nextNode)
               return 0;
           else
               return 1;
       }
} 

//小火车状态转换函数 
void trainStatusSwitcher(int id)
{
    train currenttrain = NULL;
    trackNode nexttrackNode;
    trainQueueNode  currentdata;
    trainQueueNode  newcurrentdata;
    trainQueueNode  anothercurrentdata;
    int next,i,k;
    queueNode p,q = NULL;
    for(i=0;i<=(MAXITEM-1);i++)
    {
        if(trainList[i]->id==id)
            currenttrain=trainList[i];
    }
    currenttrain->status=FREE;
	   switch(currenttrain->status)
        {
        case FREE:
            currentdata=(trainQueueNode)currenttrain->mission->head->next->data;
            if(currentdata->type==TSTATION)   //小火车接到新任务
	        	  {
                      currenttrain->status=RUN;
                      fprintf(outputLog,"at %lums train%d status changes from FREE to RUN.\n",RUN_TIME,id);
                  }//输出小火车的状态转换情况
            break;
        case RUN:
            next=currenttrain->nextNode;	 //小火车下一个节点编号
            for(k=0;k<=(MAXITEM-1);k++)
            {
                if(trackNodeList[k]->id==next)
                    nexttrackNode=trackNodeList[k];
            }
            if(nexttrackNode->type==BRANCH&&arriveresponddomain(id)==1)  //到达分叉节点的响应区间
            {
                branchNodeStatusSwitcher(ENTER,id,next);
                currenttrain->status=RUN;
            }
            else if (nexttrackNode->type==TRAFFIC&&arriveresponddomain(id)==1)//到达十字路节点的响应区间
            {
                trafficNodeStatusSwitcher(ENTER,id,next);
                currenttrain->status=RUN;
            }
            else if((nexttrackNode->type==TRAFFIC)&&(currenttrain->distance<=1))//小火车逼近十字路节点
            {
                trafficNodeStatusSwitcher(PASS,id,next);
                if(currenttrain->flag==forbidden)
                {
                    currenttrain->status=PAUSE;
                    fprintf(outputLog,"at %lums train%d status changes from RUN to PAUSE.\n",RUN_TIME,id);
                }
                else
                    currenttrain->status=RUN;
            }
            else if((nexttrackNode->type==BRANCH)&&currenttrain->distance<=1)//小火车逼近公共轨道
            {
                if(branchtype(id,next)==1)  //小火车即将进入公共轨道
                {
                    branchNodeStatusSwitcher(PASS,id,next);
                    if(currenttrain->flag==forbidden)
                    {currenttrain->status=PAUSE;
                        fprintf(outputLog,"at %lums train%d status changes from RUN to PAUSE.\n",RUN_TIME,id);
                    }
                    else
                    {
                        currenttrain->status=RUN;
                        nexttrackNode->branch.flag=1;   //小火车进入公共轨道后，将该段轨道标记为“忙”
                        nexttrackNode->branch.pair->branch.flag=1;
                    }
                }
                else                                           //小火车出公共轨道，将该段轨道标记为空闲
                    nexttrackNode->branch.flag=0;
                nexttrackNode->branch.pair->branch.flag=0;
            }
            else if(nexttrackNode->type==STATION&&currenttrain->distance==0)//到站
				        {
                            currenttrain->status=STA;
                            fprintf(outputLog,"at %lums train%d status changes from RUN to STA.\n",RUN_TIME,id);
                        }
            break;
        case STA:
            if(servicePolicy==SEQUENCING)
            {
                currentdata=(trainQueueNode)currenttrain->mission->head->next->data;//小火车当前任务
                if(currentdata->time<=(RUN_TIME-nexttrackNode->station.stop))   //达到要求的停靠时间
                    pop(currenttrain->mission);//更新小火车任务队列
				        }
            else if(servicePolicy==BYTHEWAY)
            {
                p=currenttrain->mission->head->next;
                anothercurrentdata=(trainQueueNode)(p->data);
                for(;(anothercurrentdata->station)!=currenttrain->nextNode;)//找到前方有任务车站对应的小火车的任务节点
                {
                    q=p;
                    p=p->next;
                    anothercurrentdata=(trainQueueNode)(p->data);
                }
                if(anothercurrentdata->time<=(RUN_TIME-nexttrackNode->station.stop))   //达到要求的停靠时间
                {
                    if(p==currenttrain->mission->head->next)  //完成的任务是小火车头结点后的首任务
                        q=currenttrain->mission->head;
                    deleteAfter(currenttrain->mission, q);//更新小火车任务队列
                }
            }
            if((currenttrain->mission->head)==(currenttrain->mission->tail)) //且小火车没有未完成的任务
            {
                currenttrain->status=FREE;
                fprintf(outputLog,"at %lums train%d status changes from STA to FREE.\n",RUN_TIME,id);
            }
            else if((currenttrain->mission->head)!=(currenttrain->mission->tail)) //且小火车有未完成的任务
            {
                currenttrain->status=RUN;
                fprintf(outputLog,"at %lums train%d status changes from STA to RUN.\n",RUN_TIME,id);
            }
            newcurrentdata=(trainQueueNode)currenttrain->mission->head->next->data;
            if(newcurrentdata->type==TLOCK)   //小火车只剩最后的锁闭任务
            {currenttrain->status=LOCK;
                fprintf(outputLog,"at %lums train%d status changes from STA to LOCK.\n",RUN_TIME,id);}
            break;
        case PAUSE:
            if((nexttrackNode->type==TRAFFIC)&&(currenttrain->distance<=1))  //等待进入十字路节点
            {
                trafficNodeStatusSwitcher(PASS, id, currenttrain->nextNode);
                if(currenttrain->flag==permitted)
                {currenttrain->status=RUN;
                    fprintf(outputLog,"at %lums train%d status changes from PAUSE to RUN.\n",RUN_TIME,id);}
            }
            else if((nexttrackNode->type==BRANCH)&&(branchtype(id,nexttrackNode->id)==1)&&(currenttrain->distance<=1)) //等待进入公共轨道
            {
                branchNodeStatusSwitcher(PASS, id, currenttrain->nextNode);
                if(currenttrain->flag==permitted)
                {currenttrain->status=RUN;
                    fprintf(outputLog,"at %lums train%d status changes from PAUSE to RUN.\n",RUN_TIME,id);}
            }
            break;
    }
}
                          
//十字路节点状态机转换函数					  
void trafficNodeStatusSwitcher(request req, int trainID, int trackNodeID) 
{int i,k,m;
trackNode  currenttrackNode = NULL;
train   currenttrain = NULL,firsttrain = NULL,secondtrain = NULL;
 for(i=0;i<=(MAXITEM-1);i++) 
     {if(trackNodeList[i]->id==trackNodeID)
        currenttrackNode=trackNodeList[i];
		}
    for(k=0;k<=(MAXITEM-1);k++) 
        {if(trainList[k]->id==trainID)
        currenttrain=trainList[k];}
		
	currenttrackNode->traffic.status=0;
    switch(currenttrackNode->traffic.status) 
	     {
		   case 0:
		       if(req==ENTER)   
                  {currenttrackNode->traffic.status=1;
                  fprintf(outputLog,"at %lums trafficNode%d status changes from 0 to 1.\n",RUN_TIME,trackNodeID);//输出十字路节点的状态转换情况 
                    currenttrain->flag=permitted;
                  if(currenttrackNode->traffic.train[0]==-1)
                    currenttrackNode->traffic.train[0]=trainID;
                   else if(currenttrackNode->traffic.train[1]==-1)
                    currenttrackNode->traffic.train[1]=trainID;} 
              if(req==PASS&&trainID!=currenttrackNode->traffic.train[0]&&trainID!=currenttrackNode->traffic.train[1])
                     ;        //小火车通过十字路节点后仍发出PASS指令的情况 
               break;
          case 1:
               if(req==ENTER)   
                  {if(trainID!=currenttrackNode->traffic.train[0]&&trainID!=currenttrackNode->traffic.train[1])  //区别出小火车在响应区间内多次发出ENTER指令的情况 
				   {currenttrackNode->traffic.status=2;
				   fprintf(outputLog,"at %lums trafficNode%d status changes from 1 to 2.\n",RUN_TIME,trackNodeID);
                   if(currenttrackNode->traffic.train[0]==-1)
                    currenttrackNode->traffic.train[0]=trainID;
                   else if(currenttrackNode->traffic.train[1]==-1)
                    currenttrackNode->traffic.train[1]=trainID;
                    firstgo=judge(currenttrackNode->traffic.train[0],currenttrackNode->traffic.train[1]); //根据当前策略判断出两辆竞争的小火车谁先走 
				   for(m=0;m<=(MAXITEM-1);m++) 
                     {if(trainList[m]->id==currenttrackNode->traffic.train[0])
                       firsttrain=trainList[m];
				      else if(trainList[m]->id==currenttrackNode->traffic.train[1])
					     secondtrain=trainList[m];}
					if(firstgo==currenttrackNode->traffic.train[0])
					  {firsttrain->flag=permitted;
				       secondtrain->flag=forbidden;}
				   else
				     {secondtrain->flag=permitted;
				       firsttrain->flag=forbidden;}
				     } }
				else if(req==PASS&&currenttrain->flag==permitted)
                   {if(trainID==currenttrackNode->traffic.train[0]||trainID==currenttrackNode->traffic.train[1]) //区别小火车通过十字路节点后仍发出PASS指令的情况 
				     {currenttrackNode->traffic.status=0;
				     fprintf(outputLog,"at %lums trafficNode%d status changes from 1 to 0.\n",RUN_TIME,trackNodeID);
                   if(trainID==currenttrackNode->traffic.train[0])//删除十字路节点表示竞争的小火车数组中已通过的小火车id
				     currenttrackNode->traffic.train[0]=-1;
				    else
				      currenttrackNode->traffic.train[1]=-1;}}
				    break;
       
	  case 2:if(req==ENTER) 
	  	     {if(trainID==currenttrackNode->traffic.train[0]||trainID==currenttrackNode->traffic.train[1])       //区别出小火车在响应区间内多次发出ENTER指令的情况  
	  	            ;}
	  	      else if((req==PASS)&&(currenttrain->flag==permitted))
                  { 
                    currenttrackNode->traffic.status=1;
                    fprintf(outputLog,"at %lums trafficNode%d status changes from 2 to 1.\n",RUN_TIME,trackNodeID);
					if(controlPolicy==AUTO) 
                      (trainList[trainID]->passTimes)++; //交替策略下先走的小火车先行数加一 
                   if(trainID==currenttrackNode->traffic.train[0]) //删除十字路节点表示竞争的小火车数组中已通过的小火车id 
				     {currenttrackNode->traffic.train[0]=-1;
				     for(m=0;m<=(MAXITEM-1)&&trainList[m]->id!=currenttrackNode->traffic.train[1];m++) 
                                ;
				      trainList[m]->flag=permitted;}  //另一辆小火车的flag置为permitted 
				  else
				      {currenttrackNode->traffic.train[1]=-1;
					 for(m=0;m<=(MAXITEM-1)&&trainList[m]->id!=currenttrackNode->traffic.train[0];m++) 
                                ;
				      trainList[m]->flag=permitted;}}
				break;
        }}
        
        
//分叉点状态机转换函数 
void branchNodeStatusSwitcher(request req, int trainID, int trackNodeID) 
{ int i,k,m,n;
  trackNode currenttrackNode = NULL;
  train currenttrain = NULL,firsttrain = NULL,secondtrain = NULL;
  for(i=0;i<=(MAXITEM-1);i++) 
     {if(trackNodeList[i]->id==trackNodeID)
        currenttrackNode=trackNodeList[i];}
  for(k=0;k<=(MAXITEM-1);k++) 
        {if(trainList[k]->id==trainID)
        currenttrain=trainList[k];}
   currenttrackNode->branch.status=0;
 switch(currenttrackNode->branch.status){
  	  case 0:
	   if(req==ENTER)
	  	   {currenttrackNode->branch.status=1;
	  	   fprintf(outputLog,"at %lums branchNode%d status changes from 0 to 1.\n",RUN_TIME,trackNodeID);//输出分叉节点的状态转换情况 
	  	    currenttrain->flag=permitted;
            if(currenttrackNode->branch.train[0]==-1)   //将小火车加入分叉点的竞争数组 
                    currenttrackNode->branch.train[0]=trainID;
             else if(currenttrackNode->branch.train[1]==-1)
                    currenttrackNode->branch.train[1]=trainID;}
       else if(req==PASS&&trainID!=currenttrackNode->branch.train[0]&&trainID!=currenttrackNode->branch.train[1])
                     ;  //区别小火车通过分叉点后仍发出PASS指令的情况 
         break;
	  	
	case  1:
	  	if(req==ENTER)   //区别出小火车在响应区间内多次发出ENTER指令的情况 
			{if(trainID!=currenttrackNode->branch.train[0]&&trainID!=currenttrackNode->branch.train[1])
			   {currenttrackNode->branch.status=2;
			   fprintf(outputLog,"at %lums branchNode%d status changes from 1 to 2.\n",RUN_TIME,trackNodeID);
			      if(currenttrackNode->branch.train[0]==-1)
                    currenttrackNode->branch.train[0]=trainID;
                   else if(currenttrackNode->branch.train[1]==-1)
                    currenttrackNode->branch.train[1]=trainID;
                    firstgo=judge(currenttrackNode->branch.train[0],currenttrackNode->branch.train[1]);//根据当前策略判断出两辆竞争的小火车谁先进入公共轨道 
                   for(m=0;m<=(MAXITEM-1);m++) 
                     {if(trainList[m]->id==currenttrackNode->branch.train[0])
                       firsttrain=trainList[m];
				      else if(trainList[m]->id==currenttrackNode->branch.train[1])
					     secondtrain=trainList[m];}
					if(firstgo==currenttrackNode->branch.train[0])
					  {firsttrain->flag=permitted;
				       secondtrain->flag=forbidden;}
			else
				{secondtrain->flag=permitted;
				firsttrain->flag=forbidden;} }}
		else if(req==PASS&&currenttrain->flag==permitted)   
		     {if(trainID==currenttrackNode->branch.train[0]||trainID==currenttrackNode->branch.train[1])//区别小火车通过分叉点后仍发出PASS指令的情况
			   {if(checkTrack(trackNodeID,currenttrackNode->branch.pair->id)==0)  //根据公共轨道的忙和空闲情况判断小火车是否能进入公共轨道 
                   currenttrain->flag=permitted;
				 else
				   currenttrain->flag=forbidden;
				 if((currenttrain->flag=permitted))
				  {currenttrackNode->branch.status=0;
				  fprintf(outputLog,"at %lums branchNode%d status changes from 1 to 0.\n",RUN_TIME,trackNodeID);
                    if(trainID==currenttrackNode->branch.train[0])//删除分叉节点表示竞争的小火车数组中已通过的小火车id
                    currenttrackNode->branch.train[0]=-1;
                   else if(trainID==currenttrackNode->branch.train[1])
                    currenttrackNode->branch.train[1]=-1;}}}
				break;
		
		case 2:
			if(req==ENTER) //区别出小火车在响应区间内多次发出ENTER指令的情况
			{if(trainID==currenttrackNode->branch.train[0]||trainID==currenttrackNode->branch.train[1])
	  	            ;}
		    else if((req==PASS)&&(currenttrain->flag==permitted))
                  { if(checkTrack(trackNodeID,currenttrackNode->branch.pair->id)==0) //根据公共轨道的忙和空闲情况判断小火车是否能进入公共轨道 
				     currenttrain->flag=permitted;
				   else
				   currenttrain->flag=forbidden;
				   if(currenttrain->flag==permitted)
				   {currenttrackNode->branch.status=1;
				   fprintf(outputLog,"at %lums branchNode%d status changes from 2 to 1.\n",RUN_TIME,trackNodeID);
                      if(controlPolicy==AUTO) 
                      (trainList[trainID]->passTimes)++;//交替策略下先走的小火车先行数加一 
                    if(trainID==currenttrackNode->branch.train[0])//删除分叉节点表示竞争的小火车数组中已通过的小火车id
				     {currenttrackNode->branch.train[0]=-1;
				     for(n=0;n<=(MAXITEM-1)&&trainList[n]->id!=currenttrackNode->branch.train[1];n++) 
                                ;
				      trainList[n]->flag=permitted;}//另一辆小火车的flag置为permitted
				  else
				      {currenttrackNode->branch.train[1]=-1;
					 for(n=0;n<=(MAXITEM-1)&&trainList[n]->id!=currenttrackNode->branch.train[0];n++) 
                                ;
				      trainList[n]->flag=permitted;}} 
					  }
		   break;	}}	 
				 
				   
              
		        
			  
				       
	       
	          
      	
        
        
        
