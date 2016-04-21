#include "include/imgpro.h"

int main(int argc, char**argv)
{
    char fileName[128];
    if (argc > 1) {
        strcpy(fileName, argv[1]);
    } else {
        puts("请输入需要读入的图像名：");
        cin >> fileName;
    }
    IplImage *pSrc = loadImg(fileName, 0, 0, 0);
    if (!pSrc) {
        cout << "读取文件失败！" << endl;
        return -1;
    }
    IplImage *pTemp = cvCreateImage(cvGetSize(pSrc), pSrc->depth, pSrc->nChannels);
    IplImage *pDst = cvCreateImage(cvGetSize(pSrc), pSrc->depth, pSrc->nChannels);

    //将原图像转换为二值图像,注意字迹的话应该使用CV_THRESH_BINARY_INV
    cvThreshold(pSrc, pTemp, 128, 1, CV_THRESH_BINARY_INV);
    //图像细化核心函数
    Hilditch(pSrc, pDst, 128);

    for (int i = 0; i < pDst->height; ++i) {
        for (int j = 0; j < pDst->width; ++j) {
            if (CV_IMAGE_ELEM(pDst, uchar, i, j) == 1)
                CV_IMAGE_ELEM(pDst, uchar, i, j) = 255;
        }
    }
    cout << Entropy(pDst) << endl;
    showImg(pSrc, "src", 0);
    //showImg(pDst,"dst",1);
    saveRGB(pDst, 0, 1); //保存像素值
    reverseRGB();       //对像素取反
    saveIplImage(pDst); //保存RGB到图像指针
    showImg(pDst, "dst", 0, 1);
    saveImg("3save", pDst);
    cvWaitKey(0);
    return 0;
}
