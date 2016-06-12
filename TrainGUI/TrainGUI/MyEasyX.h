// MyEasyX.h: interface for the CMyEasyX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYEASYX_H__1A6D4734_44AD_47DF_ABF7_967B0BDE4E80__INCLUDED_)
#define AFX_MYEASYX_H__1A6D4734_44AD_47DF_ABF7_967B0BDE4E80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(linker, "/NODEFAULTLIB:./GDIPlus/GdiPlus.lib")//忽略GdiPlus.lib的原始路径
#pragma comment(lib, "./EasyXWithGDIPlus/GDIPlus/GdiPlus.lib")

#include "./EasyXWithGDIPlus/GDIPlus/Includes/Gdiplus.h"
class Gdiplus::Bitmap;

class CMyEasyX
{
public:
	int getHeight();
	int getWidth();
	CMyEasyX();
	virtual ~CMyEasyX();
	//CxImage * pngImage;
	//HBITMAP hBmp;//png图片加载转换后句柄
	Gdiplus::Bitmap * pngImage;
	float angle;
};

typedef CMyEasyX IMAGE_PNG;

#endif // !defined(AFX_MYEASYX_H__1A6D4734_44AD_47DF_ABF7_967B0BDE4E80__INCLUDED_)
