//
//  input.c
//  LittleTrain
//
//  Created by ������ on 16/5/27.
//  Copyright ? 2016�� 1601. All rights reserved.
//

#include "train.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//�߶�˵��������������ʾ����վ̨���3m
//С��Ĭ��˳ʱ�����С�
//���������Ͻ�Ϊԭ��

//ID������ӳ�����顣��¼�𳵳�ʼ����͸����ڵ�����
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
    //�������Ͻǵ�ĳһ��ľ���
    if (y[id1] == y1) {
        return x[id1] - x1;
    } else if (x[id1] == x2) {
        return x2-x1+y[id1]-y1;
    } else if (y[id1] == y2) {
        return x2-x[id1]+x2-x1+y2-y1;
    } else {
        return y2-y[id1]+x2-x1+y2-y1+x2-x1;
    }
}
int getDistence(int id1, int id2, int x1, int y1, int x2, int y2) {
    //��˳ʱ���˳�����id1��id2�����ڵ��ľ���
    int d1 = absDistence(id2, x1, y1, x2, y2);
    int d2 = absDistence(id1, x1, y1, x2, y2);
    int result = d1 - d2;
    if (result<0) {
        result += (x2-x1+y2-y1)*2;
    }
    return result;
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
            } else if (node->branch.left || node->branch.right) {
                return D;
            } else {
                return L;
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
void connectPreviousBranch(trackNode previousNode, trackNode currentNode, int distence) {
    switch (getFreeBranch(previousNode)) {
        case L:
            switch (previousNode->type) {
                case STATION:
                    previousNode->station.left = currentNode;
                    previousNode->station.ldistance = distence;
                    break;
                case BRANCH:
                    previousNode->branch.left = currentNode;
                    previousNode->branch.ldistance = distence;
                    break;
                case TRAFFIC:
                    previousNode->traffic.left = currentNode;
                    previousNode->traffic.ldistance = distence;
                    break;
                default:
                    break;
            }
            break;
        case R:
            switch (previousNode->type) {
                case STATION:
                    previousNode->station.right = currentNode;
                    previousNode->station.rdistance = distence;
                    break;
                case BRANCH:
                    previousNode->branch.right = currentNode;
                    previousNode->branch.rdistance = distence;
                    break;
                case TRAFFIC:
                    previousNode->traffic.right = currentNode;
                    previousNode->traffic.rdistance = distence;
                    break;
                default:
                    break;
            }
            break;
        case U:
            previousNode->traffic.up = currentNode;
            previousNode->traffic.udistance = distence;
            break;
        case D:
            switch (previousNode->type) {
                case BRANCH:
                    previousNode->branch.down = currentNode;
                    previousNode->branch.ddistance = distence;
                    break;
                case TRAFFIC:
                    previousNode->traffic.down = currentNode;
                    previousNode->traffic.ddistance = distence;
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
    //��ʼ��trainList��trackNodeList
    for (int i=0; i<MAXITEM; i++) {
        trainList[i] = NULL;
        trackNodeList[i] = NULL;
    }
    for (int i=0; i<24; i++) {
        for (int j=0; j<80; j++) {
            idInPosition[i][j]=-1;
            buff[i][j] = ' ';
        }
    }
    FILE *fp = fopen("init.txt", "r");
    FILE *conf = fopen("configure.dat", "w");
    int x1,x2,y1,y2;
    
    if (!fp) {
        fp = stdin;
    }
    printf("=============1601 С��===========\n");
    printf("=============���������===========\n");
    printf("ÿ��С�����ҽ���һ���Լ��Ĺ����������С�𳵵�������");
    fscanf(fp, "%d", &trainNumber);
    fprintf(conf, "%d\n", trainNumber);
    global_id = trainNumber;
    int nodeNumber;
    char nodeType;
    int inputId, inputDistence;
    int inputX, inputY;
    trackNode currentNode, previousNode;
    
    for (int i=0; i<trainNumber; i++) {
        trainList[i] = newTrain();
        int trainNodeListIndex=0;
        printf("��ָ����%d��С�𳵵Ĺ��λ��(���Ͻ���������½�����):\nע�⣬����ԭ������Ļ���Ͻǡ�\n", i);
        fscanf(fp, "%d%d%d%d", &x1, &y1, &x2, &y2);
        fprintf(conf, "%d %d %d %d\n", x1, y1, x2, y2);
        for (int j=x1; j<=x2; j++) {
            buff[y1][j] = buff[y1][j]!=' '?buff[y1][j]: trackChar;
            buff[y2][j] = buff[y2][j]!=' '?buff[y2][j]: trackChar;
        }
        for (int j=y1; j<=y2; j++) {
            buff[j][x1] = buff[j][x1]!=' '?buff[j][x1]: trackChar;
            buff[j][x2] = buff[j][x2]!=' '?buff[j][x2]: trackChar;
        }
        printTrack();
        trainList[i]->x1 = x1;
        trainList[i]->y1 = y1;
        trainList[i]->x2 = x2;
        trainList[i]->y2 = y2;
        printf("����������վ���������ֲ�������Լ�ʮ�ֽ����������ͣ�");
        fscanf(fp, "%d", &nodeNumber);
        fprintf(conf, "%d\n", nodeNumber);
        printf("�ڵ������ڻ𳵱�ź��档�밴˳ʱ���˳������ڵ�����ͺ����ꡣ��شӷǹ�������ϵ�վ�㿪ʼ��վ��ΪS���ֲ��ΪB��ʮ�ֽ����ΪT��\n");
        printf("�������%d���ڵ�����ͺ����꣺", global_id);
        fgetc(fp);
        fscanf(fp, "%c %d %d", &nodeType, &inputX, &inputY);
        fprintf(conf, "%c %d %d\n", nodeType, inputX, inputY);
        buff[inputY][inputX] = stationChar;
        printTrack();
        idInPosition[inputY][inputX] = global_id;
        x[global_id] = inputX;
        y[global_id] = inputY;
        currentNode = newTrackNode();
        trackNodeList[global_id] = currentNode;
        //trackNodeList[global_id+1] = newTrackNode();
        currentNode->type = STATION;
        currentNode->id = global_id;
        trainList[i]->nodeList[trainNodeListIndex++] = global_id;
        previousNode = currentNode;
        global_id++;
        int visited;
        for (int j=1; j<nodeNumber; j++) {
            printf("�������%d���ڵ�����ͺ����꣺", global_id);
            fgetc(fp);
            fscanf(fp, "%c %d %d", &nodeType, &inputX, &inputY);
            fprintf(conf, "%c %d %d\n", nodeType, inputX, inputY);
            if (idInPosition[inputY][inputX] != -1) {
                visited = 1;
                global_id--;
            } else {
                visited = 0;
                idInPosition[inputY][inputX] = global_id;
                x[global_id] = inputX;
                y[global_id] = inputY;
            }
			if (!visited)
			{
				currentNode = trackNodeList[global_id];
			}
			else
			{
				currentNode = trackNodeList[idInPosition[inputY][inputX]];
			}
            if (currentNode == NULL) {
                currentNode = newTrackNode();
                trackNodeList[global_id] = currentNode;
            }
            
            int distence;
            switch (nodeType) {
                case 'S':
                    if (visited) {
                        break;
                    }
                    buff[inputY][inputX] = stationChar;
                    printTrack();
                    currentNode->type = STATION;
                    currentNode->id = global_id;
                    currentNode->station.left = trackNodeList[global_id-1];
                    currentNode->station.ldistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
                    connectPreviousBranch(previousNode, currentNode, currentNode->station.ldistance);
                    break;
                case 'B':
                    if (visited) {
                        currentNode = trackNodeList[idInPosition[inputY][inputX]];
                        if (currentNode->branch.left == previousNode ||
                            currentNode->branch.right == previousNode ||
                            currentNode->branch.down == previousNode) {
                            break;
                        }
                        int range;
                        printf("���������ľ���:");
                        fscanf(fp, "%d", &range);
                        fprintf(conf, "%d\n", range);
                        switch (getFreeBranch(currentNode)) {
                            case L:
                                currentNode->branch.left = previousNode;
                                currentNode->branch.ldistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
                                currentNode->branch.lrange = range;
                                connectPreviousBranch(previousNode, currentNode, currentNode->branch.ldistance);
                                break;
                            case R:
                                currentNode->branch.right = previousNode;
                                currentNode->branch.rdistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
                                currentNode->branch.rrange = range;
                                connectPreviousBranch(previousNode, currentNode, currentNode->branch.rdistance);
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                    buff[inputY][inputX] = branchChar;
                    printTrack();
                    currentNode->type = BRANCH;
                    currentNode->id = global_id;
                    char branchIO;
                    int range = 0;
                    printf("��������Էֲ���У���һ���ֲ���ID,˳ʱ���ǽ���(I)����������ǳ�(O)�������,�Լ�������ľ���:");
                    fscanf(fp, "%d %c %d", &inputId, &branchIO, &range);
                    fprintf(conf, "%d %c %d\n", inputId, branchIO, range);
                    if (trackNodeList[inputId] == NULL) {
                        trackNodeList[inputId] = newTrackNode();
                    }
                    currentNode->branch.pair = trackNodeList[inputId];
                    switch (branchIO) {
                        case 'I':
                            if (!currentNode->branch.left) {
                                currentNode->branch.left = previousNode;
                                currentNode->branch.ldistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
                                distence = currentNode->branch.ldistance;
                            } else {
                                currentNode->branch.right = previousNode;
                                currentNode->branch.rdistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
                                distence = currentNode->branch.rdistance;
                            }
                            break;
                        case 'O':
                            currentNode->branch.down = previousNode;
                            currentNode->branch.ddistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
                            distence = currentNode->branch.ddistance;
                            break;
                        default:
                            break;
                    }
                    currentNode->branch.lrange = range;
                    currentNode->branch.rrange = range;
                    currentNode->branch.train[0] = -1;
                    currentNode->branch.train[1] = -1;
                    connectPreviousBranch(previousNode, currentNode, distence);
                    break;
                case 'T':
                    printf("���������ľ���:");
                    fscanf(fp, "%d", &range);
                    fprintf(conf, "%d\n", range);
                    if (visited) {
                        currentNode = trackNodeList[idInPosition[inputY][inputX]];
                        if (inputX == x1 || inputX == x2) {
                            currentNode->traffic.up = previousNode;
                            currentNode->traffic.udistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
                            distence = currentNode->traffic.udistance;
                        } else {
                            currentNode->traffic.left = previousNode;
                            currentNode->traffic.ldistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
                            distence = currentNode->traffic.ldistance;
                        }
                        connectPreviousBranch(previousNode, currentNode, distence);
                        break;
                    }
                    buff[inputY][inputX] = trafficChar;
                    printTrack();
                    currentNode->type = TRAFFIC;
                    currentNode->id = global_id;
                    if (inputX == x1 || inputX == x2) {
                        currentNode->traffic.up = previousNode;
                        currentNode->traffic.udistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
                        distence = currentNode->traffic.udistance;
                    } else {
                        currentNode->traffic.left = previousNode;
                        currentNode->traffic.ldistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
                        distence = currentNode->traffic.ldistance;
                    }
                    currentNode->traffic.urange = currentNode->traffic.lrange = currentNode->traffic.rrange = currentNode->traffic.drange = range;
                    currentNode->traffic.train[0] = -1;
                    currentNode->traffic.train[1] = -1;
                    connectPreviousBranch(previousNode, currentNode, distence);
                    break;
                default:
                    break;
            }
            trainList[i]->nodeList[trainNodeListIndex++] = currentNode->id;
            global_id++;
            previousNode = currentNode;
        }
        currentNode = trackNodeList[trainList[i]->nodeList[0]];
        currentNode->station.left = previousNode;
        currentNode->station.ldistance = getDistence(previousNode->id, currentNode->id, x1, y1, x2, y2);
        connectPreviousBranch(previousNode, currentNode, currentNode->station.ldistance);
        
        printf("��ָ����%d��С�𳵵�λ�á��𳵲��ɳ�ʼ���ڹ�������ϡ�С��Ĭ��Ϊ˳ʱ�룬ָ����ǰ���ڵ��ID�;���:", i);
        fscanf(fp, "%d%d", &inputId, &inputDistence);
        fprintf(conf, "%d %d\n", inputId, inputDistence);
        trainList[i]->nextNode = inputId;
        trainList[i]->distance = inputDistence;
        trainList[i]->id = i;
        trainList[i]->status = FREE;
    }
    servicePolicy = SEQUENCING;
    controlPolicy = AUTO;
    //input(fp);
    fclose(fp);
    fclose(conf);
}

void input(FILE *fp) {
    int configFile = 1;
    if (!fp) {
        fp = fopen("input.txt", "r");
        configFile = 0;
    }
    char missionType;//S����վ���ͣ������T��ʾ���û𳵲�����Q��ʾ�˳�����C��ʾ�л��������
    char cmdBuff[20], trainDirection[20];
    int nodeID, stopTime, serverTrain;
    int trainID, trainSpeed;
    mainQueueNode mainData;
    
    while (fp && (fscanf(fp, "%s", cmdBuff))==1) {
        missionType = cmdBuff[0];
        switch (missionType) {
            case 'C':
                fscanf(fp, "%s", cmdBuff);
                if (!strcmp(cmdBuff, "AUTO")) {
                    controlPolicy = AUTO;
                } else if (!strcmp(cmdBuff, "MANUAL")) {
                    controlPolicy = MANUAL;
                }
                fscanf(fp, "%s", cmdBuff);
                if (!strcmp(cmdBuff, "SEQUENCING")) {
                    servicePolicy = SEQUENCING;
                } else if (!strcmp(cmdBuff, "BYTHEWAY")) {
                    servicePolicy = BYTHEWAY;
                }
                break;
            case 'S':
                fscanf(fp, "%d%d", &nodeID, &serverTrain);
                stopTime = (rand() % 9)+ 2;//(rand() % (10-2+1))+ a;
                mainData = calloc(1, sizeof(struct _mainQueueNode));
                mainData->type = MSTATION;
                mainData->station.id = nodeID;
                mainData->station.time = stopTime;
                mainData->station.train = serverTrain;
                append(mainMission, mainData);
                break;
            case 'T':
                fgetc(fp);
                fscanf(fp, "%d%d%s", &trainID, &trainSpeed, trainDirection);
                mainData = calloc(1, sizeof(struct _mainQueueNode));
                mainData->type = MTRAIN;
                mainData->train.id = trainID;
                mainData->train.speed = trainSpeed;
                if (!strcmp(trainDirection, "CLOCKWISE")) {
                    mainData->train.direction = MCLOCLWISE;
                } else {
                    mainData->train.direction = MANTICLOCKWISE;
                }
                append(mainMission, mainData);
                break;
            case 'Q':
                mainData = calloc(1, sizeof(struct _mainQueueNode));
                mainData->type = MLOCK;
                append(mainMission, mainData);
                break;
            default:
                break;
        }
    }
	if (fp)
	{
		fclose(fp);
	}
    if (!configFile) {
        fp = fopen("input.txt", "w");
        fclose(fp);
    }
}
