//
//  input.c
//  LittleTrain
//
//  Created by 杨林青 on 16/5/15.
//  Copyright © 2016年 1601. All rights reserved.
//

#include <stdio.h>
#include "train.h"

void build() {
    FILE *fp = fopen("track.txt", "r");
    char buff[24][80];
    for (int i=0; i<24; i++) {
        for (int j=0; j<80; j++) {
            buff[i][j] = fgetc(fp);
        }
        fgetc(fp);
    }
}