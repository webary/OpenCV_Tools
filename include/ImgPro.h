/**
    ��ͷ�ļ�ʹ��OpenCV��ʵ�ֶ�ȡͼƬ������������Ϣ��ֱ�����һ��ͼƬ��
    ��ͼ�����һЩ�������������
    @author : webary
    @time (last update time): 2014.7.12  19:00
**/
#pragma once
#ifndef _IMGPRO_H_
#define _IMGPRO_H_

//����Ƿ�ʹ�ö��̼߳���
#define b_multiThread 0

#include <windows.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cv.h>
#include <highgui.h>
#include <opencv2/legacy/legacy.hpp>	//openCV 2.3�ϵ�Ŀ¼,֧�ֺ���cvCalcPGH

typedef const char cchar;
typedef const IplImage CIplImage;

#if !b_multiThread
using std::vector;
extern vector<vector<uchar> >R, G, B, A;
extern vector<vector<float> >H, S, L;
extern int width, height, depth;
extern IplImage *pimg, *pGray;
#define InitWith_pimg pimg
#else
#define InitWith_pimg 0
#endif

extern char info[10000];

//ΪRGB������������ڴ�
void getRGBMemory(int w, int h);
IplImage *loadImg(cchar* tmpimgName, int ask = 1, int cls = 1, int color = 1, int show = 1);
#ifdef __AFXWIN_H__
IplImage *loadImg(CString tmpimgstring, int ask = 1, int cls = 1, int color = 1, int show = 1);
#endif
void saveRGB(CIplImage* img = InitWith_pimg, cchar *fileOutName = 0, bool getMem = 0);
void outputImg(cchar* imgOutName, int readAgain = 1, IplImage* img = InitWith_pimg);
void saveIplImage(IplImage* img = InitWith_pimg);
void saveImg(cchar* imgSaveName, IplImage* img = InitWith_pimg);
int  showImg(CIplImage* img = InitWith_pimg, cchar* str = "ͼƬ��ʾ", cchar* ss = "��ʾͼ�����", int pause = 1);
void releaseImg(IplImage** ppimg);
CvSeq* findContour(CIplImage* src, IplImage** dst = 0, int th = 100, int filled = 0);
double find_obj(cchar* imgName, cchar* objName, int showInfo = 0, int showMatch = 0, int showSurf = 0);
double find_obj(CIplImage* obj_color, CIplImage* img_color, int showInfo = 0, int showMatch = 0, int showSurf = 0);
void matchWeight(CIplImage* img1, CIplImage* img2, double* wm, int show = 0, int n = 20, int m = 20);
void Screen(const char *filename = "");

void Hilditch(CIplImage* src, IplImage* dst = 0, int th = 128, int maxIterations = -1);
float Entropy(CIplImage* img = InitWith_pimg, int th = 1, int w = 0, int h = 0);
IplImage* equalizeHist(CIplImage* img = InitWith_pimg, int saveFile = 0);
void Histeq_self(IplImage** ppimg, int save = 0);
int Otsu(CIplImage* src);


#ifdef _MSC_VER
#	include <Shlwapi.h>	//PathFileExists()
#	pragma comment(lib,"shlwapi.lib")
#else
#	include <io.h>      //access()
#endif // _MSC_VER

/* �ж��ļ��Ƿ���ڡ������ڷ����棬���򷵻ؼ�
The mode argument can be one of :
00 Existence only
02 Write permission
04 Read permission
06 Read and write permission
*/
inline int isFileExists(cchar* fileName, int amode = 0)
{
#ifdef _MSC_VER
    return PathFileExists(fileName);
#else
    return !access(fileName, amode);
#endif // _MSC_VER
}

#ifdef _MSC_VER
#	define FILEOPEN(fp,filename,mode) fopen_s(&fp,filename,mode);
#	define FPRINTF fprintf_s
#else
#	define FILEOPEN(fp,filename,mode) fp = fopen(filename,mode);
#	define FPRINTF fprintf
#   include "ImgPro.cpp" //���ݷ���Ŀ��������
#endif // _MSC_VER


struct MyRect {
	int x,y;
	MyRect(int _x,int _y)
	{
		x=_x,y=_y;
	}
	//rectImg��ԭʼͼ��ĳߴ�,rectOutMax�Ǳ任�����ĳߴ�,����ʵ�ʳߴ�
	static MyRect resize(MyRect rectImg,MyRect rectOutMax = MyRect(512,512))
	{
		//����ͼƬ��С��̬������ʾ����
		MyRect show_WH(rectImg);	//��Ҫ��ʾ�Ŀ��
		if(rectImg.x>rectOutMax.x || rectImg.y>rectOutMax.y) {	//���ͼƬ�������ο�
			if((double)rectImg.x/rectImg.y > (double)rectOutMax.x/rectOutMax.y) { //���ͼƬ�Ͽ�
				show_WH.x = rectOutMax.x;
				show_WH.y = rectImg.y * rectOutMax.x / rectImg.x;
			} else {	//���ͼƬ�ϳ�
				show_WH.y = rectOutMax.y;
				show_WH.x = rectImg.x * rectOutMax.y / rectImg.y;
			}
		}
		return show_WH;
	}
#ifdef __OPENCV_CORE_TYPES_H__
	MyRect(CvSize size)
	{
		x = size.width;
		y = size.height;
	}
	static CvSize  resize(CvSize ImgSize,MyRect rectOutMax = MyRect(512,512))
	{
		//����ͼƬ��С��̬������ʾ����
		MyRect rectImg(ImgSize);
		CvSize show_WH = {rectImg.x,rectImg.y};
		if(rectImg.x>rectOutMax.x || rectImg.y>rectOutMax.y) {	//���ͼƬ�������ο�
			if((double)rectImg.x/rectImg.y > (double)rectOutMax.x/rectOutMax.y) { //���ͼƬ�Ͽ�
				show_WH.width = rectOutMax.x;
				show_WH.height = rectImg.y * rectOutMax.x / rectImg.x;
			} else {	//���ͼƬ�ϳ�
				show_WH.width = rectImg.x * rectOutMax.y / rectImg.y;
				show_WH.height = rectOutMax.y;
			}
		}
		return show_WH;
	}
#endif
};

#endif // _IMGPRO_H_
