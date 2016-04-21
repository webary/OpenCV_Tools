/**
    ��ͷ�ļ�ʹ��OpenCV��ʵ�ֶ�ȡͼƬ������������Ϣ��ֱ�����һ��ͼƬ��
    ��ͼ�����һЩ�������������
    @author : webary
    @time (last update time): 2014.11.13  18:00
    @update���޸���vs��Ŀ�е��ø�ͷ�ļ��ı����������
**/
#include "ImgPro.h"

using namespace std;

#if !b_multiThread
vector<vector<uchar> >R, G, B, A;
vector<vector<float> >H, S, L;
IplImage *pimg = 0, *pGray = 0;
int width = 0, height = 0, depth = 0;
#endif

char info[10000] = "";

//ΪRGB������������ڴ�
void getRGBMemory(int w, int h)
{
    R.resize(h);
    G.resize(h);
    B.resize(h);
    if (depth == 32) {
        A.resize(h);
    }
    for (int i = 0; i < h; i++) {
        R[i].resize(w, 0);
        G[i].resize(w, 0);
        B[i].resize(w, 0);
        if (depth == 32) {
            A[i].resize(w, 0);
        }
    }
#ifdef _HSL_
    H.resize(height);
    S.resize(height);
    L.resize(height);
    for (i = 0; i < height; i++) {
        H[i].resize(width);
        S[i].resize(width);
        L[i].resize(width);
    }
#endif
}

//����ͼƬ
IplImage *loadImg(cchar* tmpimgName, int ask, int cls, int color, int show)
{
    if (tmpimgName == 0 || tmpimgName[0] == 0) {
        return 0;
    }
    char imgName[200], tempName[200];
    strcpy(imgName, tmpimgName);
    int first = 1, flag;
    IplImage * img = 0;
    do {
        flag = 0;
        if (first == 0) {
            cout << "��������Ҫ�����ͼƬ: ";
            cin.getline(imgName, 200);
        }
        if (!isFileExists(imgName, 0)) {                    //��ʧ��,û�ж�ȡȨ��
            strcpy(tempName, imgName);
            if (strstr(tempName, ".jpg") == 0) {            //�ļ���û����.jpg��β
                strcat(tempName, ".jpg");
                if (!isFileExists(tempName, 0)) {           //��ʧ��,û�ж�ȡȨ��
                    strcpy(tempName, imgName);
                    if (strstr(tempName, ".png") == 0) {    //�ļ���û����.png��β
                        strcat(tempName, ".png");
                        if (!isFileExists(tempName, 0)) {   //��ʧ��,û�ж�ȡȨ��
                            strcpy(tempName, imgName);
                            if (strstr(tempName, ".bmp") == 0) { //�ļ���û����.bmp��β
                                strcat(tempName, ".bmp");
                                if (isFileExists(tempName, 0)) {
                                    strcpy(imgName, tempName);
                                }
                            }
                        } else {
                            strcpy(imgName, tempName);
                        }
                    }
                } else {
                    strcpy(imgName, tempName);
                }
            }
            if (!isFileExists(imgName, 0)) {                        //��ʧ��,û�ж�ȡȨ��
                if (first) {
                    first = 0;
                    flag = 1;
                    continue;
                }
                cout << "��ͼƬ \"" << imgName << "\" ʧ�� ��" << endl;
                flag = 1;
            }
        }
        if (isFileExists(imgName, 0) && (strstr(imgName, ".jpg") || strstr(imgName, ".bmp") || strstr(imgName, ".png"))) {                      //ͼƬ���Զ�ȡ
            img = cvLoadImage(imgName, color);
            if (img == 0) {
                cout << "��ͼƬ \"" << imgName << "\" ʧ�� ��" << endl;
                first = 0;
                flag = 1;
                continue;
            }
            if (cls) {
                system("cls");
            }
            char tips[200] = "�ѳɹ�����ͼƬ ";
            strcat(tips, imgName);
            if (show) {
                cout << tips << endl;
            }
            if (first && ask && MessageBox(NULL, "�Ƿ�������������ͼƬ��", tips, MB_YESNO) == IDYES) {
                if (cls) {
                    system("cls");
                }
                flag = 1;
            }
        }
        first = 0;
    } while (flag);
    if (img == 0) {
        return 0;
    }
    if (show) {
        cout << "width(��) : " << img->width << "    height(��) : " << img->height << endl;
    }
    //    <<"    depth(λ���) : "<<img->depth
    width = img->width;
    height = img->height;
    depth = img->depth;
    pimg = img;
    //getRGBMemory();
    return img;
}
#ifdef __AFXWIN_H__ //��ʶ��__AFXWIN_H__��MFC�ж���
IplImage *loadImg(CString tmpimgName, int ask/*=1*/, int cls/*=1*/, int color/*=1*/, int show/*=1*/)
{
    return loadImg((LPSTR)(LPCTSTR)tmpimgName, ask, cls, color, show);
}
#endif  //__AFXWIN_H__

//���ͼƬ�������ļ����ڶ���������ѡ���Ƿ��ٴζ�ȡRGBֵ---���̰߳�ȫ��readAgainΪ��ʱ��
void outputImg(cchar* imgOutName, int readAgain/* = 1*/, IplImage* img/* = pimg*/)
{
    if (imgOutName[0] == 0 || img == 0) {
        return;
    }
    if (readAgain)
#if b_multiThread
        MessageBox(0, "�ѿ������̣߳��޷�ʹ���̲߳���ȫ��ȫ�ֱ���", "��ȡȫ�ֱ���ʧ��", 0);
#else
        saveIplImage(img);
#endif
    if (strstr(imgOutName, ".jpg") == 0 && strstr(imgOutName, ".bmp") == 0 && strstr(imgOutName, ".png") == 0) {
        char name[200];
        strcpy(name, imgOutName);
        strcat(name, ".jpg");
        cvSaveImage(name, img);
    } else {
        cvSaveImage(imgOutName, img);
    }
}

//����ͼƬ���ļ�---�˺����̰߳�ȫ��Ĭ��readAgainΪ�٣�
void saveImg(cchar* imgSaveName, IplImage* img)
{
    outputImg(imgSaveName, 0, img);
}

//����ͼƬ������Ϣ���ļ�������---���̰߳�ȫ��ʹ����ȫ�ֱ���RGB��
void saveRGB(CIplImage* img/* = pimg*/, cchar *fileOutName/* = 0*/, bool getMem/* = 0 */)
{
    if (img == 0) {
        return;
    }
    if (getMem) {
        getRGBMemory(width, height);
    }
    uchar* data = (uchar*)img->imageData;
    if (fileOutName && fileOutName[0]) {
        FILE* fp1 = 0;
        FILEOPEN(fp1, fileOutName, "w");
        if (fp1) {
            if (img->nChannels == 3) {
                for (int y = 0; y < img->height; y++)
                    for (int x = 0, yy = y * img->widthStep; x < img->width; x++) {
                        FPRINTF(fp1, "%d\t%d\t%d\n", data[yy + 3 * x + 2], data[yy + 3 * x + 1], data[yy + 3 * x]);
                    }
            } else if (img->nChannels == 1) {
                for (int y = 0; y < img->height; y++)
                    for (int x = 0, yy = y * img->widthStep; x < img->width; x++) {
                        FPRINTF(fp1, "%d\n", data[yy + x]);
                    }
            }
            fclose(fp1);
        }
    } else {
#if b_multiThread
        MessageBox(0, "�ѿ������̣߳��޷�ʹ���̲߳���ȫ��ȫ�ֱ���", "��ȡȫ�ֱ���ʧ��", 0);
#else
        if (img->nChannels == 3) {
            for (int y = 0; y < img->height; y++)
                for (int x = 0, yy = y * img->widthStep; x < img->width; x++) {
                    R[y][x] = data[yy + 3 * x + 2];
                    G[y][x] = data[yy + 3 * x + 1];
                    B[y][x] = data[yy + 3 * x];
                }
        } else if (img->nChannels == 1) {
            for (int y = 0; y < img->height; y++)
                for (int x = 0, yy = y * img->widthStep; x < img->width; x++) {
                    R[y][x] = data[yy + x];
                }
        }
#endif
    }
}

//���浱ǰ��RGB���鵽IplImageָ����---���̰߳�ȫ
void saveIplImage(IplImage* img/* = pimg*/)
{
#if b_multiThread
    MessageBox(0, "�ѿ������̣߳��޷�ʹ���̲߳���ȫ��ȫ�ֱ���", "��ȡȫ�ֱ���ʧ��", 0);
#else
    uchar* data = (uchar*)img->imageData;
    if (img->nChannels == 3) {
        for (int y = 0; y < img->height; y++)
            for (int x = 0, yy = y * img->widthStep; x < img->width; x++) {
                data[yy + 3 * x + 2] = R[y][x];
                data[yy + 3 * x + 1] = G[y][x];
                data[yy + 3 * x] = B[y][x];
            }
    } else if (img->nChannels == 1) {
        for (int y = 0; y < img->height; y++)
            for (int x = 0, yy = y * img->widthStep; x < img->width; x++) {
                data[yy + x] = R[y][x];
            }
    }
#endif
}

//�õ�ĳһ�����ص�ĻҶ�ֵ---���̰߳�ȫ
inline unsigned char getGray(int i, int j)
{
#if b_multiThread
    MessageBox(0, "�ѿ������̣߳��޷�ʹ���̲߳���ȫ��ȫ�ֱ���", "��ȡȫ�ֱ���ʧ��", 0);
    return 0;
#else
    //return (.3*R[i][j]+.6*G[i][j]+.1*B[i][j]+1)/3;
    if (pimg->nChannels == 3) {
        return (R[i][j] + G[i][j] + B[i][j] + 1) / 3;
    } else {
        return R[i][j];
    }
#endif
}

//����ȡ��---���̰߳�ȫ
void reverseRGB(int th = 255)
{
#if b_multiThread
    MessageBox(0, "�ѿ������̣߳��޷�ʹ���̲߳���ȫ��ȫ�ֱ���", "��ȡȫ�ֱ���ʧ��", 0);
#else
    for (int i = 0; i<height; i++)
        for (int j = 0; j<width; j++) {
            R[i][j] = R[i][j]>th ? 0 : 255 - R[i][j];
            G[i][j] = G[i][j]>th ? 0 : 255 - G[i][j];
            B[i][j] = B[i][j] > th ? 0 : 255 - B[i][j];
        }
#endif
}

//�ҶȻ�ͼ��---���̰߳�ȫ
void grayRGB()
{
#if b_multiThread
    MessageBox(0, "�ѿ������̣߳��޷�ʹ���̲߳���ȫ��ȫ�ֱ���", "��ȡȫ�ֱ���ʧ��", 0);
#else
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            R[i][j] = G[i][j] = B[i][j] = getGray(i, j);
        }
#endif
}

//���Ʋ��˳�ƫ�ף����Ƚϸߣ���������---���̰߳�ȫ
void controlWhtieNoise(int th)
{
#if b_multiThread
    MessageBox(0, "�ѿ������̣߳��޷�ʹ���̲߳���ȫ��ȫ�ֱ���", "��ȡȫ�ֱ���ʧ��", 0);
#else
    //��һ��˼�룺if(R[i][j]>th && G[i][j]>th && B[i][j]>th)
    for (int i = 0; i<height; i++)
        for (int j = 0; j < width; j++) {
            if (pimg->nChannels == 3) {
                int sum = (int)R[i][j] + G[i][j] + B[i][j];
                if (sum > 3 * th) {
                    R[i][j] = G[i][j] = B[i][j] = (uchar)255;
                } else {
                    R[i][j] = G[i][j] = B[i][j] = (uchar)((sum + 1) / 3);
                }
            } else if (R[i][j]>th) {
                R[i][j] = (uchar)255;
            }
        }
#endif
}

//��ʾͼƬ
int showImg(CIplImage* img/* = pimg*/, cchar* str/* = "ͼƬ��ʾ"*/, cchar* ss/*"��ʾͼ�����"*/, int pause/*=1*/)
{
    if (img == 0) {
#ifdef __AFXWIN_H__
        MessageBox(NULL, "ͼ��ָ����Ч������������Чͼ�����ʹ�øù���", ss, MB_ICONSTOP);
#endif
        return 0;
    }
    cvNamedWindow(str);
    cvShowImage(str, img);
    if (pause == 1) {
        cvWaitKey(0);
        cvDestroyWindow(str);//���ٴ���
    }
    return 1;
}

void showImg(cchar *file)
{
    if (strstr(file, ".jpg") == 0 && strstr(file, ".bmp") == 0 && strstr(file, ".png") == 0) {
        char name[200];
        strcpy(name, file);
        strcat(name, ".jpg");
        system(name);
    } else {
        system(file);
    }
}

//�ͷ�ͼƬ�ڴ�ָ��
void releaseImg(IplImage** ppimg/* = &pimg*/)
{
    if (*ppimg != NULL) {
        cvReleaseImage(ppimg);
        *ppimg = 0;
    }
}

//���ļ��ж�ȡ����ͼƬ
void readBMPFromFolder(const char* fileFolder, char readPath[100][300], int del = 1)
{
    char str[300] = "DIR /s /b /o:e ";  // /s�õ������ļ����Լ����ļ���������ͼƬ
    strcat(str, fileFolder);
    strcat(str, " > fileList.txt");
    system(str);                // ������ļ����������ļ���fileList.txt
    int i = 0, flag = 1;        // flagΪ�ļ�������ʶ;
    FILE* fp;
    FILEOPEN(fp, "fileList.txt", "r");
    while (1) {
        char filePath[200] = " ";
        while (strstr(filePath, ".bmp") == 0
                && strstr(filePath, ".jpg") == 0
                && strstr(filePath, ".png") == 0)
            if (fgets(filePath, 199, fp) == NULL) {
                flag = 0;       // �ļ��Ѷ��ֱ꣬����ֹ
                break;
            }
        if (flag == 0) {
            break;
        }
        strcpy(readPath[i++], filePath);    //�õ���ͼ��ľ���·��
    }
    fclose(fp);
    if (del) {
        remove("fileList.txt");
    }
}

//�����䷽���㷨 ���Ҷ�ͼ��Ҷȼ��ָ�(ע��:�������Ҫ�ǻҶ�ͼ������)
int Otsu(CIplImage* src)
{
    int height = src->height, width = src->width, threshold, i, j;
    double histogram[256] = { 0 };
    for (i = 0; i < height; i++) {
        uchar* p = (uchar*)src->imageData + src->widthStep * i;
        for (j = 0; j < width; j++) {
            histogram[*p++]++;
        }
    }
    int size = height * width;
    for (i = 0; i < 256; i++) {
        histogram[i] /= size;
    }
    double avgValue = 0, maxVariance = 0, w = 0, u = 0;
    for (i = 0; i < 256; i++) {
        avgValue += i * histogram[i];    //����ͼ���ƽ���Ҷ�
    }
    for (i = 0; i < 256; i++) {
        w += histogram[i];
        u += i * histogram[i];
        double t = avgValue * w - u;
        double variance = t * t / (w * (1 - w));
        if (variance > maxVariance) {
            maxVariance = variance;
            threshold = i;
        }
    }
    return threshold;
}

//��ͼƬֱ��ͼ���⻯����--����cv�⺯��--��Ҫ�ͷŸ�ָ��
IplImage* equalizeHist(CIplImage* img, int saveFile/*=0*/)
{
    if (img == 0) {
        return 0;
    }
    IplImage* pGray = cvCreateImage(cvGetSize(img), img->depth, 1);
    if (img->nChannels == 3) {
        cvCvtColor(img, pGray, CV_BGR2GRAY);
    } else if (img->nChannels == 1) {
        cvCopy(img, pGray);
    } else {
        releaseImg(&pGray);
        return 0;
    }
    cvEqualizeHist(pGray, pGray);
    if (saveFile) {
        outputImg("���⻯.jpg", 0, pGray);
    }
    return pGray;
}

//��ͼƬֱ��ͼ���⻯����--�����Աຯ��--��Ҫ�ͷŸ�ָ��
IplImage* Histeq(CIplImage* img, int saveFile = 0)
{
    if (img == 0) {
        return 0;
    }
    IplImage* pGray = cvCreateImage(cvGetSize(img), img->depth, 1);
    if (img->nChannels == 3) {
        cvCvtColor(img, pGray, CV_BGR2GRAY);
    } else if (img->nChannels == 1) {
        cvCopy(img, pGray);
    } else {
        releaseImg(&pGray);
        return 0;
    }
    double histogram[256] = { 0 }, dTemp;
    int i, j, k;
    uchar* p;
    for (i = 0; i < pGray->height; i++) {
        p = (uchar*)pGray->imageData + pGray->widthStep * i;
        for (j = 0; j < pGray->width; j++) {
            histogram[*p++]++;
        }
    }
    int size = pGray->height * pGray->width;
    for (i = 0; i < 256; i++) {
        histogram[i] /= size;
    }
    for (i = 0; i < pGray->height; i++) {
        p = (uchar*)pGray->imageData + pGray->widthStep * i;
        for (j = 0; j < pGray->width; j++) {
            dTemp = 0;
            int gray = getGray(i, j);
            for (k = 0; k < gray; ++k) {
                dTemp += histogram[k];
            }
            int tar = int(255 * dTemp + .5);
            if (tar < 0) {
                tar = 0;
            } else if (tar>255) {
                tar = 255;
            }
            *p++ = tar;
        }
    }
    if (saveFile) {
        outputImg("���⻯.jpg", 0, pGray);
    }
    return pGray;
}

//��ͼƬֱ��ͼ���⻯����--�������������ָ����
void Histeq_self(IplImage** ppimg, int save)
{
    IplImage* img = *ppimg;
    saveRGB(img);
    controlWhtieNoise(Otsu(img));//����RGB�����Զ���ֵȥ��
    reverseRGB();           //ȡ��RGB
    saveIplImage(img);      //��RGB���浽ͼ��ָ��
    IplImage* gray = equalizeHist(img); //���⻯
    gray = equalizeHist(gray);//Ŀ���Ǻ�ɫ�����ٽ���һ�ξ��⻯
    saveRGB(gray);
    reverseRGB();
    saveIplImage(gray);
    if (save) {
        outputImg("histeq", 0, gray);
    }
    releaseImg(ppimg);
    *ppimg = gray;
}

//����ͼƬ������
CvSeq* findContour(CIplImage* src, IplImage** dst, int th, int filled)
{
    int releaseDst = 0;
    if (dst == 0) { // û�д��ݶ���ָ�����ú���
        releaseDst = 1;
        IplImage* ddst = cvCreateImage(cvSize(src->width, src->height), 8, 1);
        dst = &ddst;
    } else if (*dst == 0) { // ���ݵĶ���ָ��û�г�ʼ��Ϊͼ��ָ��
        *dst = cvCreateImage(cvSize(src->width, src->height), 8, 1);
    }
    if (src->nChannels == 3) {
        cvCvtColor(src, *dst, CV_BGR2GRAY);
    } else {
        cvCopy(src, *dst);
    }
    CvMemStorage *storage = cvCreateMemStorage();
    if (th == -1) {
        int getTh(CIplImage*);
        th = getTh(src);
    }
    //cvSmooth(*dst,*dst,CV_MEDIAN,3,3,0,0);        //3x3��ֵ�˲�
    cvCanny(*dst, *dst, th, th);
    CvSeq *contour = NULL;
    cvFindContours(*dst, storage, &contour);            //�ڶ�ֵͼ����Ѱ������
    cvThreshold(*dst, *dst, th, 255, CV_THRESH_BINARY); //��ͼ����й̶���ֵ����
    if (filled) {
        cvDrawContours(*dst, contour, cvScalarAll(255), cvScalarAll(200), 0, CV_FILLED);
    } else {
        cvDrawContours(*dst, contour, cvScalarAll(255), cvScalarAll(200), 1);
    }
    if (releaseDst) {
        releaseImg(dst);
    }
    return contour;
}

//����ͼƬ��7��Hu������
double* getHu(CIplImage* src, int th, int printHu, int show, int save, IplImage* dst)
{
    if (src == 0) {
        return 0;
    }
    int delDst = 0;
    if (dst == 0) {
        dst = cvCreateImage(cvSize(src->width, src->height), 8, 1);
        delDst = 1;
    }
    CvSeq *contour = findContour(src, &dst, th);
    CvMoments moments;
    static CvHuMoments hu;          //�ֲ�������Ϊ����ֵ����ȫ,������Ϊ��̬����
    cvMoments(dst, &moments, 1);    //�������κ͹�դ��״����ߴ����׵����о�
    cvGetHuMoments(&moments, &hu);  //����7��Hu������
    cvDrawContours(dst, contour, cvScalar(255), cvScalar(200), 1);
    cvReleaseMemStorage(&contour->storage);
    for (int i = 0; i < 7; ++i) {
        ((double*)&hu)[i] = fabs(log(fabs(((double*)&hu)[i])));
    }
    //��ʾ7����(ȡ������Ľ��)
    if (printHu) {
        cout << setprecision(4);
        for (int i = 0; i < 7; ++i) {
            cout << ((double*)&hu)[i] << "\t";
        }
        cout << "\n";
    }
    //��ʾͼƬ
    if (show) {
        showImg(dst, "dst");
    }
    //����ͼƬ
    if (save) {
        outputImg("����.jpg", 0, dst);
    }
    if (delDst) {
        cvReleaseImage(&dst);
    }
    return (double*)&hu;
}

//����õ���Hu��ֵ
void saveHu(const double* hu, cchar* path, cchar* file = "hu.txt", int th = -1)
{
    FILE* fp = fopen(file, "a");
    FPRINTF(fp, "%-16s", path);
    for (int i = 0; i < 7; i++) {
        FPRINTF(fp, "%.2lf\t", hu[i]);
    }
    if (th != -1) {
        FPRINTF(fp, "��ֵ: %d", th);
    }
    FPRINTF(fp, "\n");
    fclose(fp);
}

//��������Hu�صľ���
double comHu(CIplImage* src, CIplImage* dst, int th = 100)
{
    return cvMatchShapes(findContour(src, 0, th), findContour(dst, 0, th), CV_CONTOURS_MATCH_I3);
}
double comHu(const CvSeq *contour1, CIplImage* dst, int th = 100)
{
    return cvMatchShapes(contour1, findContour(dst, 0, th), CV_CONTOURS_MATCH_I3);
}
double comHu(const double* h1, const double* h2)
{
    if (h1 == 0 || h2 == 0) {
        return 0;
    }
    double dif = 0;
    for (int i = 0; i < 7; i++) {
        dif += fabs(((h1[i] - h2[i])) / h1[i]);
    }
    return dif;
}

//ͨ�����ڻ���õ������ֵ
bool first;
CIplImage* p_image = NULL;
void on_trackbar(int g_thresh)     //�ص�����
{
    static IplImage* p_gray = 0;
    if (first) {
        p_gray = cvCreateImage(cvGetSize(p_image), 8, 1);
    }
    CvSeq* contours = findContour(p_image, &p_gray, g_thresh);
    cvZero(p_gray);
    if (contours) {
        cvDrawContours(p_gray, contours, cvScalarAll(255), cvScalarAll(200), 1);    //������
    }
    cvShowImage("������ֵ", p_gray);
    first = 0;
    cvReleaseImage(&p_gray);
}

int getTh(CIplImage* src)
{
    first = 1;
    p_image = src;
    int g_thresh = 100;    //��ʼ��ֵ
    cvNamedWindow("������ֵ", 1);
    cvCreateTrackbar("threshold", "������ֵ", &g_thresh, 255, on_trackbar);
    on_trackbar(g_thresh);
    cvWaitKey();
    return g_thresh;
}

//����������pair-wise����ֱ��ͼ
CvHistogram* gesCalcContoursPGH(const CvSeq* contour)
{
    CvContour* tempCont = (CvContour*)contour;//�õ��ɶԼ���ֱ��ͼ�ڶ���ά���ϵķ�Χ
    cvBoundingRect(tempCont, 1);
    int sizes[2] = { 60, 200 };
    float ranges[2][2] = { {0,3.14159f}, {0,200} };
    float** rangesPtr = new float*[2];
    rangesPtr[0] = ranges[0];
    rangesPtr[1] = ranges[1];
    //��ʼ������ֱ��ͼ
    CvHistogram* hist = cvCreateHist(2, sizes, CV_HIST_ARRAY, rangesPtr, 1);
    //���������ĳɶԼ���ֱ��ͼ
    cvCalcPGH(contour, hist);//���������� pair-wise ����ֱ��ͼ
    delete[]rangesPtr;
    return hist;
}

//��������pair-wise����ֱ��ͼ����ƥ��
double getMatchContoursPGH(const CvSeq* contour1, const CvSeq* contour2)
{
    //�õ������ĳɶԼ���ֱ��ͼ
    CvHistogram* hist1 = gesCalcContoursPGH(contour1);
    CvHistogram* hist2 = gesCalcContoursPGH(contour2);
    //��һ��ֱ��ͼ
    cvNormalizeHist(hist1, 1);
    cvNormalizeHist(hist2, 2);
    //ֱ��ͼƥ��
    double result = cvCompareHist(hist1, hist2, CV_COMP_INTERSECT);
    //�ͷ��ڴ�
    cvReleaseHist(&hist1);
    cvReleaseHist(&hist2);
    return result;
}

// ��һ��ͼ���ź���ת�任�󱣴浽��һ��ͼ��
void imRotate(CIplImage *src, IplImage *&dst, double angle, double scale = 1, CvPoint2D32f center = cvPoint2D32f(-1, -1))
{
    if (dst == 0) {
        return;
    }
    if (src->width == dst->width && src->height == dst->height
    &&src->depth == dst->depth &&src->nChannels == dst->nChannels) {
        CvMat *mapMatrix = cvCreateMat(2, 3, CV_32FC1);
        if (center.x == -1 && center.y == center.x) {
            center = cvPoint2D32f(src->width / 2, src->height / 2);
        }
        cv2DRotationMatrix(center, angle, scale, mapMatrix);//�����ά��ת�ķ���任����
        cvWarpAffine(src, dst, mapMatrix);    //��ͼ��������任
    } else
#ifndef __AFXWIN_H__
        cout << "����ͼ���ͻ���һ��,�޷�����ת��" << endl;
#else
        SPRINTF(info, "%s����ͼ���ͻ���һ��,�޷�����ת��\r\n", info);
#endif
}

//���漸������һ�����surf�ǵ��⹦�ܣ�������ƥ��Ա�����(�ⲿ����find_obj����)
//�Ƚ�surf����
double compareSURFDescriptors(const float* d1, const float* d2, double best, int length)
{
    double total_cost = 0;
    assert(length % 4 == 0);
    for (int i = 0; i < length; i += 4) {
        double t0 = d1[i] - d2[i];
        double t1 = d1[i + 1] - d2[i + 1];
        double t2 = d1[i + 2] - d2[i + 2];
        double t3 = d1[i + 3] - d2[i + 3];
        total_cost += t0*t0 + t1*t1 + t2*t2 + t3*t3;
        if (total_cost > best) {
            break;
        }
    }
    return total_cost;
}
//�ҵ���������ڽ���
int naiveNearestNeighbor(const float* vec, int laplacian, const CvSeq* model_keypoints, const CvSeq* model_descriptors)
{
    int length = (int)(model_descriptors->elem_size / sizeof(float));
    int i, neighbor = -1;
    double d, dist1 = 1e6, dist2 = 1e6;
    CvSeqReader reader, kreader;
    cvStartReadSeq(model_keypoints, &kreader, 0);
    cvStartReadSeq(model_descriptors, &reader, 0);

    for (i = 0; i < model_descriptors->total; i++) {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* mvec = (const float*)reader.ptr;
        CV_NEXT_SEQ_ELEM(kreader.seq->elem_size, kreader);
        CV_NEXT_SEQ_ELEM(reader.seq->elem_size, reader);
        if (laplacian != kp->laplacian) {
            continue;
        }
        d = compareSURFDescriptors(vec, mvec, dist2, length);
        if (d < dist1) {
            dist2 = dist1;
            dist1 = d;
            neighbor = i;
        } else if (d < dist2) {
            dist2 = d;
        }
    }
    if (dist1 < 0.6*dist2) {
        return neighbor;
    }
    return -1;
}
//Ѱ��ƥ���
void findPairs(const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, vector<int>& ptpairs)
{
    int i;
    CvSeqReader reader, kreader;
    cvStartReadSeq(objectKeypoints, &kreader);
    cvStartReadSeq(objectDescriptors, &reader);
    ptpairs.clear();

    for (i = 0; i < objectDescriptors->total; i++) {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* descriptor = (const float*)reader.ptr;
        CV_NEXT_SEQ_ELEM(kreader.seq->elem_size, kreader);
        CV_NEXT_SEQ_ELEM(reader.seq->elem_size, reader);
        int nearest_neighbor = naiveNearestNeighbor(descriptor, kp->laplacian, imageKeypoints, imageDescriptors);
        if (nearest_neighbor >= 0) {
            ptpairs.push_back(i);
            ptpairs.push_back(nearest_neighbor);
        }
    }
}
//surfƥ��
double find_obj(CIplImage* obj_color, CIplImage* img_color,
int showInfo, int showMatch, int showSurf)
{
    IplImage *object = cvCreateImage(cvGetSize(obj_color), 8, 1); //��ͨ��
    IplImage *image  = cvCreateImage(cvGetSize(img_color), 8, 1); //��ͨ��
    IplImage* object_surf = cvCreateImage(cvGetSize(object), 8, 3); //3ͨ��
    if (obj_color->nChannels != 1) {
        cvCvtColor(obj_color, object, CV_BGR2GRAY);
        cvCopy(obj_color, object_surf);
    } else {
        cvCopy(obj_color, object);
        cvCvtColor(obj_color, object_surf, CV_GRAY2BGR);
    }
    if (img_color->nChannels != 1) {
        cvCvtColor(img_color, image, CV_BGR2GRAY);
    } else {
        cvCopy(img_color, image);
    }
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* objectKeypoints = 0, *objDescriptors = 0;
    CvSeq* imageKeypoints = 0, *imageDescriptors = 0;
    int64 tt = cvGetTickCount();
    CvSURFParams params = cvSURFParams(800, 1);
    cvExtractSURF(object, 0, &objectKeypoints, &objDescriptors, storage, params);
    Sleep(1);
    cvExtractSURF(image, 0, &imageKeypoints, &imageDescriptors, storage, params);
    if (showInfo) {
#ifndef __AFXWIN_H__
        cout << "Object Descriptors: " << objDescriptors->total
        << "\nImage Descriptors: " << imageDescriptors->total << endl;
        cout << "Extraction time = "
        << (cvGetTickCount() - tt) / (cvGetTickFrequency()*1000.) << "ms\n";
#else
        SPRINTF(info, "%sObject Descriptors: %d  \tImage Descriptors: %d  \t"
        , info, objDescriptors->total, imageDescriptors->total);
        SPRINTF(info, "%sExtraction time = %gms\r\n", info
        , (cvGetTickCount() - tt) / (cvGetTickFrequency()*1000.));
#endif
    }
    int max_width = image->width > object->width ? image->width : object->width;
    int _height = object->height + image->height;
    IplImage* correspond = NULL;
    if(showMatch) {
        correspond = cvCreateImage(cvSize(max_width, _height), obj_color->depth
        , obj_color->nChannels);
        cvSetImageROI(correspond, cvRect(0, 0, object->width, object->height));
        cvCopy(object_surf, correspond);
        cvSetImageROI(correspond, cvRect(0, object->height, image->width, correspond->height));
        cvCopy(img_color, correspond);
        cvResetImageROI(correspond);
    }
    vector<int> ptpairs;
    findPairs(objectKeypoints, objDescriptors, imageKeypoints, imageDescriptors, ptpairs);
    srand((unsigned)time(NULL));
    int matchNum = 0;
    double rt_w = object->width / (image->width + .01);
    double rt_h = object->height / (image->height + .01);
    //���ֻ��ƥ���������ƴ�С��ͼ������ͨ��б�������˵�����ɵĽǵ�ƥ���
    //double error = 0.13;
    for (int i = 0; i < (int)ptpairs.size(); i += 2) {
        CvScalar cs = { rand() % 256, rand() % 256, rand() % 256 };
        CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem(objectKeypoints, ptpairs[i]);
        CvSURFPoint* r2 = (CvSURFPoint*)cvGetSeqElem(imageKeypoints, ptpairs[i + 1]);
        //if (fabs(r1->pt.x / (r2->pt.x + .01) - rt_w) < error*rt_w \
        &&fabs(r1->pt.y / (r2->pt.y + .01) - rt_h) < error*rt_h)
        {
            if (showMatch) //correspond ->find_obj_match
                cvLine(correspond, cvPointFrom32f(r1->pt), cvPoint(cvRound(r2->pt.x),
                cvRound(r2->pt.y + object->height)), cs);
            matchNum++;
        }
    }
    if (showSurf) {
        CvScalar red = { 0,0,255 };
        for (int i = 0; i < objectKeypoints->total; i++) {
            CvSURFPoint* r = (CvSURFPoint*)cvGetSeqElem(objectKeypoints, i);
            CvPoint center = { cvRound(r->pt.x), cvRound(r->pt.y) };
            int radius = cvRound(r->size*1.2 / 9. * 2);
            cvCircle(object_surf, center, radius, red, 1, 8, 0);
        }
        cvNamedWindow("Object", 1);
        cvShowImage("Object", object_surf);
    }
    int totalNum = objDescriptors->total < imageKeypoints->total ?
    objDescriptors->total : imageKeypoints->total;
    double ratio = pow((double)matchNum / totalNum, 1.0 / 9);
    if (showInfo)
#ifndef __AFXWIN_H__
        cout << "match : " << matchNum << "(" << totalNum << ")" << endl << "ratio: " << ratio << endl;
#else
        SPRINTF(info, "%smatch : %d\tratio: %g\r\n", info, matchNum, ratio);
#endif
    if (showMatch) {
        cvNamedWindow("Object Correspond", 1);
        cvShowImage("Object Correspond", correspond); //->find_obj_match
    }
    cvWaitKey(0);
    if (showSurf) {
        cvDestroyWindow("Object");
    }
    if (showMatch) {
        cvDestroyWindow("Object Correspond");
    }
    releaseImg(&object_surf);
    releaseImg(&correspond);
    releaseImg(&object);
    releaseImg(&image);
    if (showMatch) {
        releaseImg(&correspond); //->find_obj_match
    }
    cvReleaseMemStorage(&storage);
#ifndef __AFXWIN_H__
    cout << "surf�ǵ�ƥ����Ϊ:\t" << ratio << endl;
#else
    SPRINTF(info, "%ssurf�ǵ�ƥ����Ϊ:\t%g\r\n", info, ratio);
#endif
    return ratio;
}

double find_obj(cchar* imgName, cchar* objName, int showInfo, int showMatch, int showSurf)
{
#ifndef __AFXWIN_H__
    IplImage* object = loadImg(objName, 0, 0, 1, showInfo); //��ȡ��ͼ,��֤��ɫ��ʾ
    IplImage* image = loadImg(imgName, 0, 0, 1, showInfo);  //��ȡ��ͼ,��֤��ɫ��ʾ
#else
    IplImage* object = loadImg(objName, 0, 0, 0, 0);
    IplImage* image = loadImg(imgName, 0, 0, 0, 0);
#endif
    if (!object || !image) {
#ifndef __AFXWIN_H__
        FPRINTF(stderr, "Can not load %s and/or %s\n", objName, imgName);
        exit(-1);
#else
        SPRINTF(info, "%sCan not load %s and/or %s\r\n", info, objName, imgName);
        return -1;
#endif
    }
    double ratio = find_obj(object, image, showInfo, showMatch, showSurf);
    releaseImg(&object);
    releaseImg(&image);
    return ratio;
}

//surf�ǵ��Ⲣ���
IplImage* surfDetector(CIplImage* img, cchar* saveSurf = "")
{
    CvMemStorage* storage = cvCreateMemStorage(0);
    cvNamedWindow("Image");
    IplImage* img1 = cvCreateImage(cvSize(img->width, img->height), img->depth, 3);
    cvCopy(img, img1);
    IplImage* image = cvCreateImage(cvSize(img->width, img->height), 8, 1);
    cvCvtColor(img1, image, CV_BGR2GRAY);
    CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
    CvSURFParams params = cvSURFParams(800, 1);
    double tt = (double)cvGetTickCount();
    cvExtractSURF(image, 0, &imageKeypoints, &imageDescriptors, storage, params);
#ifndef __AFXWIN_H__
    cout << "Image Descriptors: " << imageDescriptors->total << endl;
#else
    SPRINTF(info, "%sImage Descriptors: %d\r\n", info, imageDescriptors->total);
#endif
    CvSURFPoint* r;
    for (int i = 0; i < imageKeypoints->total; i++) {
        r = (CvSURFPoint*)cvGetSeqElem(imageKeypoints, i);
        CvPoint center;
        center.x = cvRound(r->pt.x);
        center.y = cvRound(r->pt.y);
        int radius = cvRound(r->size*1.2 / 9 * 2);
        cvCircle(img1, center, radius, cvScalar(0, 0, 255), 1, 8, 0);
    }
#ifndef __AFXWIN_H__
    cout << "last time is " << (cvGetTickCount() - tt) / (cvGetTickFrequency()*1000.) << "ms\n";
#else
    SPRINTF(info, "%slast time is %gms\r\n", info, (cvGetTickCount() - tt) / (cvGetTickFrequency()*1000.));
#endif
    if (saveSurf[0]) {
        saveImg(saveSurf, img1);
    }
    cvShowImage("Image", img1);
    cvWaitKey(-1);
    cvDestroyWindow("Image");
    releaseImg(&image);
    cvReleaseMemStorage(&storage);
    return img1;
}

//���رȱȽ���
class WM
{
    int _n_, _m_, each_part, tran, th_, width, height;
    vector<uchar> data,gray; //������ȡ�����ģ��, �Ҷ�����
public:
    WM(int n = 20, int m = 20) {
        _n_ = n;
        _m_ = m;
        each_part = tran = 0;
    }
    // ���������ݻҶȻ�+��ֵ������뵽һ������������
    int saveToArr(CIplImage * img, int th = 180) {
        if (img) {
            width = img->width;
            height = img->height;
        } else {
            MessageBox(0, "�������ر����ͼ��ָ������", "���رȼ���ʧ��", 0);
            return 0;
        }
        if (th > 0 && th < 255) {
            th_ = th;
        }
        gray.resize(img->height * img->width, 0);
        uchar* data = (uchar*)img->imageData;
        for (int y = 0; y < img->height; y++)
            for (int x = 0, yy = y * img->widthStep; x < img->width; x++) {
                if (img->nChannels == 3) {
                    gray[y*img->width + x] = (data[yy + 3 * x + 2] + data[yy + 3 * x + 1] + data[yy + 3 * x] + 1) / 3;
                } else if (img->nChannels == 1) {
                    gray[y*img->width + x] = data[yy + x];
                }
                if (gray[y*img->width + x] > th_) {
                    gray[y*img->width + x] = 255;
                }
            }
        return 1;
    }
    //��������֮��3*3����ƽ���������Ҷ�ֱ��ͼ���Ҷ�ֵV��max��R,G,B��,��ԭͼ����ֵ���д���
    void binaryzation(int a, int b, int KY) {
        //Ѱ�ҵ�
        int mat = 0;
        for (int i = 0; i < each_part; i++)
            for (int j = 0; j < tran; j++) {
                int zz = a*each_part*width + b*tran + i*width + j;
                if (gray[zz] > KY) { //��ʱ�Ϊ�Ǳʼ�
                    gray[zz] = 255;
                } else {          //�ر���ͳ����Ŀ
                    mat++;
                }
            }
    }
    //ͳ�Ʊʼ�����Ŀ
    int mark(int a, int b) {
        int mat = 0;
        for (int i = 0; i < each_part; i++)
            for (int j = 0; j < tran; j++)
                if (gray[a*each_part*width + b*tran + i*width + j] != 255) {
                    mat++;
                }
        return mat;
    }
    //Otsu�����䷽�
    template<class T1>
    int Otsu(T1& arr, int size, int size2) {
        double histogram[256] = { 0 };
        int i, threshold;
        for (i = 0; i < size; i++)
            if (arr[i] <= th_) {
                histogram[arr[i]]++;
            }
        for (i = 0; i < th_; i++)
            if (histogram[i]>.99) {
                histogram[i] /= size2;
            }
        double avgValue = 0, maxVariance = 0, w = 0, u = 0;
        for (i = 0; i < th_; i++) {
            avgValue += i * histogram[i];
        }
        for (i = 0; i < th_; i++) {
            w += histogram[i];
            u += i * histogram[i];
            double t = avgValue * w - u;
            double variance = t * t / (w * (1 - w));
            if (variance > maxVariance) {
                maxVariance = variance;
                threshold = i;
            }
        }
        return threshold;
    }
    //����Ԥ����
    void Outline_pretreatment(vector<uchar> &_data, int a, int b, float *res) {
        int before = mark(a, b);
        if (before == 0) {
            res[a*_m_ + b] = 0;
            return;
        }
        int KT = Otsu(_data, each_part*tran, before);
        binaryzation(a, b, KT); //��ѡ�еĲ��ֽ��о����ֵ������
        int after = mark(a, b);
        if (after == 0) {
            res[a*_m_ + b] = 0;
        } else {
            res[a*_m_ + b] = (float)after / before;
        }
    }
    //�ֶ�ƥ��
    void subsection_mactching(float *res) {
        each_part = height / _n_; //ÿһ���м���
        tran = width / _m_; //������¼����ķֶ�
        data.resize(gray.size(), 0);
        for (int a = 0; a < _n_; a++)
            for (int b = 0; b < _m_; b++) {
                for (int i = 0; i < each_part; i++)
                    for (int j = 0; j < tran; j++) {
                        data[i*tran + j] = gray[a*each_part*width + b*tran + i*width + j];
                    }
                Outline_pretreatment(data, a, b, res);
            }
    }

    void matching(float *res, float *res2, double* mw) {
        int k, j, nm = _n_*_m_ / 4;
        for (j = 0; j < 4; j++) {
            mw[j] = 0;
        }
        for (k = 0; k < _n_ / 2; k++)
            for (j = 0; j<_m_ / 2; j++)
                if (fabs(res[j*_n_ + k] - res2[j*_n_ + k])>0.3) {
                    mw[0]++;
                }
        for (k = _n_ / 2; k < _n_; k++)
            for (j = 0; j<_m_ / 2; j++)
                if (fabs(res[j*_n_ + k] - res2[j*_n_ + k])>0.3) {
                    mw[1]++;
                }
        for (k = 0; k < _n_ / 2; k++)
            for (j = _m_ / 2; j<_m_; j++)
                if (fabs(res[j*_n_ + k] - res2[j*_n_ + k])>0.3) {
                    mw[2]++;
                }
        for (k = _n_ / 2; k < _n_; k++)
            for (j = _m_ / 2; j<_m_; j++)
                if (fabs(res[j*_n_ + k] - res2[j*_n_ + k])>0.3) {
                    mw[3]++;
                }
        for (j = 0; j < 4; j++) {
            mw[j] = 1 - mw[j] / nm;
        }
    }
};

void matchWeight(CIplImage* img1, CIplImage* img2, double* mw, int show, int n, int m)
{
    WM wm1(n, m), wm2(n, m);
    float res[2][10000] = { 0 };
    if (!wm1.saveToArr(img1)) {
        return;
    }
    wm1.subsection_mactching(res[0]);
    Sleep(1);
    if (!wm2.saveToArr(img2)) {
        return;
    }
    wm2.subsection_mactching(res[1]);
    wm1.matching(res[0], res[1], mw);
#ifndef __AFXWIN_H__
    cout << "���ر����Ƹ���Ϊ :\t" << mw[0] << "," << mw[1] << "," << mw[2] << "," << mw[3] << endl;
#else
    SPRINTF(info, "%s���ر����Ƹ���Ϊ :\t%g,%g,%g,%g\r\n", info, mw[0], mw[1], mw[2], mw[3]);
#endif
}

#ifdef __AFXWIN_H__
//����ǰ��Ļ���浽�ļ�
void Screen(char *filename)
{
    CString s;
    if (filename == 0 || filename[0] == 0) {
        srand(unsigned(time(0)));
        int a = rand() % 9000 + 1000, b = rand() % 9000 + 1000;
        s.Format("%d%d", a, b);
        filename = s.GetBuffer();
    }
    CDC *pDC;//��ĻDC
    pDC = CDC::FromHandle(::GetDC(NULL));//��ȡ��ǰ������ĻDC
    int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//�����ɫģʽ
    int Width = pDC->GetDeviceCaps(HORZRES);
    int Height = pDC->GetDeviceCaps(VERTRES);

    CDC memDC;//�ڴ�DC
    memDC.CreateCompatibleDC(pDC);

    CBitmap memBitmap, *oldmemBitmap;//��������Ļ���ݵ�bitmap
    memBitmap.CreateCompatibleBitmap(pDC, Width, Height);
    oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC
    memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC

    //���´��뱣��memDC�е�λͼ���ļ�
    BITMAP bmp;
    memBitmap.GetBitmap(&bmp);//���λͼ��Ϣ
    FILE *fp = fopen(filename, "w+b");

    BITMAPINFOHEADER bih = { 0 };//λͼ��Ϣͷ
    bih.biBitCount = bmp.bmBitsPixel;//ÿ�������ֽڴ�С
    bih.biCompression = BI_RGB;
    bih.biHeight = bmp.bmHeight;//�߶�
    bih.biPlanes = 1;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//ͼ�����ݴ�С
    bih.biWidth = bmp.bmWidth;//���

    BITMAPFILEHEADER bfh = { 0 };//λͼ�ļ�ͷ
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//��λͼ���ݵ�ƫ����
    bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//�ļ��ܵĴ�С
    bfh.bfType = (WORD)0x4d42;

    fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//д��λͼ�ļ�ͷ
    fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//д��λͼ��Ϣͷ
    byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//�����ڴ汣��λͼ����
    GetDIBits(memDC.m_hDC, (HBITMAP)memBitmap.m_hObject, 0, Height, p, (LPBITMAPINFO)&bih, DIB_RGB_COLORS);//��ȡλͼ����
    fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//д��λͼ����
    delete[] p;
    fclose(fp);
    memDC.SelectObject(oldmemBitmap);
}
#endif

#ifdef _MSC_VER //_MSC_VER
#define LOG2(x) log(x)/log(2)
#else
#define LOG2(x) log10(x)/log10(2)
#endif // _MSC_VER

//�Զ�ֵ�����ͼ������Ϣ��
//imgΪ����ͼ��,w��hΪ��������Ŀ��,����ǰ����Ǽܵ���С����
float Entropy(CIplImage* img, int th, int w, int h)
{
    int count = 0;
    CvSize size, sizeImg = cvGetSize(img);;
    //��������˿�߲�����ʹ�ô��ݵĲ���--һ��Ϊ�ùǼܵ���С����
    if (w > 0 && h > 0) {
        size = cvSize(w, h);
    } else {
        size = cvGetSize(img);
    }
    for (int i = 0; i < sizeImg.height; ++i)
        for (int j = 0; j < sizeImg.width; ++j)
            if (CV_IMAGE_ELEM(img, uchar, i, j) <= th) {
                count++;    //ͳ��ǰ�����ص����
            }
    float ratio1 = (float)count / size.width / size.height;
    //������Ϣ��:H(S) = -p0*log2(p0)-p1*log2(p1);
    return float(-ratio1*LOG2(ratio1) - (1 - ratio1)*LOG2(1 - ratio1));
}

// ������ͼ�����ϸ��
//maxIterations���Ƶ�������,Ĭ��Ϊ-1,�������Ƶ�������,ֱ��������ս��
void Hilditch(CIplImage* src, IplImage* dst, int th, int maxIterations)
{
    int tt = clock();
    int noDst = 0;
    if (!dst) {
        dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
        noDst = 1;
    }
    //��ԭͼ��ת��Ϊ��ֵͼ���ּ�����ʱ��Ҫ�����ֵ��
    cvThreshold(src, dst, th, 1, CV_THRESH_BINARY_INV);
    int count = 0;  //��¼��������
    CvSize size = cvGetSize(src);
    while (1) {
        if (clock() - tt > 50) {
            Sleep(1);
            tt = clock();
        }
        count++;
        if (maxIterations != -1 && count > maxIterations) { //���Ƶ�������
            break;
        }
        vector<pair<int, int> > mFlag; //���ڱ����Ҫɾ���ĵ�
        //�Ե���
        for (int i = 0; i < size.height; ++i) {
            for (int j = 0; j < size.width; ++j) {
                //��������ĸ����������б��
                //  p[8] p[1] p[2]
                //  p[7] p[0] p[3]
                //  p[6] p[5] p[4]
                int p[9];
                p[0] = CV_IMAGE_ELEM(dst, uchar, i, j);//ȡ��[i][j]��Ӧ��ֵ��ǰ��Ϊ1������Ϊ0
                if (!p[0]) {
                    continue;
                }
                p[1] = (i == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j);
                p[2] = (i == 0 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j + 1);
                p[3] = (j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j + 1);
                p[4] = (i == size.height - 1 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j + 1);
                p[5] = (i == size.height - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j);
                p[6] = (i == size.height - 1 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j - 1);
                p[7] = (j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j - 1);
                p[8] = (i == 0 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j - 1);
                //����1:��֤p0��8������Ϊǰ�������ص������2~6֮�䣬��p1��Ϊ�˵�
                int pTotal = p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7] + p[8];
                if (pTotal >= 2 && pTotal <= 6) {
                    int ap = 0;
                    for (int k = 1; k < 8; ++k)
                        if (p[k] == 0 && p[k + 1]) {
                            ++ap;
                        }
                    if (p[8] == 0 && p[1]) {
                        ++ap;
                    }
                    //����2:p0��˳ʱ�뷽����Χ�����ص����ɱ�����ǰ���Ĵ���Ϊ1
                    if (ap == 1 && p[1] * p[3] * p[5] == 0 && p[3] * p[5] * p[7] == 0) {
                        mFlag.push_back(make_pair(i, j));    //���
                    }
                }
            }
        }

        //����ǵĵ�ɾ��
        for (vector<pair<int, int> >::iterator i = mFlag.begin(); i != mFlag.end(); ++i) {
            CV_IMAGE_ELEM(dst, uchar, i->first, i->second) = 0;
        }
        //ֱ��û�е����㣬�㷨����
        if (mFlag.size() == 0) {
            break;
        } else {
            mFlag.clear();    //��mFlag���
        }

        //�ڶ��ζԵ���
        for (int i = 0; i < size.height; ++i) {
            for (int j = 0; j < size.width; ++j) {
                //����������������б��
                //  p[8] p[1] p[2]
                //  p[7] p[0] p[3]
                //  p[6] p[5] p[4]
                int p[9];
                p[0] = CV_IMAGE_ELEM(dst, uchar, i, j);//ȡ��[i][j]��Ӧ��ֵ��ǰ��Ϊ1������Ϊ0
                if (p[0] != 1) {
                    continue;
                }
                p[1] = (i == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j);
                p[2] = (i == 0 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j + 1);
                p[3] = (j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j + 1);
                p[4] = (i == size.height - 1 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j + 1);
                p[5] = (i == size.height - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j);
                p[6] = (i == size.height - 1 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j - 1);
                p[7] = (j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j - 1);
                p[8] = (i == 0 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j - 1);

                //����1:��֤p0��8������Ϊǰ�������ص������2~6֮�䣬��p1��Ϊ�˵�
                int pTotal = p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7] + p[8];
                if (pTotal >= 2 && pTotal <= 6) {
                    int ap = 0;
                    for (int k = 1; k < 8; ++k)
                        if (p[k] == 0 && p[k + 1]) {
                            ++ap;
                        }
                    if (p[8] == 0 && p[1]) {
                        ++ap;
                    }
                    //����2:p0��˳ʱ�뷽����Χ�����ص����ɱ�����ǰ���Ĵ���Ϊ1
                    if (ap == 1 && p[1] * p[3] * p[7] == 0 && p[1] * p[5] * p[7] == 0) {
                        mFlag.push_back(make_pair(i, j));    //���
                    }
                }
            }
        }
        //����ǵĵ�ɾ��
        for (vector<pair<int, int> >::iterator i = mFlag.begin(); i != mFlag.end(); ++i) {
            CV_IMAGE_ELEM(dst, uchar, i->first, i->second) = 0;
        }

        //ֱ��û�е����㣬�㷨����
        if (mFlag.size() == 0) {
            break;
        } else {
            mFlag.clear();    //��mFlag���
        }
    }
    if (noDst) {
        releaseImg(&dst);
    } else {
        for (int i = 0; i < size.height; ++i)
            for (int j = 0; j < size.width; ++j) {
                if (CV_IMAGE_ELEM(dst, uchar, i, j)) {
                    CV_IMAGE_ELEM(dst, uchar, i, j) = CV_IMAGE_ELEM(src, uchar, i, j);
                } else {
                    CV_IMAGE_ELEM(dst, uchar, i, j) = 255;
                }
            }
    }
}

//��ֵ�˲�---//δ���
void midFilter(IplImage* src)
{
    int va[9];
    uchar* data = (uchar*)src->imageData;
    if (src->nChannels == 3) {
        for (int y = 0; y < src->height; y++)
            for (int x = 0, yy = y * src->widthStep; x < src->width; x++) {
                data[yy + 3 * x];
            }
    } else if (src->nChannels == 1) {
        for (int y = 0; y < src->height; y++)
            for (int x = 0, yy = y * src->widthStep; x < src->width; x++) {
                data[yy + x];
            }
    }
    sort(va, va + 8);
    //if(RGB_v(i,j)==va[0] || RGB_v(i,j)==va[8])
    //RGB_v(i,j)=va[4];
}
