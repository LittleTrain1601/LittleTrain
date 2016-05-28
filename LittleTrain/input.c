//
//  input.c
//  LittleTrain
//
//  Created by 杨林青 on 16/5/27.
//  Copyright © 2016年 1601. All rights reserved.
//

#include "train.h"
#include <stdio.h>

//尺度说明：＃－－＃表示两个站台相距3m
//小火车默认顺时针运行。
//坐标以左上角为原点

//ID与坐标映射数组。记录火车初始坐标和各个节点坐标
int trainNumber;
int global_id = 0;
int x[MAXITEM];
int y[MAXITEM];
char buff[24][80]={0};
int idInPosition[24][80];
int trainChar = '&';
int trackChar = '#';
int branchChar = 'r';
int stationChar = 'P';
int trafficChar = '+';


void printTrack() {
    for (int i=0; i<24; i++) {
        for (int j=0; j<80; j++) {
            printf("%c", buff[i][j]);
        }
        printf("\n");
    }
}


int absDistence(int id1, int x1, int y1, int x2, int y2) {
    //矩形左上角到某一点的距离
    if (y[id1] == y1) {
        return x2 - x[id1];
    } else if (x[id1] == x2) {
        return x2-x1+y[id1]-y1;
    } else if (y[id1] == y2) {
        return x2-x[id1]+x2-x1+y2-y1;
    } else {
        return y2-y[id1]+x2-x1+y2-y1+x2-x1;
    }
}
int getDistence(int id1, int id2, int x1, int y1, int x2, int y2) {
    //以顺时针的顺序计算id1到id2两个节点间的距离
    return absDistence(id2, x1, y1, x2, y2) - absDistence(id1, x1, y1, x2, y2);
}

enum branchStat{L, R, U, D, N};
branchState getFreeBranch(trackNode node){
    switch (node->type) {
        case STATION:
            if (node->station.right == NULL) {
                return R;
            } else if (node->station.left == NULL) {
                return L;
            } else return N;
            break;
        case BRANCH:
            if (node->branch.down) {
                if (node->branch.left == NULL) {
                    return L;
                } else if (node->branch.right == NULL) {
                    return R;
                } else {
                    return N;
                }
            } else {
                if (node->branch.left || node->branch.right) {
                    return D;
                } else {
                    return L;
                }
            }
            break;
        case TRAFFIC:
            if (node->traffic.left && (!node->traffic.right)) {
                return R;
            } else if (node->traffic.right && (!node->traffic.left)) {
                return L;
            } else if (node->traffic.up && (!node->traffic.down)) {
                return D;
            } else if (node->traffic.down && (!node->traffic.up)) {
                return U;
            } else return N;
            break;
        default:
            return N;
            break;
    }
}
void connectPreviousBranch(trackNode previousNode, trackNode currentNode) {
    switch (getFreeBranch(previousNode)) {
        case L:
            switch (previousNode->type) {
                case STATION:
                    previousNode->station.left = currentNode;
                    previousNode->station.ldistance = currentNode->station.ldistance;
                    break;
                case BRANCH:
                    previousNode->branch.left = currentNode;
                    previousNode->branch.ldistance = currentNode->station.ldistance;
                    break;
                case TRAFFIC:
                    previousNode->traffic.left = currentNode;
                    previousNode->traffic.ldistance = currentNode->station.ldistance;
                    break;
                default:
                    break;
            }
            break;
        case R:
            switch (previousNode->type) {
                case STATION:
                    previousNode->station.right = currentNode;
                    previousNode->station.rdistance = currentNode->station.ldistance;
                    break;
                case BRANCH:
                    previousNode->branch.right = currentNode;
                    previousNode->branch.rdistance = currentNode->station.ldistance;
                    break;
                case TRAFFIC:
                    previousNode->traffic.right = currentNode;
                    previousNode->traffic.rdistance = currentNode->station.ldistance;
                    break;
                default:
                    break;
            }
            break;
        case U:
            previousNode->traffic.up = currentNode;
            previousNode->traffic.udistance = currentNode->station.ldistance;
            break;
        case D:
            switch (previousNode->type) {
                case BRANCH:
                    previousNode->branch.down = currentNode;
                    previousNode->branch.ddistance = currentNode->station.ldistance;
                    break;
                case TRAFFIC:
                    previousNode->traffic.down = currentNode;
                    previousNode->traffic.ddistance = currentNode->station.ldistance;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void build() {
    //初始化trainList和trackNodeList
    for (int i=0; i<MAXITEM; i++) {
        trainList[i] = NULL;
        trackNodeList[i] = NULL;
    }
    for (int i=0; i<24; i++) {
        for (int j=0; j<80; j++) {
            idInPosition[j][i]=-1;
        }
    }
    FILE *fp = fopen("track.txt", "r");
    FILE *conf = fopen("configure.dat", "w");
    int x1,x2,y1,y2;
    global_id = trainNumber;
    if (!fp) {
        fp = stdin;
    }
    printf("=============1601 小火车===========\n");
    printf("=============轨道配置向导===========\n");
    printf("每辆小火车有且仅有一条自己的轨道。请输入小火车的数量：");
    fscanf(fp, "%d", &trainNumber);
    fprintf(conf, "%d\n", trainNumber);
    int nodeNumber;
    char nodeType;
    int inputId;
    int inputX, inputY;
    trackNode currentNode, previousNode;
    
    for (int i=0; i<trainNumber; i++) {
        trainList[i] = newTrain();
        int trainNodeListIndex=0;
        printf("请指定第%d辆小火车的轨道位置(左上角坐标和右下角坐标):\n注意，坐标原点在屏幕左上角。\n", i);
        fscanf(fp, "%d%d%d%d", &x1, &y1, &x2, &y2);
        fprintf(conf, "%d %d %d %d\n", x1, y1, x2, y2);
        for (int j=x1; j<=x2; j++) {
            buff[y1][j] = trackChar;
            buff[y2][j] = trackChar;
        }
        for (int j=y1; j<=y2; j++) {
            buff[j][x1] = trackChar;
            buff[j][x2] = trackChar;
        }
        printTrack();
        printf("请输入轨道上站点数量、分岔点数量以及十字交叉点的数量和：");
        fscanf(fp, "%d", &nodeNumber);
        fprintf(conf, "%d\n", nodeNumber);
        printf("节点编号排在火车编号后面。请按顺时针的顺序输入节点的类型和坐标。务必从非公共轨道上的站点开始。站点为S，分叉点为B，十字交叉点为T：\n");
        printf("请输入第%d个节点的类型和坐标：", global_id);
        fscanf(fp, "%c %d %d", &nodeType, &inputX, &inputY);
        fprintf(conf, "%c %d %d\n", nodeType, inputX, inputY);
        buff[inputY][inputX] = stationChar;
        idInPosition[inputY][inputX] = global_id;
        x[global_id] = inputX;
        y[global_id] = inputY;
        currentNode = newTrackNode();
        trackNodeList[global_id] = currentNode;
        trackNodeList[global_id+1] = newTrackNode();
        currentNode->type = STATION;
        currentNode->id = global_id;
        trainList[i]->nodeList[trainNodeListIndex++] = global_id;
        previousNode = currentNode;
        global_id++;
        for (int j=1; j<nodeNumber; j++) {
            printf("请输入第%d个节点的类型和坐标：", global_id);
            fscanf(fp, "%c %d %d", &nodeType, &inputX, &inputY);
            fprintf(conf, "%c %d %d\n", nodeType, inputX, inputY);
            if (idInPosition[inputY][inputX] != -1) {
                global_id--;
            }
            currentNode = trackNodeList[global_id];
            if (currentNode == NULL) {
                currentNode = newTrackNode();
                trackNodeList[global_id] = currentNode;
            }
            idInPosition[inputY][inputX] = global_id;
            x[global_id] = inputX;
            y[global_id] = inputY;
            switch (nodeType) {
                case 'S':
                    if (idInPosition[inputY][inputX]!=-1) {
                        break;
                    }
                    buff[inputY][inputX] = stationChar;
                    currentNode->type = STATION;
                    currentNode->id = global_id;
                    currentNode->station.left = trackNodeList[global_id-1];
                    currentNode->station.ldistance = getDistence(global_id-1, global_id, x1, y1, x2, y2);
                    connectPreviousBranch(previousNode, currentNode);
                    break;
                case 'B':
                    if (idInPosition[inputY][inputX] != -1) {
                        currentNode = trackNodeList[idInPosition[inputY][inputX]];
                        switch (getFreeBranch(currentNode)) {
                            case L:
                                currentNode->branch.left = previousNode;
                                currentNode->branch.ldistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
                                break;
                            case R:
                                currentNode->branch.right = previousNode;
                                currentNode->branch.rdistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
                                break;
                            default:
                                break;
                        }
                        connectPreviousBranch(previousNode, currentNode);
                        break;
                    }
                    buff[inputY][inputX] = branchChar;
                    currentNode->type = BRANCH;
                    currentNode->id = global_id;
                    printf("请输入这对分叉点中，另一个分叉点的ID,顺时针是进入(I)公共轨道还是出(O)公共轨道,以及到监测点的距离:");
                    char branchIO;
                    int range;
                    fscanf(fp, "%d %c %d\n", &inputId, &branchIO, &range);
                    fprintf(conf, "%d %c %d\n", inputId, branchIO, range);
                    if (trackNodeList[inputId] == NULL) {
                        trackNodeList[inputId] = newTrackNode();
                    }
                    currentNode->branch.pair = trackNodeList[inputId];
                    switch (branchIO) {
                        case 'I':
                            if (!currentNode->branch.left) {
                                currentNode->branch.left = previousNode;
                                currentNode->branch.ldistance = getDistence(global_id-1, global_id, x1, y1, x2, y2);
                            } else {
                                currentNode->branch.right = previousNode;
                                currentNode->branch.rdistance = getDistence(global_id-1, global_id, x1, y1, x2, y2);
                            }
                            break;
                        case 'O':
                            currentNode->branch.down = previousNode;
                            currentNode->branch.ddistance = getDistence(global_id-1, global_id, x1, y1, x2, y2);
                            break;
                        default:
                            break;
                    }
                    currentNode->branch.lrange = range;
                    currentNode->branch.rrange = range;
                    connectPreviousBranch(previousNode, currentNode);
                    break;
                case 'T':
                    buff[inputY][inputX] = trafficChar;
                    currentNode->type = TRAFFIC;
                    currentNode->id = global_id;
                    if (inputX == x1 || inputX == x2) {
                        currentNode->traffic.up = previousNode;
                        currentNode->traffic.udistance = getDistence(global_id-1, global_id, x1, y1, x2, y2);
                    } else {
                        currentNode->traffic.left = previousNode;
                        currentNode->traffic.ldistance = getDistence(global_id-1, global_id, x1, y1, x2, y2);
                    }
                    connectPreviousBranch(previousNode, currentNode);
                    break;
                default:
                    break;
            }
            trainList[i]->nodeList[trainNodeListIndex++] = global_id;
            global_id++;
            previousNode = currentNode;
        }
        currentNode = trackNodeList[trainList[i]->nodeList[trainNodeListIndex]];
        currentNode->station.left = previousNode;
        currentNode->station.ldistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
        connectPreviousBranch(previousNode, currentNode);
    }
}