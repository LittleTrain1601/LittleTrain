//
//  train.h
//  LittleTrain
//
//  Created by 杨林青 on 16/5/11.
//  Copyright © 2016年 1601. All rights reserved.
//

#ifndef train_h
#define train_h
#include <time.h>

#define MAXITEM 20

typedef enum {FREE,RUN,PAUSE,LOCK,STA}trainState;
typedef int branchState;/*根据接收到的进入公共轨道的请求数，分叉节点状态为012，所以直接用int类型*/
typedef int trafficState;/*根据接收到的通过十字路节点请求数，红绿灯节点状态为012，所以直接用int类型*/
//一些必要的时间变量（假设时间类型是TIME）在calculator里定义
extern clock_t RUN_TIME;//小火车开始运行的时间，忽略用户输入打断的时间
//flags 在FSM里定义
extern enum {SEQUENCING, BYTHEWAY} servicePolicy;
extern enum policy {AUTO, MANUAL} controlPolicy;

//通用任务队列
typedef struct _queueNode {
    void * data;
    struct _queueNode * next;
}* queueNode;
typedef struct _queue{
    queueNode head;
    queueNode tail;
}* queue;
//主任务队列（输入文件中所有的命令）的数据结构体
typedef struct _mainQueueNode{
    enum {MSTATION,MTRAIN,MSWITCHMETHOD,MLOCK} type;    //HEAD为头结点
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
//此队列节点中的data均强制转换为mainQueueNode
extern queue mainMission;                      //在main里定义

//小火车任务队列的数据结构体
typedef struct _trainQueueNode{
    enum {TSTATION,TLOCK}type;             //HEAD为头结点
    int station;
    int time;
}* trainQueueNode;

//站点节点、分岔节点、十字路节点
typedef struct _trackNode{
    int id;                                 //id＝－1表示新建立的空节点
    enum{STATION, BRANCH, TRAFFIC}type;
    union{
        struct {
            clock_t stop;//火车停靠的时刻
            struct _trackNode * left;
            int ldistance;
            struct _trackNode * right;
            int rdistance;
        } station;
        struct {
            branchState status;
            int train[2];//发出请求的火车ID
            int flag;//flag==1则这一段轨道忙 flag==0则这一段轨道空闲
            struct _trackNode * left;
            int ldistance;
            int lrange;
            struct _trackNode * right;
            int rdistance;
            int rrange;
            struct _trackNode * down;//down指向公共轨道里的下一个节点
            int ddistance;
            struct _trackNode * pair;
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

//小火车的属性
typedef struct _train{
    int id;                                 //id＝－1表示新建立的空节点
    int v;//速度，单位是m/s
    enum {clockwise, anticlockwise} direction;
    int nextNode;//下一个节点的编号
    double distance;//距离下一个节点的距离
    enum {permitted, forbidden} flag;
    trainState status;
    int passTimes;
    int nodeList[MAXITEM];//存储小火车所在轨道经过的所有节点ID
    queue mission;//此队列中的data均强制转换为trainQueueNode
} * train;
//接下来两个数组存储所有的轨道上的节点和小火车，按ID顺序。在main里定义并分配空间
extern trackNode trackNodeList[];//以节点ID为下标
extern train trainList[]; //以小火车ID为下标

//input模块
void build();
void input();

//output
void viewer();
void logWriter();

//calculator模块
void updateTrain(int id);
int checkTrack(int branch1, int branch2);
int judge(int train1, int train2);

//FSM
typedef enum {ENTER, PASS} request;
void trainStatusSwitcher(int id);
void branchNodeStatusSwitcher(request req, int trainID, int trackNodeID);
void trafficNodeStatusSwitcher(request req, int trainID, int trackNodeID);

//coreDataAPI
queue newQueue();
queueNode append(queue, void * data);//追加到队列末尾,返回插入的节点的地址
void deleteAfter(queue, queueNode ptr);//删除ptr后面的元素
void * pop(queue);//从队列中读出一个任务并将其删除。返回data指针
train newTrain();
trackNode newTrackNode();

#endif /* train_h */
