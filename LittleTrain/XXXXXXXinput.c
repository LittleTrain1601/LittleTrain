//
//  input.c
//  LittleTrain
//
//  Created by 杨林青 on 16/5/15.
//  Copyright © 2016年 1601. All rights reserved.
//

#include <stdio.h>
#include "train.h"
#include <stdlib.h>

//尺度说明：＃－－＃表示两个站台相距3m
//小火车默认顺时针运行。
//坐标以左上角为原点

//ID与坐标映射数组。记录火车初始坐标和各个节点坐标
int global_id = 0;
int x[MAXITEM];
int y[MAXITEM];
char buff[24][80];
int idInPosition[24][80];

//搜索节点的相邻节点。不递归
//添加都队列里的数据是节点后的一个字符，有方向。在循环里一直找到下一节点再加入队列。
int visited[24][80]={0};                    //探索过的标记为1
int branchNum(int id) {
    int sum=0;
    if (buff[y[id]+1][x[id]-1] != '.' ||
        buff[y[id]+1][x[id]] != '.' ||
        buff[y[id]+1][x[id]+1] != '.' ||
        buff[y[id]][x[id]-1] != '.' ||
        buff[y[id]][x[id]+1] != '.' ||
        buff[y[id]-1][x[id]-1] != '.' ||
        buff[y[id]-1][x[id]] != '.' ||
        buff[y[id]-1][x[id]+1] != '.' ) {
        sum++;
    }
    return sum;
} 
typedef struct _exploreData{
    int previousID;
    enum {L, R, U, D} previousBranch;
    int distence;//探索时以一格（一米）为单位
    enum {RIGHT, LEFT, UP, DOWN, UPRIGHT, UPLEFT, DOWNRIGHT, DOWNLEFT, BR} status,income;
    //这个数组保存某一点的前方对应的三点坐标信息。比如（0，1）前方是（1，0）（1，1）（1，2）.下标为0的是当前坐标。
    int x[4],y[4];
}* exploreData;
queue exploreQueue;
void search(int id) {
    exploreQueue = newQueue();
    trackNode previousTrackNode;
    trackNode currentTrackNode;
    
    
    exploreData current = (exploreData)pop(exploreQueue);
    while (current != NULL) {
        switch (current->status) {
            case RIGHT:
                current->x[1] = current->x[0]+1;
                current->y[1] = current->y[0]+1;
                current->x[2] = current->x[0]+1;
                current->y[2] = current->y[0];
                current->x[3] = current->x[0]+1;
                current->y[3] = current->y[0]-1;
                if (buff[y[1]][x[1]] == '/' || buff[y[1]][x[1]] == '*' || buff[y[1]][x[1]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->status = UPRIGHT;
                } else if (buff[y[1]][x[1]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[2]][x[2]] == '-' || buff[y[2]][x[2]] == '*' || buff[y[2]][x[2]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->status = RIGHT;
                }else if (buff[y[2]][x[2]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[3]][x[3]] == '\\' || buff[y[3]][x[3]] == '*' || buff[y[3]][x[3]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->status = DOWNRIGHT;
                }else if (buff[y[3]][x[3]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->income = current->status;
                    current->status = BR;
                }
                break;
            case LEFT:
                current->x[1] = current->x[0]-1;
                current->y[1] = current->y[0]+1;
                current->x[2] = current->x[0]-1;
                current->y[2] = current->y[0];
                current->x[3] = current->x[0]-1;
                current->y[3] = current->y[0]-1;
                if (buff[y[1]][x[1]] == '\\' || buff[y[1]][x[1]] == '*' || buff[y[1]][x[1]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->status = UPLEFT;
                } else if (buff[y[1]][x[1]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[2]][x[2]] == '-' || buff[y[2]][x[2]] == '*' || buff[y[2]][x[2]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->status = LEFT;
                }else if (buff[y[2]][x[2]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[3]][x[3]] == '/' || buff[y[3]][x[3]] == '*' || buff[y[3]][x[3]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->status = DOWNLEFT;
                }else if (buff[y[3]][x[3]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->income = current->status;
                    current->status = BR;
                }
                break;
            case UP:
                current->x[1] = current->x[0]-1;
                current->y[1] = current->y[0]+1;
                current->x[2] = current->x[0];
                current->y[2] = current->y[0]+1;
                current->x[3] = current->x[0]+1;
                current->y[3] = current->y[0]+1;
                if (buff[y[1]][x[1]] == '\\' || buff[y[1]][x[1]] == '*' || buff[y[1]][x[1]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->status = UPLEFT;
                } else if (buff[y[1]][x[1]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[2]][x[2]] == '|' || buff[y[2]][x[2]] == '*' || buff[y[2]][x[2]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->status = UP;
                }else if (buff[y[2]][x[2]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[3]][x[3]] == '/' || buff[y[3]][x[3]] == '*' || buff[y[3]][x[3]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->status = UPRIGHT;
                }else if (buff[y[3]][x[3]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->income = current->status;
                    current->status = BR;
                }
                break;
            case DOWN:
                current->x[1] = current->x[0]-1;
                current->y[1] = current->y[0]-1;
                current->x[2] = current->x[0];
                current->y[2] = current->y[0]-1;
                current->x[3] = current->x[0]+1;
                current->y[3] = current->y[0]-1;
                if (buff[y[1]][x[1]] == '/' || buff[y[1]][x[1]] == '*' || buff[y[1]][x[1]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->status = DOWNLEFT;
                } else if (buff[y[1]][x[1]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[2]][x[2]] == '|' || buff[y[2]][x[2]] == '*' || buff[y[2]][x[2]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->status = DOWN;
                }else if (buff[y[2]][x[2]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[3]][x[3]] == '\\' || buff[y[3]][x[3]] == '*' || buff[y[3]][x[3]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->status = DOWNRIGHT;
                }else if (buff[y[3]][x[3]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->income = current->status;
                    current->status = BR;
                }
                break;
            case UPRIGHT:
                current->x[1] = current->x[0];
                current->y[1] = current->y[0]+1;
                current->x[2] = current->x[0]+1;
                current->y[2] = current->y[0]+1;
                current->x[3] = current->x[0]+1;
                current->y[3] = current->y[0];
                if (buff[y[1]][x[1]] == '|' || buff[y[1]][x[1]] == '*' || buff[y[1]][x[1]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->status = UP;
                } else if (buff[y[1]][x[1]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[2]][x[2]] == '/' || buff[y[2]][x[2]] == '*' || buff[y[2]][x[2]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->status = UPRIGHT;
                }else if (buff[y[2]][x[2]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[3]][x[3]] == '-' || buff[y[3]][x[3]] == '*' || buff[y[3]][x[3]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->status = RIGHT;
                }else if (buff[y[3]][x[3]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->income = current->status;
                    current->status = BR;
                }
                break;
            case UPLEFT:
                current->x[1] = current->x[0];
                current->y[1] = current->y[0]+1;
                current->x[2] = current->x[0]-1;
                current->y[2] = current->y[0]+1;
                current->x[3] = current->x[0]-1;
                current->y[3] = current->y[0];
                if (buff[y[1]][x[1]] == '|' || buff[y[1]][x[1]] == '*' || buff[y[1]][x[1]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->status = UP;
                } else if (buff[y[1]][x[1]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[2]][x[2]] == '\\' || buff[y[2]][x[2]] == '*' || buff[y[2]][x[2]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->status = UPLEFT;
                }else if (buff[y[2]][x[2]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[3]][x[3]] == '-' || buff[y[3]][x[3]] == '*' || buff[y[3]][x[3]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->status = LEFT;
                }else if (buff[y[3]][x[3]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->income = current->status;
                    current->status = BR;
                }
                break;
            case DOWNRIGHT:
                current->x[1] = current->x[0]+1;
                current->y[1] = current->y[0];
                current->x[2] = current->x[0]+1;
                current->y[2] = current->y[0]-1;
                current->x[3] = current->x[0];
                current->y[3] = current->y[0]-1;
                if (buff[y[1]][x[1]] == '-' || buff[y[1]][x[1]] == '*' || buff[y[1]][x[1]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->status = RIGHT;
                } else if (buff[y[1]][x[1]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[2]][x[2]] == '\\' || buff[y[2]][x[2]] == '*' || buff[y[2]][x[2]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->status = DOWNRIGHT;
                }else if (buff[y[2]][x[2]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[3]][x[3]] == '|' || buff[y[3]][x[3]] == '*' || buff[y[3]][x[3]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->status = DOWN;
                }else if (buff[y[3]][x[3]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->income = current->status;
                    current->status = BR;
                }
                break;
            case DOWNLEFT:
                current->x[1] = current->x[0]-1;
                current->y[1] = current->y[0];
                current->x[2] = current->x[0]-1;
                current->y[2] = current->y[0]-1;
                current->x[3] = current->x[0];
                current->y[3] = current->y[0]-1;
                if (buff[y[1]][x[1]] == '-' || buff[y[1]][x[1]] == '*' || buff[y[1]][x[1]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->status = LEFT;
                } else if (buff[y[1]][x[1]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[1];
                    current->y[0] = current->y[1];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[2]][x[2]] == '/' || buff[y[2]][x[2]] == '*' || buff[y[2]][x[2]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->status = DOWNLEFT;
                }else if (buff[y[2]][x[2]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[2];
                    current->y[0] = current->y[2];
                    current->income = current->status;
                    current->status = BR;
                }else if (buff[y[3]][x[3]] == '|' || buff[y[3]][x[3]] == '*' || buff[y[3]][x[3]] == '&') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->status = DOWN;
                }else if (buff[y[3]][x[3]] == '@') {
                    current->distence++;
                    visited[y[0]][x[0]] = 1;
                    current->x[0] = current->x[3];
                    current->y[0] = current->y[3];
                    current->income = current->status;
                    current->status = BR;
                }
                break;
            case BR:
                previousTrackNode = trackNodeList[current->previousID];
                currentTrackNode = trackNodeList[idInPosition[current->y[0]][current->x[0]]];
                if (previousTrackNode->type == STATION) {
                    if (current->previousBranch == R) {
                        previousTrackNode->station.right = currentTrackNode;
                        previousTrackNode->station.rdistance = current->distence;
                    } else if (current->previousBranch == L) {
                        previousTrackNode->station.left = currentTrackNode;
                        previousTrackNode->station.ldistance = current->distence;
                    }
                } else if (previousTrackNode->type == BRANCH) {
                    if (current->previousBranch == R) {
                        previousTrackNode->branch.right = currentTrackNode;
                        previousTrackNode->branch.rdistance = current->distence;
                    } else if (current->previousBranch == L) {
                        previousTrackNode->branch.left = currentTrackNode;
                        previousTrackNode->branch.ldistance = current->distence;
                    } else if (current->previousBranch == D) {
                        previousTrackNode->branch.down = currentTrackNode;
                        previousTrackNode->branch.ddistance = current->distence;
                    }
                } else if (previousTrackNode->type == TRAFFIC) {
                    if (current->previousBranch == R) {
                        previousTrackNode->traffic.right = currentTrackNode;
                        previousTrackNode->traffic.rdistance = current->distence;
                    } else if (current->previousBranch == L) {
                        previousTrackNode->traffic.left = currentTrackNode;
                        previousTrackNode->traffic.ldistance = current->distence;
                    } else if (current->previousBranch == U) {
                        previousTrackNode->traffic.up = currentTrackNode;
                        previousTrackNode->traffic.udistance = current->distence;
                    } else if (current->previousBranch == D) {
                        previousTrackNode->traffic.down = currentTrackNode;
                        previousTrackNode->traffic.ddistance = current->distence;
                    }
                }
                switch (branchNum(idInPosition[current->y[0]][current->x[0]])) {
                    case 2:
                        currentTrackNode->type = STATION;
                        switch (current->income) {
                            case RIGHT:
                                currentTrackNode->station.left = previousTrackNode;
                                currentTrackNode->station.ldistance = current->distence;
                                if (!visited[current->y[0]][current->x[0]+1]) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousBranch = R;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->status = RIGHT;
                                    newExploreData->x[0] = current->x[0]+1;
                                    newExploreData->y[0] = current->y[0];
                                    append(exploreQueue, newExploreData);
                                }
                                current = (exploreData)pop(exploreQueue);
                                break;
                            case LEFT:
                                currentTrackNode->station.right = previousTrackNode;
                                currentTrackNode->station.rdistance = current->distence;
                                if (!visited[current->y[0]][current->x[0]-1]) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousBranch = L;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->status = LEFT;
                                    newExploreData->x[0] = current->x[0]-1;
                                    newExploreData->y[0] = current->y[0];
                                    append(exploreQueue, newExploreData);
                                }
                                current = (exploreData)pop(exploreQueue);
                                break;
                            case UP:
                                currentTrackNode->station.right = previousTrackNode;
                                currentTrackNode->station.rdistance = current->distence;
                                if (!visited[current->y[0]-1][current->x[0]]) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousBranch = L;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->status = UP;
                                    newExploreData->x[0] = current->x[0];
                                    newExploreData->y[0] = current->y[0]-1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = (exploreData)pop(exploreQueue);
                                break;
                            case DOWN:
                                currentTrackNode->station.left = previousTrackNode;
                                currentTrackNode->station.ldistance = current->distence;
                                if (!visited[current->y[0]-1][current->x[0]]) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousBranch = R;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->status = RIGHT;
                                    newExploreData->x[0] = current->x[0];
                                    newExploreData->y[0] = current->y[0]-1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = (exploreData)pop(exploreQueue);
                                break;
                            case UPRIGHT:
                                currentTrackNode->station.left = previousTrackNode;
                                currentTrackNode->station.ldistance = current->distence;
                                if (!visited[current->y[0]+1][current->x[0]+1]) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousBranch = R;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->status = UPRIGHT;
                                    newExploreData->x[0] = current->x[0]+1;
                                    newExploreData->y[0] = current->y[0]+1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = (exploreData)pop(exploreQueue);
                                break;
                            case DOWNRIGHT:
                                currentTrackNode->station.left = previousTrackNode;
                                currentTrackNode->station.ldistance = current->distence;
                                if (!visited[current->y[0]-1][current->x[0]+1]) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousBranch = R;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->status = DOWNRIGHT;
                                    newExploreData->x[0] = current->x[0]+1;
                                    newExploreData->y[0] = current->y[0]-1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = (exploreData)pop(exploreQueue);
                                break;
                            case UPLEFT:
                                currentTrackNode->station.right = previousTrackNode;
                                currentTrackNode->station.rdistance = current->distence;
                                if (!visited[current->y[0]+1][current->x[0]-1]) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousBranch = L;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->status = UPLEFT;
                                    newExploreData->x[0] = current->x[0]-1;
                                    newExploreData->y[0] = current->y[0]+1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = (exploreData)pop(exploreQueue);
                                break;
                            case DOWNLEFT:
                                currentTrackNode->station.right = previousTrackNode;
                                currentTrackNode->station.rdistance = current->distence;
                                if (!visited[current->y[0]-1][current->x[0]-1]) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousBranch = L;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->status = DOWNLEFT;
                                    newExploreData->x[0] = current->x[0]-1;
                                    newExploreData->y[0] = current->y[0]-1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = (exploreData)pop(exploreQueue);
                                break;
                            default:
                                break;
                        }
                        break;
                    case 4:
                        currentTrackNode->type = TRAFFIC;
                        switch (current->income) {
                            case RIGHT:
                                currentTrackNode->traffic.left = previousTrackNode;
                                if (!visited[current->y[0]][current->x[0]]+1) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->previousBranch = R;
                                    newExploreData->status = RIGHT;
                                    newExploreData->y[0] = current->y[0];
                                    newExploreData->x[0] = current->x[0]+1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = pop(exploreQueue);
                                break;
                            case DOWN:
                                currentTrackNode->traffic.up = previousTrackNode;
                                if (!visited[current->y[0]][current->x[0]]+1) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->previousBranch = R;
                                    newExploreData->status = RIGHT;
                                    newExploreData->y[0] = current->y[0];
                                    newExploreData->x[0] = current->x[0]+1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = pop(exploreQueue);
                                break;
                            case RIGHT:
                                currentTrackNode->traffic.left = previousTrackNode;
                                if (!visited[current->y[0]][current->x[0]]+1) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->previousBranch = R;
                                    newExploreData->status = RIGHT;
                                    newExploreData->y[0] = current->y[0];
                                    newExploreData->x[0] = current->x[0]+1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = pop(exploreQueue);
                                break;
                            case RIGHT:
                                currentTrackNode->traffic.left = previousTrackNode;
                                if (!visited[current->y[0]][current->x[0]]+1) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->previousBranch = R;
                                    newExploreData->status = RIGHT;
                                    newExploreData->y[0] = current->y[0];
                                    newExploreData->x[0] = current->x[0]+1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = pop(exploreQueue);
                                break;
                            case RIGHT:
                                currentTrackNode->traffic.left = previousTrackNode;
                                if (!visited[current->y[0]][current->x[0]]+1) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->previousBranch = R;
                                    newExploreData->status = RIGHT;
                                    newExploreData->y[0] = current->y[0];
                                    newExploreData->x[0] = current->x[0]+1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = pop(exploreQueue);
                                break;
                            case RIGHT:
                                currentTrackNode->traffic.left = previousTrackNode;
                                if (!visited[current->y[0]][current->x[0]]+1) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->previousBranch = R;
                                    newExploreData->status = RIGHT;
                                    newExploreData->y[0] = current->y[0];
                                    newExploreData->x[0] = current->x[0]+1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = pop(exploreQueue);
                                break;
                            case RIGHT:
                                currentTrackNode->traffic.left = previousTrackNode;
                                if (!visited[current->y[0]][current->x[0]]+1) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->previousBranch = R;
                                    newExploreData->status = RIGHT;
                                    newExploreData->y[0] = current->y[0];
                                    newExploreData->x[0] = current->x[0]+1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = pop(exploreQueue);
                                break;
                            case RIGHT:
                                currentTrackNode->traffic.left = previousTrackNode;
                                if (!visited[current->y[0]][current->x[0]]+1) {
                                    exploreData newExploreData = malloc(sizeof(struct _exploreData));
                                    newExploreData->distence = 1;
                                    newExploreData->previousID = idInPosition[current->y[0]][current->x[0]];
                                    newExploreData->previousBranch = R;
                                    newExploreData->status = RIGHT;
                                    newExploreData->y[0] = current->y[0];
                                    newExploreData->x[0] = current->x[0]+1;
                                    append(exploreQueue, newExploreData);
                                }
                                current = pop(exploreQueue);
                                break;
                                
                            default:
                                break;
                        }
                        break;
                    case 3:
                        
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void build() {
    //初始化trainList和trackNodeList
    for (int i=0; i<MAXITEM; i++) {
        trainList[i] = NULL;
        trackNodeList[i] = NULL;
    }
    FILE *fp = fopen("track.txt", "r");

    for (int i=0; i<24; i++) {
        for (int j=0; j<80; j++) {
            buff[i][j] = fgetc(fp);
            switch (buff[i][j]) {
                case '&':
                    x[global_id] = j;
                    y[global_id] = i;
                    trainList[global_id] = newTrain();
                    idInPosition[i][j] = global_id;
                    global_id++;
                    break;
                case '#':
                    x[global_id] = j;
                    y[global_id] = i;
                    trackNodeList[global_id] = newTrackNode();
                    idInPosition[i][j] = global_id;
                    global_id++;
                    break;
                case '@':
                    x[global_id] = j;
                    y[global_id] = i;
                    trackNodeList[global_id] = newTrackNode();
                    idInPosition[i][j] = global_id;
                    global_id++;
                default:
                    break;
            }
        }
        fgetc(fp);
    }
    
    for (int i=0; i<MAXITEM; i++) {
        if (trackNodeList[i]) {
            search(i);
        }
    }
}