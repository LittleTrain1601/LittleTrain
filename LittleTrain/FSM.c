//
//  FSM.c
//  LittleTrain
//
//  Created by 陈曦远 on 16/5/16.
//  Copyright ? 2016年 1601. All rights reserved.
//


#include "train.h"
#include <stdio.h>

enum _servicePolicy servicePolicy;
enum policy controlPolicy;
 
int firstgo;
int arriveresponddomain(int id);
int branchtype(int trainid,int tracknodeid);

    	
//判断id号火车是否到达分叉节点或十字路节点的响应区间 ，是返回1，否返回0 
int arriveresponddomain(int id){
    train currenttrain = NULL;
    trackNode nexttrackNode = NULL;
    int lastNode;//小火车走过的上一个节点
    int m,n;
    int range; //下一分叉节点或十字路节点的响应区间长度
    currenttrain=trainList[id];
    nexttrackNode=trackNodeList[currenttrain->nextNode];
    for(m=0;m<=(MAXITEM-1)&&(currenttrain->nodeList[m]!=currenttrain->nextNode);m++);
    if(nexttrackNode->type!=BRANCH&&nexttrackNode->type!=TRAFFIC)
        return 0;
    else if(nexttrackNode->type==BRANCH){
        if(branchtype(id,currenttrain->nextNode)==0)
            return 0;
        else
        {if(currenttrain->direction==clockwise)
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
            lastNode=currenttrain->nodeList[0];}
            if(lastNode==nexttrackNode->branch.left->id)
                range=nexttrackNode->branch.lrange;
            else if(lastNode==nexttrackNode->branch.right->id)
                range=nexttrackNode->branch.rrange;
            
            if(currenttrain->distance<=range&&currenttrain->distance>1)   //小火车进入响应区间
                return 1;
            else
                return 0;
        }
    } else {
        if(currenttrain->direction==clockwise){
            if(m!=0) {
                lastNode=currenttrain->nodeList[m-1];
            } else {
                for(n=0;n<=(MAXITEM-1)&&(currenttrain->nodeList[n]!=-1);n++);
                lastNode=currenttrain->nodeList[n-1];
            }
        } else {
            if(currenttrain->nodeList[m+1]!=-1) {
                lastNode=currenttrain->nodeList[m+1];
            } else {
                lastNode=currenttrain->nodeList[0];
            }
        }
        if(lastNode==nexttrackNode->traffic.down->id) {
            range=nexttrackNode->traffic.drange;
        } else if(lastNode==nexttrackNode->traffic.left->id) {
            range=nexttrackNode->traffic.lrange;
        } else if(lastNode==nexttrackNode->traffic.right->id) {
            range=nexttrackNode->traffic.rrange;
        } else if(lastNode==nexttrackNode->traffic.up->id) {
            range=nexttrackNode->traffic.urange;
        }
        if(currenttrain->distance<=range&&currenttrain->distance>1) {
            return 1;
        } else {
            return 0;
        }
    }
}


//若分叉节点是小火车当前运行方向下进入公共轨道的节点返回1，若分叉节点是小火车当前运行方向下出公共轨道的节点返回0 
int branchtype(int trainid,int tracknodeid){
    int m,n;
    train currenttrain = NULL;
    trackNode  tracknode = NULL;
    int lastNode,nextNode;
   currenttrain=trainList[trainid];
   tracknode=trackNodeList[tracknodeid];
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
    trackNode nexttrackNode=NULL;
    trainQueueNode  currentdata=NULL;
    trainQueueNode  newcurrentdata=NULL;
    trainQueueNode  anothercurrentdata=NULL;
    int next;
    queueNode p,q,s,t = NULL;
    currenttrain = trainList[id];
	   switch(currenttrain->status)
        {
        case FREE:
                if (currenttrain->mission->head->next) {
                    currentdata=(trainQueueNode)currenttrain->mission->head->next->data;
                    if(currentdata->type==TSTATION)   //小火车接到新任务
                    {
                        currenttrain->status=RUN;
                        fprintf(outputLog,"at %lums train%d status changes from FREE to RUN.\n",RUN_TIME,id);
                        //输出小火车的状态转换情况
                    } else if (currentdata->type == TLOCK) {
                        currenttrain->status = LOCK;
                    }
                }
            
            break;
        case RUN:
            next=currenttrain->nextNode;	 //小火车下一个节点编号
            nexttrackNode=trackNodeList[next];
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
                    {nexttrackNode->branch.flag=0;
                   nexttrackNode->branch.pair->branch.flag=0;
                    }
            }
            else if(nexttrackNode->type==STATION&&currenttrain->distance==0)//到站
				        {
                            currenttrain->status=STA;
                            fprintf(outputLog,"at %lums train%d status changes from RUN to STA.\n",RUN_TIME,id);
                        }
            break;
        case STA:
            next=currenttrain->nextNode;	 //小火车下一个节点编号
            nexttrackNode=trackNodeList[next];
            if(servicePolicy==SEQUENCING)
            {
                currentdata=(trainQueueNode)currenttrain->mission->head->next->data;//小火车当前任务
                if((currentdata->time)*CLOCKS_PER_SEC<=(RUN_TIME-nexttrackNode->station.stop)) {   //达到要求的停靠时间
                    pop(currenttrain->mission);//更新小火车任务队列
                    currenttrain->status=RUN;
                    fprintf(outputLog,"at %lums train%d status changes from STA to RUN.\n",RUN_TIME,id);
                }
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
                if((currentdata->time)*CLOCKS_PER_SEC<=(RUN_TIME-nexttrackNode->station.stop))   //达到要求的停靠时间
                {
                    if(p==currenttrain->mission->head->next)  //完成的任务是小火车头结点后的首任务
                        q=currenttrain->mission->head;
                    deleteAfter(currenttrain->mission, q);//更新小火车任务队列
                     s=currenttrain->mission->head->next;    //更新小火车任务队列后遍历链表 
                    anothercurrentdata=(trainQueueNode)(s->data);
                for(;(s!=currenttrain->mission->tail)&&(anothercurrentdata->station)!=currenttrain->nextNode;)//找到前方有任务车站对应的小火车的任务节点
                {
                    t=s;
                    s=s->next;
                    anothercurrentdata=(trainQueueNode)(s->data);
                }
                    if(s==currenttrain->mission->tail)    //在当前站点没有其他任务 
					{currenttrain->status=RUN;
                    fprintf(outputLog,"at %lums train%d status changes from STA to RUN.\n",RUN_TIME,id);
                    }
                    else  //在当前站点还有其他任务 
                       {fprintf(outputLog,"another mission at the station.\n")
					        ;} 
                   }
                }
            
            if((currenttrain->mission->head)==(currenttrain->mission->tail)) //且小火车没有未完成的任务
            {
                currenttrain->status=FREE;
                fprintf(outputLog,"at %lums train%d status changes from STA to FREE.\n",RUN_TIME,id);
            }
            else if((currenttrain->mission->head)!=(currenttrain->mission->tail)) //且小火车有未完成的任务
            {  newcurrentdata=(trainQueueNode)currenttrain->mission->head->next->data;
			    if(newcurrentdata->type==TLOCK)   //小火车只剩最后的锁闭任务
                {
                    currenttrain->status=LOCK;
                    fprintf(outputLog,"at %lums train%d status changes from STA to LOCK.\n",RUN_TIME,id);
                }
            }
            break;
        case PAUSE:
                next=currenttrain->nextNode;	 //小火车下一个节点编号
                nexttrackNode=trackNodeList[next];
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
{
    trackNode currenttrackNode = NULL;
    train currenttrain = NULL,firsttrain = NULL,secondtrain = NULL;
    currenttrackNode=trackNodeList[trackNodeID];
    currenttrain=trainList[trainID];
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
                firsttrain=trainList[currenttrackNode->traffic.train[0]];
                secondtrain=trainList[currenttrackNode->traffic.train[1]];
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
                trainList[currenttrackNode->traffic.train[1]]->flag=permitted;}  //另一辆小火车的flag置为permitted
            else
            {currenttrackNode->traffic.train[1]=-1;
                trainList[currenttrackNode->traffic.train[0]]->flag=permitted;}
            break;
        }
    }
    
}
//分叉点状态机转换函数
void branchNodeStatusSwitcher(request req, int trainID, int trackNodeID) 
{
  trackNode currenttrackNode = NULL;
  train currenttrain = NULL,firsttrain = NULL,secondtrain = NULL;
  currenttrackNode=trackNodeList[trackNodeID];
  currenttrain=trainList[trainID]; 
    switch(currenttrackNode->branch.status){
  	  case 0:
	   if(req==ENTER)
	  	   {currenttrackNode->branch.status=1;
               currenttrackNode->branch.pair->branch.status=1;
	  	   fprintf(outputLog,"at %lums branchNode%d and its pair status changes from 0 to 1.\n",RUN_TIME,trackNodeID);//输出分叉节点的状态转换情况
	  	    currenttrain->flag=permitted;
            if(currenttrackNode->branch.train[0]==-1&&currenttrackNode->branch.pair->branch.train[0]==-1)   //将小火车加入分叉点的竞争数组
              {currenttrackNode->branch.train[0]=trainID;
                  currenttrackNode->branch.pair->branch.train[0]=trainID;}
            else if(currenttrackNode->branch.train[1]==-1&&currenttrackNode->branch.pair->branch.train[0]==-1)
            {currenttrackNode->branch.train[1]=trainID;
                currenttrackNode->branch.pair->branch.train[0]=trainID;}
            else if(currenttrackNode->branch.train[0]==-1&&currenttrackNode->branch.pair->branch.train[1]==-1)
            {currenttrackNode->branch.train[0]=trainID;
                currenttrackNode->branch.pair->branch.train[1]=trainID;}
            else if(currenttrackNode->branch.train[1]==-1&&currenttrackNode->branch.pair->branch.train[1]==-1)
            {currenttrackNode->branch.train[1]=trainID;
                currenttrackNode->branch.pair->branch.train[1]=trainID;}     }
       else if(req==PASS&&trainID!=currenttrackNode->branch.train[0]&&trainID!=currenttrackNode->branch.train[1])
                     ;  //区别小火车通过分叉点后仍发出PASS指令的情况 
         break;
	  	
	case  1:
	  	if(req==ENTER)   //区别出小火车在响应区间内多次发出ENTER指令的情况 
			{if(trainID!=currenttrackNode->branch.train[0]&&trainID!=currenttrackNode->branch.train[1])
			   {currenttrackNode->branch.status=2;
                   currenttrackNode->branch.pair->branch.status=2;
			   fprintf(outputLog,"at %lums branchNode%d and its pair status changes from 1 to 2.\n",RUN_TIME,trackNodeID);
			      if(currenttrackNode->branch.train[0]==-1&&currenttrackNode->branch.pair->branch.train[0]==-1)
                    {currenttrackNode->branch.train[0]=trainID;
                        currenttrackNode->branch.pair->branch.train[0]=trainID;}
                   else if(currenttrackNode->branch.train[0]==-1&&currenttrackNode->branch.pair->branch.train[1]==-1)
                   {currenttrackNode->branch.train[0]=trainID;
                       currenttrackNode->branch.pair->branch.train[1]=trainID;}
                   else if(currenttrackNode->branch.train[1]==-1&&currenttrackNode->branch.pair->branch.train[0]==-1)
                   {currenttrackNode->branch.train[1]=trainID;
                       currenttrackNode->branch.pair->branch.train[0]=trainID;}
                   else if(currenttrackNode->branch.train[1]==-1&&currenttrackNode->branch.pair->branch.train[1]==-1)
                   {currenttrackNode->branch.train[1]=trainID;
                       currenttrackNode->branch.pair->branch.train[1]=trainID;}
                   firstgo=judge(currenttrackNode->branch.train[0],currenttrackNode->branch.train[1]);//根据当前策略判断出两辆竞争的小火车谁先进入公共轨道
                   firsttrain=trainList[currenttrackNode->branch.train[0]];
				    secondtrain=trainList[currenttrackNode->branch.train[1]];
					if(firstgo==currenttrackNode->branch.train[0])
					  {firsttrain->flag=permitted;
				       secondtrain->flag=forbidden;}
			else
				{secondtrain->flag=permitted;
				firsttrain->flag=forbidden;} }}
		else if(req==PASS&&currenttrain->flag==permitted)   
		     {if(trainID==currenttrackNode->branch.train[0]||trainID==currenttrackNode->branch.train[1])//区别小火车通过分叉点后仍发出PASS指令的情况
			   {if(checkTrack(trainID, trackNodeID,currenttrackNode->branch.pair->id)==0)  //根据公共轨道的忙和空闲情况判断小火车是否能进入公共轨道
                   currenttrain->flag=permitted;
				 else
				   currenttrain->flag=forbidden;
				 if(currenttrain->flag==permitted)
				  {currenttrackNode->branch.status=0;
                      currenttrackNode->branch.pair->branch.status=0;
				  fprintf(outputLog,"at %lums branchNode%d and its pair status changes from 1 to 0.\n",RUN_TIME,trackNodeID);
                    if(trainID==currenttrackNode->branch.train[0]&&trainID==currenttrackNode->branch.pair->branch.train[0])//删除分叉节点表示竞争的小火车数组中已通过的小火车id
                    {currenttrackNode->branch.train[0]=-1;
                        currenttrackNode->branch.pair->branch.train[0]=-1;}
                     else if(trainID==currenttrackNode->branch.train[0]&&trainID==currenttrackNode->branch.pair->branch.train[1])//删除分叉节点表示竞争的小火车数组中已通过的小火车id
                      {currenttrackNode->branch.train[0]=-1;
                          currenttrackNode->branch.pair->branch.train[1]=-1;}
                     else if(trainID==currenttrackNode->branch.train[1]&&trainID==currenttrackNode->branch.pair->branch.train[0])//删除分叉节点表示竞争的小火车数组中已通过的小火车id
                      {currenttrackNode->branch.train[1]=-1;
                          currenttrackNode->branch.pair->branch.train[0]=-1;}
                      if(trainID==currenttrackNode->branch.train[1]&&trainID==currenttrackNode->branch.pair->branch.train[1])//删除分叉节点表示竞争的小火车数组中已通过的小火车id
                      {currenttrackNode->branch.train[1]=-1;
                          currenttrackNode->branch.pair->branch.train[1]=-1;}
                  }
               }
             }
        else if (req==PASS&&currenttrain->flag==forbidden) {
            if(checkTrack(trainID, trackNodeID,currenttrackNode->branch.pair->id)==0)  //根据公共轨道的忙和空闲情况判断小火车是否能进入公共轨道
            {currenttrain->flag=permitted;}
            else
                ;}
            
            break;
		
		case 2:
			if(req==ENTER) //区别出小火车在响应区间内多次发出ENTER指令的情况
			{if(trainID==currenttrackNode->branch.train[0]||trainID==currenttrackNode->branch.train[1])
	  	            ;}
		    else if((req==PASS)&&(currenttrain->flag==permitted))
                  { if(checkTrack(trainID, trackNodeID,currenttrackNode->branch.pair->id)==0) //根据公共轨道的忙和空闲情况判断小火车是否能进入公共轨道
				     currenttrain->flag=permitted;
				   else
				   currenttrain->flag=forbidden;
				   if(currenttrain->flag==permitted)
				   {currenttrackNode->branch.status=1;
                       currenttrackNode->branch.pair->branch.status=1;
				   fprintf(outputLog,"at %lums branchNode%d and its pair status changes from 2 to 1.\n",RUN_TIME,trackNodeID);
                      if(controlPolicy==AUTO) 
                      (trainList[trainID]->passTimes)++;//交替策略下先走的小火车先行数加一 
                    if(trainID==currenttrackNode->branch.train[0]&&trainID==currenttrackNode->branch.pair->branch.train[0])//删除分叉节点表示竞争的小火车数组中已通过的小火车id
				     {currenttrackNode->branch.train[0]=-1;
                         currenttrackNode->branch.pair->branch.train[0]=-1;
				    trainList[currenttrackNode->branch.train[1]]->flag=permitted;}//另一辆小火车的flag置为permitted
				  else if(trainID==currenttrackNode->branch.train[0]&&trainID==currenttrackNode->branch.pair->branch.train[1])
                  {currenttrackNode->branch.train[0]=-1;
                      currenttrackNode->branch.pair->branch.train[1]=-1;
                      trainList[currenttrackNode->branch.train[1]]->flag=permitted;}
                       
                  else if(trainID==currenttrackNode->branch.train[1]&&trainID==currenttrackNode->branch.pair->branch.train[0])
                  {currenttrackNode->branch.train[1]=-1;
                      currenttrackNode->branch.pair->branch.train[0]=-1;
                      trainList[currenttrackNode->branch.train[0]]->flag=permitted;}
                       
                  else if(trainID==currenttrackNode->branch.train[1]&&trainID==currenttrackNode->branch.pair->branch.train[1])
                  {currenttrackNode->branch.train[1]=-1;
                      currenttrackNode->branch.pair->branch.train[1]=-1;
                      trainList[currenttrackNode->branch.train[0]]->flag=permitted;}
                   }
                  }
            else if((req==PASS)&&(currenttrain->flag==forbidden)){
                if(checkTrack(trainID, trackNodeID,currenttrackNode->branch.pair->id)==0)  //根据公共轨道的忙和空闲情况判断小火车是否能进入公共轨道
                {currenttrain->flag=permitted;}
                else
                    ;
            }
		   break;	}
}
				 
				   
              
		        
			  
				       
	       
	          
      	
        
        
        

