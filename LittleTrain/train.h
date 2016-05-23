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

typedef enum {FREE,RUN,PAUSE,LOCK,STATION}trainState;
typedef int branchState;/*根据接收到的进入公共轨道的请求数，分叉节点状态为012，所以直接用int类型*/
typedef int trafficState;/*根据接收到的通过十字路节点请求数，红绿灯节点状态为012，所以直接用int类型*/
//一些必要的时间变量（假设时间类型是TIME）在calculator里定义
extern clock_t PROGRAM_START;
extern clock_t CURRENT_CIRCLE;
extern clock_t PREVIOUS_CIRCLE;
//flags 在FSM里定义
extern enum {SEQUENCING, BYTHEWAY} servicePolicy;
extern enum {AUTO, MANUAL} controlPolicy;

//通用任务队列
typedef struct _queueNode {
    void * data;
    struct _queueNode * next;
}queueNode;
typedef struct {
    queueNode head;
    queueNode tail;
}queue;
//主任务队列（输入文件中所有的命令）
typedef struct {
    enum {HEAD,STATION,TRAIN,SWITCHMETHOD} type;    //HEAD为头结点
    union{
        struct {
            int id;
            int time;
            int train;
        }station;
        struct {
            enum {clockwise, anticlockwise} direction;
            int speed;
            int id;
        }train;
        struct {
            enum {AUTO, MANUAL} method;
        }switchmethod;
    };
}* mainQueueNode;
//此队列节点中的data均强制转换为mainQueueNode
extern queue mainMission;                      //在main里定义

//小火车任务队列
typedef struct {
    enum {HEAD,STATION,LOCK}type;             //HEAD为头结点
    int station;
    int time;
}* trainQueueNode;

//站点节点、分岔节点、十字路节点
typedef struct _trackNode{
    int id;                                 //id＝－1表示新建立的空节点
    enum{STATION, BRANCH, TRAFFIC}type;
    union{
        struct {
            struct _trackNode * left;
            int ldistance;
            struct _trackNode * right;
            int rdistance;
        } station;
        struct {
            branchState status;
            struct _trackNode * left;
            int ldistance;
            int lrange;
            struct _trackNode * right;
            int rdistance;
            int rrange;
            struct _trackNode * down;//down指向公共轨道里的下一个节点
            int ddistance;
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
        } traffic;
    };
} * trackNode;

//小火车的属性
typedef struct {
    int id;                                 //id＝－1表示新建立的空节点
    int v;//速度，单位是m/s
    enum {clockwise, anticlockwise} direction;
    int lastNode;//上一个节点的编号
    double distance;//距离上一个节点的距离
    enum {permitted, forbidden} flag;
    trainState status;
    int passTimes;
    int serveTimes;
    int nodeList[100];//存储小火车所在轨道经过的所有节点ID
    queue mission;//此队列中的data均强制转换为trainQueueNode
} * train;
//接下来两个数组存储所有的轨道上的节点和小火车，按ID顺序。在main里定义并分配空间
extern trackNode *trackNodeList;//以节点ID为下标
extern train *trainList; //以小火车ID为下标

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
queueNode insertAfter(queue, void * data, void * ptr);
int pop(queue);//从队列中读出一个任务并将其删除。返回0表示成功，－1表示队列为空
train newTrain();
trackNode newTrackNode();

#endif /* train_h */
