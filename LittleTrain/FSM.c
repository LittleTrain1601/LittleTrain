//
//  FSM.c
//  LittleTrain
//
//  Created by ����Զ on 16/5/16.
//  Copyright ? 2016�� 1601. All rights reserved.
//


#include "train.h"
#include <stdio.h>

enum _servicePolicy servicePolicy;
enum policy controlPolicy;
 
int firstgo;
int arriveresponddomain(int id);
int branchtype(int trainid,int tracknodeid);

    	
//�ж�id�Ż��Ƿ񵽴�ֲ�ڵ��ʮ��·�ڵ����Ӧ���� ���Ƿ���1���񷵻�0 
int arriveresponddomain(int id){
    train currenttrain = NULL;
    trackNode nexttrackNode = NULL;
    int lastNode;//С���߹�����һ���ڵ�
    int m,n;
    int range; //��һ�ֲ�ڵ��ʮ��·�ڵ����Ӧ���䳤��
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
        {if(m!=0)                                 //ͬһС�𳵹���ϵĽڵ㰴˳ʱ��˳�򴢴�
            lastNode=currenttrain->nodeList[m-1];
        else    //С�𳵼�������Ľڵ���С�𳵹�������е��±�Ϊ0
        {for(n=0;n<=(MAXITEM-1)&&(currenttrain->nodeList[n]!=-1);n++)
        {;}
            lastNode=currenttrain->nodeList[n-1];}}
        else
        {if(currenttrain->nodeList[m+1]!=-1)
            lastNode=currenttrain->nodeList[m+1];
        else //С�𳵼�������Ľڵ���С�𳵹�������е����һ��Ԫ��
            lastNode=currenttrain->nodeList[0];}
            if(lastNode==nexttrackNode->branch.left->id)
                range=nexttrackNode->branch.lrange;
            else if(lastNode==nexttrackNode->branch.right->id)
                range=nexttrackNode->branch.rrange;
            
            if(currenttrain->distance<=range&&currenttrain->distance>1)   //С�𳵽�����Ӧ����
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


//���ֲ�ڵ���С�𳵵�ǰ���з����½��빫������Ľڵ㷵��1�����ֲ�ڵ���С�𳵵�ǰ���з����³���������Ľڵ㷵��0 
int branchtype(int trainid,int tracknodeid){
    int m,n;
    train currenttrain = NULL;
    trackNode  tracknode = NULL;
    int lastNode,nextNode;
   currenttrain=trainList[trainid];
   tracknode=trackNodeList[tracknodeid];
    for(m=0;m<=(MAXITEM-1)&&(currenttrain->nodeList[m]!=tracknodeid);m++)
    {;}
    //˳ʱ��Ѱ��һ��С�𳵹���ϱ��Ϊtracknodeid�Ľڵ��ǰһ���ͺ�һ���ڵ�
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

//С��״̬ת������ 
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
                    if(currentdata->type==TSTATION)   //С�𳵽ӵ�������
                    {
                        currenttrain->status=RUN;
                        fprintf(outputLog,"at %lums train%d status changes from FREE to RUN.\n",RUN_TIME,id);
                        //���С�𳵵�״̬ת�����
                    } else if (currentdata->type == TLOCK) {
                        currenttrain->status = LOCK;
                    }
                }
            
            break;
        case RUN:
            next=currenttrain->nextNode;	 //С����һ���ڵ���
            nexttrackNode=trackNodeList[next];
            if(nexttrackNode->type==BRANCH&&arriveresponddomain(id)==1)  //����ֲ�ڵ����Ӧ����
            {
                branchNodeStatusSwitcher(ENTER,id,next);
                currenttrain->status=RUN;
            }
            else if (nexttrackNode->type==TRAFFIC&&arriveresponddomain(id)==1)//����ʮ��·�ڵ����Ӧ����
            {
                trafficNodeStatusSwitcher(ENTER,id,next);
                currenttrain->status=RUN;
            }
            else if((nexttrackNode->type==TRAFFIC)&&(currenttrain->distance<=1))//С�𳵱ƽ�ʮ��·�ڵ�
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
            else if((nexttrackNode->type==BRANCH)&&currenttrain->distance<=1)//С�𳵱ƽ��������
            {
                if(branchtype(id,next)==1)  //С�𳵼������빫�����
                {
                    branchNodeStatusSwitcher(PASS,id,next);
                    if(currenttrain->flag==forbidden)
                    {currenttrain->status=PAUSE;
                        fprintf(outputLog,"at %lums train%d status changes from RUN to PAUSE.\n",RUN_TIME,id);
                    }
                    else
                    {
                        currenttrain->status=RUN;
                        nexttrackNode->branch.flag=1;   //С�𳵽��빫������󣬽��öι�����Ϊ��æ��
                        nexttrackNode->branch.pair->branch.flag=1;
                    }
                }
                else                                           //С�𳵳�������������öι�����Ϊ����
                    {nexttrackNode->branch.flag=0;
                   nexttrackNode->branch.pair->branch.flag=0;
                    }
            }
            else if(nexttrackNode->type==STATION&&currenttrain->distance==0)//��վ
				        {
                            currenttrain->status=STA;
                            fprintf(outputLog,"at %lums train%d status changes from RUN to STA.\n",RUN_TIME,id);
                        }
            break;
        case STA:
            next=currenttrain->nextNode;	 //С����һ���ڵ���
            nexttrackNode=trackNodeList[next];
            if(servicePolicy==SEQUENCING)
            {
                currentdata=(trainQueueNode)currenttrain->mission->head->next->data;//С�𳵵�ǰ����
                if((currentdata->time)*CLOCKS_PER_SEC<=(RUN_TIME-nexttrackNode->station.stop)) {   //�ﵽҪ���ͣ��ʱ��
                    pop(currenttrain->mission);//����С���������
                    currenttrain->status=RUN;
                    fprintf(outputLog,"at %lums train%d status changes from STA to RUN.\n",RUN_TIME,id);
                }
            }
            else if(servicePolicy==BYTHEWAY)
            {
                p=currenttrain->mission->head->next;
                anothercurrentdata=(trainQueueNode)(p->data);
                for(;(anothercurrentdata->station)!=currenttrain->nextNode;)//�ҵ�ǰ��������վ��Ӧ��С�𳵵�����ڵ�
                {
                    q=p;
                    p=p->next;
                    anothercurrentdata=(trainQueueNode)(p->data);
                }
                if((currentdata->time)*CLOCKS_PER_SEC<=(RUN_TIME-nexttrackNode->station.stop))   //�ﵽҪ���ͣ��ʱ��
                {
                    if(p==currenttrain->mission->head->next)  //��ɵ�������С��ͷ�����������
                        q=currenttrain->mission->head;
                    deleteAfter(currenttrain->mission, q);//����С���������
                     s=currenttrain->mission->head->next;    //����С��������к�������� 
                    anothercurrentdata=(trainQueueNode)(s->data);
                for(;(s!=currenttrain->mission->tail)&&(anothercurrentdata->station)!=currenttrain->nextNode;)//�ҵ�ǰ��������վ��Ӧ��С�𳵵�����ڵ�
                {
                    t=s;
                    s=s->next;
                    anothercurrentdata=(trainQueueNode)(s->data);
                }
                    if(s==currenttrain->mission->tail)    //�ڵ�ǰվ��û���������� 
					{currenttrain->status=RUN;
                    fprintf(outputLog,"at %lums train%d status changes from STA to RUN.\n",RUN_TIME,id);
                    }
                    else  //�ڵ�ǰվ�㻹���������� 
                       {fprintf(outputLog,"another mission at the station.\n")
					        ;} 
                   }
                }
            
            if((currenttrain->mission->head)==(currenttrain->mission->tail)) //��С��û��δ��ɵ�����
            {
                currenttrain->status=FREE;
                fprintf(outputLog,"at %lums train%d status changes from STA to FREE.\n",RUN_TIME,id);
            }
            else if((currenttrain->mission->head)!=(currenttrain->mission->tail)) //��С����δ��ɵ�����
            {  newcurrentdata=(trainQueueNode)currenttrain->mission->head->next->data;
			    if(newcurrentdata->type==TLOCK)   //С��ֻʣ������������
                {
                    currenttrain->status=LOCK;
                    fprintf(outputLog,"at %lums train%d status changes from STA to LOCK.\n",RUN_TIME,id);
                }
            }
            break;
        case PAUSE:
                next=currenttrain->nextNode;	 //С����һ���ڵ���
                nexttrackNode=trackNodeList[next];
            if((nexttrackNode->type==TRAFFIC)&&(currenttrain->distance<=1))  //�ȴ�����ʮ��·�ڵ�
            {
                trafficNodeStatusSwitcher(PASS, id, currenttrain->nextNode);
                if(currenttrain->flag==permitted)
                {currenttrain->status=RUN;
                    fprintf(outputLog,"at %lums train%d status changes from PAUSE to RUN.\n",RUN_TIME,id);}
            }
            else if((nexttrackNode->type==BRANCH)&&(branchtype(id,nexttrackNode->id)==1)&&(currenttrain->distance<=1)) //�ȴ����빫�����
            {
                branchNodeStatusSwitcher(PASS, id, currenttrain->nextNode);
                if(currenttrain->flag==permitted)
                {currenttrain->status=RUN;
                    fprintf(outputLog,"at %lums train%d status changes from PAUSE to RUN.\n",RUN_TIME,id);}
            }
            break;
    }
}
                          
//ʮ��·�ڵ�״̬��ת������					  
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
                fprintf(outputLog,"at %lums trafficNode%d status changes from 0 to 1.\n",RUN_TIME,trackNodeID);//���ʮ��·�ڵ��״̬ת�����
                currenttrain->flag=permitted;
                if(currenttrackNode->traffic.train[0]==-1)
                    currenttrackNode->traffic.train[0]=trainID;
                else if(currenttrackNode->traffic.train[1]==-1)
                    currenttrackNode->traffic.train[1]=trainID;}
            if(req==PASS&&trainID!=currenttrackNode->traffic.train[0]&&trainID!=currenttrackNode->traffic.train[1])
                ;        //С��ͨ��ʮ��·�ڵ���Է���PASSָ������
            break;
        case 1:
            if(req==ENTER)
            {if(trainID!=currenttrackNode->traffic.train[0]&&trainID!=currenttrackNode->traffic.train[1])  //�����С������Ӧ�����ڶ�η���ENTERָ������
            {currenttrackNode->traffic.status=2;
                fprintf(outputLog,"at %lums trafficNode%d status changes from 1 to 2.\n",RUN_TIME,trackNodeID);
                if(currenttrackNode->traffic.train[0]==-1)
                    currenttrackNode->traffic.train[0]=trainID;
                else if(currenttrackNode->traffic.train[1]==-1)
                    currenttrackNode->traffic.train[1]=trainID;
                firstgo=judge(currenttrackNode->traffic.train[0],currenttrackNode->traffic.train[1]); //���ݵ�ǰ�����жϳ�����������С��˭����
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
            {if(trainID==currenttrackNode->traffic.train[0]||trainID==currenttrackNode->traffic.train[1]) //����С��ͨ��ʮ��·�ڵ���Է���PASSָ������
            {currenttrackNode->traffic.status=0;
                fprintf(outputLog,"at %lums trafficNode%d status changes from 1 to 0.\n",RUN_TIME,trackNodeID);
                if(trainID==currenttrackNode->traffic.train[0])//ɾ��ʮ��·�ڵ��ʾ������С����������ͨ����С��id
                    currenttrackNode->traffic.train[0]=-1;
                else
                    currenttrackNode->traffic.train[1]=-1;}}
            break;
            
        case 2:if(req==ENTER)
        {if(trainID==currenttrackNode->traffic.train[0]||trainID==currenttrackNode->traffic.train[1])       //�����С������Ӧ�����ڶ�η���ENTERָ������
            ;}
        else if((req==PASS)&&(currenttrain->flag==permitted))
        {
            currenttrackNode->traffic.status=1;
            fprintf(outputLog,"at %lums trafficNode%d status changes from 2 to 1.\n",RUN_TIME,trackNodeID);
            if(controlPolicy==AUTO)
                (trainList[trainID]->passTimes)++; //������������ߵ�С����������һ
            if(trainID==currenttrackNode->traffic.train[0]) //ɾ��ʮ��·�ڵ��ʾ������С����������ͨ����С��id
            {currenttrackNode->traffic.train[0]=-1;
                trainList[currenttrackNode->traffic.train[1]]->flag=permitted;}  //��һ��С�𳵵�flag��Ϊpermitted
            else
            {currenttrackNode->traffic.train[1]=-1;
                trainList[currenttrackNode->traffic.train[0]]->flag=permitted;}
            break;
        }
    }
    
}
//�ֲ��״̬��ת������
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
	  	   fprintf(outputLog,"at %lums branchNode%d and its pair status changes from 0 to 1.\n",RUN_TIME,trackNodeID);//����ֲ�ڵ��״̬ת�����
	  	    currenttrain->flag=permitted;
            if(currenttrackNode->branch.train[0]==-1&&currenttrackNode->branch.pair->branch.train[0]==-1)   //��С�𳵼���ֲ��ľ�������
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
                     ;  //����С��ͨ���ֲ����Է���PASSָ������ 
         break;
	  	
	case  1:
	  	if(req==ENTER)   //�����С������Ӧ�����ڶ�η���ENTERָ������ 
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
                   firstgo=judge(currenttrackNode->branch.train[0],currenttrackNode->branch.train[1]);//���ݵ�ǰ�����жϳ�����������С��˭�Ƚ��빫�����
                   firsttrain=trainList[currenttrackNode->branch.train[0]];
				    secondtrain=trainList[currenttrackNode->branch.train[1]];
					if(firstgo==currenttrackNode->branch.train[0])
					  {firsttrain->flag=permitted;
				       secondtrain->flag=forbidden;}
			else
				{secondtrain->flag=permitted;
				firsttrain->flag=forbidden;} }}
		else if(req==PASS&&currenttrain->flag==permitted)   
		     {if(trainID==currenttrackNode->branch.train[0]||trainID==currenttrackNode->branch.train[1])//����С��ͨ���ֲ����Է���PASSָ������
			   {if(checkTrack(trainID, trackNodeID,currenttrackNode->branch.pair->id)==0)  //���ݹ��������æ�Ϳ�������ж�С���Ƿ��ܽ��빫�����
                   currenttrain->flag=permitted;
				 else
				   currenttrain->flag=forbidden;
				 if(currenttrain->flag==permitted)
				  {currenttrackNode->branch.status=0;
                      currenttrackNode->branch.pair->branch.status=0;
				  fprintf(outputLog,"at %lums branchNode%d and its pair status changes from 1 to 0.\n",RUN_TIME,trackNodeID);
                    if(trainID==currenttrackNode->branch.train[0]&&trainID==currenttrackNode->branch.pair->branch.train[0])//ɾ���ֲ�ڵ��ʾ������С����������ͨ����С��id
                    {currenttrackNode->branch.train[0]=-1;
                        currenttrackNode->branch.pair->branch.train[0]=-1;}
                     else if(trainID==currenttrackNode->branch.train[0]&&trainID==currenttrackNode->branch.pair->branch.train[1])//ɾ���ֲ�ڵ��ʾ������С����������ͨ����С��id
                      {currenttrackNode->branch.train[0]=-1;
                          currenttrackNode->branch.pair->branch.train[1]=-1;}
                     else if(trainID==currenttrackNode->branch.train[1]&&trainID==currenttrackNode->branch.pair->branch.train[0])//ɾ���ֲ�ڵ��ʾ������С����������ͨ����С��id
                      {currenttrackNode->branch.train[1]=-1;
                          currenttrackNode->branch.pair->branch.train[0]=-1;}
                      if(trainID==currenttrackNode->branch.train[1]&&trainID==currenttrackNode->branch.pair->branch.train[1])//ɾ���ֲ�ڵ��ʾ������С����������ͨ����С��id
                      {currenttrackNode->branch.train[1]=-1;
                          currenttrackNode->branch.pair->branch.train[1]=-1;}
                  }
               }
             }
        else if (req==PASS&&currenttrain->flag==forbidden) {
            if(checkTrack(trainID, trackNodeID,currenttrackNode->branch.pair->id)==0)  //���ݹ��������æ�Ϳ�������ж�С���Ƿ��ܽ��빫�����
            {currenttrain->flag=permitted;}
            else
                ;}
            
            break;
		
		case 2:
			if(req==ENTER) //�����С������Ӧ�����ڶ�η���ENTERָ������
			{if(trainID==currenttrackNode->branch.train[0]||trainID==currenttrackNode->branch.train[1])
	  	            ;}
		    else if((req==PASS)&&(currenttrain->flag==permitted))
                  { if(checkTrack(trainID, trackNodeID,currenttrackNode->branch.pair->id)==0) //���ݹ��������æ�Ϳ�������ж�С���Ƿ��ܽ��빫�����
				     currenttrain->flag=permitted;
				   else
				   currenttrain->flag=forbidden;
				   if(currenttrain->flag==permitted)
				   {currenttrackNode->branch.status=1;
                       currenttrackNode->branch.pair->branch.status=1;
				   fprintf(outputLog,"at %lums branchNode%d and its pair status changes from 2 to 1.\n",RUN_TIME,trackNodeID);
                      if(controlPolicy==AUTO) 
                      (trainList[trainID]->passTimes)++;//������������ߵ�С����������һ 
                    if(trainID==currenttrackNode->branch.train[0]&&trainID==currenttrackNode->branch.pair->branch.train[0])//ɾ���ֲ�ڵ��ʾ������С����������ͨ����С��id
				     {currenttrackNode->branch.train[0]=-1;
                         currenttrackNode->branch.pair->branch.train[0]=-1;
				    trainList[currenttrackNode->branch.train[1]]->flag=permitted;}//��һ��С�𳵵�flag��Ϊpermitted
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
                if(checkTrack(trainID, trackNodeID,currenttrackNode->branch.pair->id)==0)  //���ݹ��������æ�Ϳ�������ж�С���Ƿ��ܽ��빫�����
                {currenttrain->flag=permitted;}
                else
                    ;
            }
		   break;	}
}
				 
				   
              
		        
			  
				       
	       
	          
      	
        
        
        

