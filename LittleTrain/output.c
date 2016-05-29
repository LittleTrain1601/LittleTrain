//
//  output.c
//  LittleTrain
//
//  Created by 杨林青 on 16/5/28.
//  Copyright © 2016年 1601. All rights reserved.
//

#include "train.h"
#include <stdio.h>

extern char buff[24][80];
extern int x[MAXITEM];
extern int y[MAXITEM];
char screenBuff[24][80];
FILE *outputLog;
void logWriter() {
    outputLog = fopen("output.txt", "w");
}

void putTrain(int id) {
    int x1, y1, x2, y2;
    x1 = trainList[id]->x1;
    y1 = trainList[id]->y1;
    x2 = trainList[id]->x2;
    y2 = trainList[id]->y2;
    int distance = trainList[id]->distance;
    double xPosition, yPosition;
    xPosition = x[trainList[id]->nextNode];
    yPosition = y[trainList[id]->nextNode];
    if (yPosition == y1) {
        if (trainList[id]->direction == clockwise) {
            xPosition = xPosition - distance;
            if (xPosition < x1) {
                distance -= xPosition + distance - x1;
                xPosition = x1;
                yPosition = yPosition + distance;
                if (yPosition > y2) {
                    distance -= y2-y1;
                    yPosition = y2;
                    xPosition += distance;
                    if (xPosition > x2) {
                        distance -= x2-x1;
                        xPosition = x2;
                        yPosition -= distance;
                    }
                }
            }
        } else {
            xPosition += distance;
            if (xPosition > x2) {
                distance -= x2-(xPosition-distance);
                xPosition = x2;
                yPosition += distance;
                if (yPosition > y2) {
                    distance -= y2-y1;
                    yPosition = y2;
                    xPosition -= distance;
                    if (xPosition<x1) {
                        distance -= x2-x1;
                        xPosition = x1;
                        yPosition -= distance;
                    }
                }
            }
        }
    } else if (xPosition == x2) {
        if (trainList[id]->direction == clockwise) {
            yPosition -= distance;
            if (yPosition < y1) {
                distance -= yPosition + distance - y1;
                yPosition = y1;
                xPosition -= distance;
                if (xPosition < x1) {
                    distance -= x2-x1;
                    xPosition = x1;
                    yPosition += distance;
                    if (yPosition > y2) {
                        distance -= y2-y1;
                        yPosition = y2;
                        xPosition += distance;
                    }
                }
            }
        } else {
            yPosition += distance;
            if (yPosition > y2) {
                distance -= y2 - (yPosition - distance);
                xPosition -= distance;
                if (xPosition < x1) {
                    distance -= x2-x1;
                    xPosition = x1;
                    yPosition -= distance;
                    if (yPosition < y1) {
                        distance -= y2-y1;
                        yPosition = y1;
                        xPosition += distance;
                    }
                }
            }
        }
    } else if (yPosition == y2) {
        if (trainList[id]->direction == clockwise) {
            xPosition += distance;
            if (xPosition > x2) {
                distance -= x2-(xPosition-distance);
                xPosition = x2;
                yPosition -= distance;
                if (yPosition < y1) {
                    distance -= y2-y1;
                    yPosition = y1;
                    xPosition -= distance;
                    if (xPosition<x1) {
                        distance -= x2-x1;
                        xPosition = x1;
                        yPosition += distance;
                    }
                }
            }
        } else {
            xPosition -= distance;
            if (xPosition < x1) {
                distance -= xPosition + distance -x1;
                xPosition = x1;
                yPosition -= distance;
                if (yPosition < y1) {
                    distance -= y2-y1;
                    yPosition = y1;
                    xPosition += distance;
                    if (xPosition > x2) {
                        distance -= x2-x1;
                        xPosition = x2;
                        yPosition += distance;
                    }
                }
            }
        }
    } else if (xPosition == x1) {
        if (trainList[id]->direction == clockwise) {
            yPosition += distance;
            if (yPosition > y2) {
                distance -= y2-(yPosition-distance);
                yPosition = y2;
                xPosition += distance;
                if (xPosition > x2) {
                    distance -= x2-x1;
                    xPosition = x2;
                    yPosition -= distance;
                    if (yPosition < y1) {
                        distance -= y2-y1;
                        yPosition = y1;
                        xPosition -= distance;
                    }
                }
            }
        } else {
            yPosition -= distance;
            if (yPosition < y1) {
                distance -= yPosition + distance - y1;
                yPosition = y1;
                xPosition += distance;
                if (xPosition > x2) {
                    distance -= x2-x1;
                    xPosition = x2;
                    yPosition += distance;
                    if (yPosition > y2) {
                        distance -= y2-y1;
                        yPosition = y2;
                        xPosition -= distance;
                    }
                }
            }
        }
    }
    screenBuff[(int)yPosition][(int)xPosition] = id + '0';
}
void viewer() {
    system("clear");
    for (int i=0; i<24; i++) {
        for (int j=0; j<80; j++) {
            screenBuff[i][j] = buff[i][j];
        }
    }
    for (int i=0; i<MAXITEM; i++) {
        if (trainList[i]) {
            putTrain(trainList[i]->id);
        }
    }
    for (int i=0; i<24; i++) {
        for (int j=0; j<80; j++) {
            putchar(screenBuff[i][j]);
        }
        putchar('\n');
    }
    printf("%lu\n", RUN_TIME);
}