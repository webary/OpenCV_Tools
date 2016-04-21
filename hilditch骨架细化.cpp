#include "include/imgpro.h"

int main(int argc, char**argv)
{
    char fileName[128];
    if (argc > 1) {
        strcpy(fileName, argv[1]);
    } else {
        puts("��������Ҫ�����ͼ������");
        cin >> fileName;
    }
    IplImage *pSrc = loadImg(fileName, 0, 0, 0);
    if (!pSrc) {
        cout << "��ȡ�ļ�ʧ�ܣ�" << endl;
        return -1;
    }
    IplImage *pTemp = cvCreateImage(cvGetSize(pSrc), pSrc->depth, pSrc->nChannels);
    IplImage *pDst = cvCreateImage(cvGetSize(pSrc), pSrc->depth, pSrc->nChannels);

    //��ԭͼ��ת��Ϊ��ֵͼ��,ע���ּ��Ļ�Ӧ��ʹ��CV_THRESH_BINARY_INV
    cvThreshold(pSrc, pTemp, 128, 1, CV_THRESH_BINARY_INV);
    //ͼ��ϸ�����ĺ���
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
    saveRGB(pDst, 0, 1); //��������ֵ
    reverseRGB();       //������ȡ��
    saveIplImage(pDst); //����RGB��ͼ��ָ��
    showImg(pDst, "dst", 0, 1);
    saveImg("3save", pDst);
    cvWaitKey(0);
    return 0;
}
