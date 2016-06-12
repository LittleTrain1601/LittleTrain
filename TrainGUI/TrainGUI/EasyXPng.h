#ifndef MYEASYX_H
#define MYEASYX_H

#pragma comment(lib, "./EasyXWithGDIPlus/EasyXGdiPlusLib.lib")

#include "./EasyXWithGDIPlus/easyx.h"
#include "MyEasyX.h"

void initgraphEx(int width, int height, int flag = NULL);
void closegraphEx();
void loadimage(CMyEasyX * image, LPCTSTR pImgFile, int nWidth = 0, int nHeight = 0);
void putimage(int dstX, int dstY, CMyEasyX *image);
void rotateimage(CMyEasyX *dstimg, CMyEasyX *srcimg, float angle);
void BeginBatchDrawEx();
void EndBatchDrawEx();

#endif