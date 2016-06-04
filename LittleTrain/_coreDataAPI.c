//
//  coreDataAPI.c
//  LittleTrain
//
//  Created by 杨林青 on 16/5/28.
//  Copyright © 2016年 1601. All rights reserved.
//

#include "train.h"
#include <stdio.h>
#include <stdlib.h>

train newTrain(){
    train current = calloc(1, sizeof(struct _train));
    return current;
}
trackNode newTrackNode(){
    trackNode current = calloc(1, sizeof(struct _trackNode));
    return current;
}