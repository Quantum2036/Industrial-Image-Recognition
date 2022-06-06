#pragma once
#include "pch.h"
#include "Target.h"
#include <vector>
#include "TClassifier.h"

//����ʵ�ֶ�ͼ���ʶ��
class CVApp
{
//	���� & ����
public:
	CVApp();
	CVApp(Mat& test, Mat& background);

//	����
private:
	//Ŀ�꼯��
	std::vector<Target> target;

	//����ͼ�񼯺�
	std::vector<Mat> testImgs;

	//����ͼ�񼯺�
	std::vector<Mat> bgImgs;

	//����ͼ��Ĵ�С
	Size sizeImg;

	//������
	TClassifier* pClassifier;

	//window name
	static String strWinName_Test[];
	static String strWinName_bg[];

//	����
public:

	//��Ӳ���ͼ�񣬲��õ��Ҷ�ͼ
	void AddTestImg(Mat& tImg);

	//��ӱ���ͼ�񣬲��õ��Ҷ�ͼ
	void AddBackground(Mat& bgImg);

	//��ӷ�����
	void AddClassifier(TClassifier& classifier) {
		pClassifier = &classifier;
		if (pClassifier == nullptr) {
		}
	}

	//�ڻ�������ʾ�������
	//	��Ŀ�����ܻ����߿������
	void ShowTatget(void);

	//�Ľ��ĵȴ���꺯��
	void WaitESC(void);

	//�������ͼ�񡪡�����������
	//	cb = true ʱ�������ص�����
	void ShowWindows(bool cb = false);

	//ɨ��ͼ���ҳ����ܵĶ���
	//	Ĭ��ɨ��ģʽΪ����ɨ��
	size_t Scan(ScanMode mode = ScanMode::SMode_grid);

	std::vector<Target>* getTargets(void) {
		return &target;
	}

private:

	//��������Ҫ��Ĳ���ͼ��ͱ���ͼ�����еĺ���
	void PostProcessing(void);

	//������ʱ�Ĳ�ɫͼ�����еĻ���ת���ɻҶ�ͼ��
	//	�����ڹ���������������
	Mat ToGray(Mat& Img);
	
	//���Ҷ�ͼ������ת����3ͨ��BGRͼ��
	Mat GrayToBGR(Mat& Img);

	//������ͼ���뱳��ͼ�����,���ض�ֵͼ��
	//	�������ֵΪdif
	//	�����ڹ���������������
	Mat MinusMat_1(uchar dif = 35);

	//������ͼ���뱳��ͼ�����,���ػҶ�ͼ��
	//	�������ֵΪ images[4] ��������ֵ
	//	�����ڹ���������������
	Mat MinusMat_2(void);

	//������ͼ���뱳��ͼ�����,���ز�ɫͼ���紫��testͼ���ǣ�
	//	�������ֵΪ images[4] ��������ֵ
	//	�����ڹ���������������
	Mat MinusMat_3(void);

	//�Ż��ָ��Ķ�ֵͼ�񣬿ɸĽ�
	//	Subprocess_1: �Ƚ���������ȥ�����������С�� MIN_TARGRT_SIZE �ĵ�
	//	Subprocess_2: Ȼ�������̬ѧ����
	//	�����ڹ���������������
	void ProBinarization(void);
	void ProBinarization_Subprocess_1(void);
	void ProBinarization_Subprocess_1_OnGrow(FList& tlist, uint nX, uint nY, uchar flag, uchar& count);
	void ProBinarization_Subprocess_1_ApplyList(FList& tlist);
	void ProBinarization_Morph(void);

	//���ɨ��
	void Scan_Random(void);

	//����ɨ�裬�������ں� MIN_SCAN_GAP �ж���
	void Scan_Grid(void);

	//���ɨ��
	void Scan_Pixel(void);

	//Ϊ����ͼƬ������ʾ���ڣ����ƶ�������λ��
	void CreateWindows(void);

	//�ж�Ŀ����Ƿ����Ŀ�꼯����
	bool IsExistTarget(Point pt);

	//�ж�����ͼ��ĳߴ��Ƿ���ͬ
	bool IsSameRect(Mat* Img_1, Mat* Img_2) {
		return (Img_1->cols == Img_2->cols && Img_1->rows == Img_2->rows);
	}
};