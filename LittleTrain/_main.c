//
//  main.c
//  LittleTrain
//
//  Created by 杨林青 on 16/5/11.
//  Copyright © 2016年 1601. All rights reserved.
//

#include <stdio.h>
#include "train.h"

trackNode trackNodeList[MAXITEM];
train trainList[MAXITEM];

int main(int argc, const char * argv[]) {

    build();
    input();
    return 0;
}
