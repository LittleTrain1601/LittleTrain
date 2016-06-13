//
//  train.h
//  LittleTrain
//
//  Created by ������ on 16/5/11.
//  Copyright ? 2016�� 1601. All rights reserved.
//

#ifndef train_h
#define train_h
#include <time.h>
#include <stdio.h>

#define MAXITEM 50 //������ 

typedef enum {FREE,RUN,PAUSE,LOCK,STA}trainState;
typedef int branchState;/*���ݽ��յ��Ľ��빫����������������ֲ�ڵ�״̬Ϊ012������ֱ����int����*/
typedef int trafficState;/*���ݽ��յ���ͨ��ʮ��·�ڵ������������̵ƽڵ�״̬Ϊ012������ֱ����int����*/
//һЩ��Ҫ��ʱ�����������ʱ��������TIME����calculator�ﶨ��
extern clock_t RUN_TIME;//С�𳵿�ʼ���е�ʱ�䣬�����û������ϵ�ʱ��
//flags ��FSM�ﶨ��
extern enum _servicePolicy{SEQUENCING, BYTHEWAY} servicePolicy;
extern enum policy {AUTO, MANUAL} controlPolicy;

//ͨ���������
typedef struct _queueNode {
    void * data;
    struct _queueNode * next;
}* queueNode;
typedef struct _queue{
    queueNode head;
    queueNode tail;
}* queue;
//��������У������ļ������е���������ݽṹ��
typedef struct _mainQueueNode{
    enum {MSTATION,MTRAIN,MLOCK} type;    //HEADΪͷ���
    union{
        struct {
            int id;
            int time;
            int train;
        }station;
        struct {
            enum {MCLOCLWISE, MANTICLOCKWISE} direction;
            int speed;
            int id;
        }train;
    };
}* mainQueueNode;
//�˶��нڵ��е�data��ǿ��ת��ΪmainQueueNode
extern queue mainMission;                      //��main�ﶨ��

//С��������е����ݽṹ��
typedef struct _trainQueueNode{
    enum {TSTATION,TLOCK}type;             //HEADΪͷ���
    int station;
    int time;
}* trainQueueNode;

//վ��ڵ㡢�ֲ�ڵ㡢ʮ��·�ڵ�
typedef struct _trackNode{
    int id;                                 //id����1��ʾ�½����Ŀսڵ�
    enum{STATION, BRANCH, TRAFFIC}type;
    union{
        struct {
            clock_t stop;//��ͣ����ʱ��
            struct _trackNode * left;
            int ldistance;
            struct _trackNode * right;
            int rdistance;
            int x;     //ͼ�λ�����վ��ķ�Χ 
			int y;
			int width;
			int height; 
			} station;
        struct {
            branchState status;
            int train[2];//��������Ļ�ID
            int flag;//flag==1����һ�ι��æ flag==0����һ�ι������
            struct _trackNode * left;
            int ldistance;
            int lrange;
            struct _trackNode * right;
            int rdistance;
            int rrange;
            struct _trackNode * down;//downָ�򹫹���������һ���ڵ�
            int ddistance;
            struct _trackNode * pair;
            int x;   //���������Χ��ע��ĳ�ֲ�ڵ������pair���ĸ���ͬ����ʼ�� 
			int y;
			int width;
			int height; 
        } branch;
        struct {
            struct _trackNode * up;
            int udistance;
            int urange;
            struct _trackNode * down;
            int ddistance;
            int drange;
            struct _trackNode * left;
            int ldistance;
            int lrange;
            struct _trackNode * right;
            int rdistance;
            int rrange;
            trafficState status;
            int train[2];
        } traffic;
    };
} * trackNode;

//С�𳵵�����
typedef struct _train{
    int id;                                 //id����1��ʾ�½����Ŀսڵ�
    int v;//�ٶȣ���λ��m/s
    enum {clockwise, anticlockwise} direction;
    int nextNode;//��һ���ڵ�ı��
    double distance;//������һ���ڵ�ľ���
    enum {permitted, forbidden} flag;
    trainState status;
    int passTimes;
    int nodeList[MAXITEM];//�洢С�����ڹ�����������нڵ�ID
    queue mission;//�˶����е�data��ǿ��ת��ΪtrainQueueNode
    int x1, y1, x2, y2;//͵͵������¼���������
} * train;
//��������������洢���еĹ���ϵĽڵ��С�𳵣���ID˳����main�ﶨ�岢����ռ�
extern trackNode trackNodeList[];//�Խڵ�IDΪ�±�
extern train trainList[]; //��С��IDΪ�±�

//inputģ��
void build();
void input();

//output
extern FILE *outputLog;
void viewer();
void logWriter();

//calculatorģ��
void updateTrain(int id);
int checkTrack(int trainID, int branch1, int branch2);
int judge(int train1, int train2);

//FSM
typedef enum {ENTER, PASS} request;
void trainStatusSwitcher(int id);
void branchNodeStatusSwitcher(request req, int trainID, int trackNodeID);
void trafficNodeStatusSwitcher(request req, int trainID, int trackNodeID);

//coreDataAPI
queue newQueue();
queueNode append(queue, void * data);//׷�ӵ�����ĩβ,���ز���Ľڵ�ĵ�ַ
void deleteAfter(queue, queueNode ptr);//ɾ��ptr�����Ԫ��
void * pop(queue);//�Ӷ����ж���һ�����񲢽���ɾ��������dataָ��
train newTrain();
trackNode newTrackNode();

//ͼ�λ�
char currentmode=T   ;  //ָʾ�Ҳ�ģ����:TΪ��ģ�飬SΪվ��ģ�飬PΪ�������ģ�� 
int trainnumber=0; //���Ϊ0,1,2��������
int totaltrain=3;//������ 
int stationnumber;//��Ŵ�3��ʼ��վ��
int  totalstation;//վ������
int branchnumber;// ��Ŵ�3+totalstation��ʼ
int tochoose=0;  //�����˹�ѡ�񴰿�Ϊ1��δ����Ϊ0

#endif /* train_h */
