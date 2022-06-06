#pragma once
#include "pch.h"
#include "Target.h"
#include <vector>
#include "TClassifier.h"

//完整实现对图像的识别
class CVApp
{
//	构造 & 析构
public:
	CVApp();
	CVApp(Mat& test, Mat& background);

//	数据
private:
	//目标集合
	std::vector<Target> target;

	//测试图像集合
	std::vector<Mat> testImgs;

	//背景图像集合
	std::vector<Mat> bgImgs;

	//测试图像的大小
	Size sizeImg;

	//分类器
	TClassifier* pClassifier;

	//window name
	static String strWinName_Test[];
	static String strWinName_bg[];

//	方法
public:

	//添加测试图像，并得到灰度图
	void AddTestImg(Mat& tImg);

	//添加背景图像，并得到灰度图
	void AddBackground(Mat& bgImg);

	//添加分类器
	void AddClassifier(TClassifier& classifier) {
		pClassifier = &classifier;
		if (pClassifier == nullptr) {
		}
	}

	//在画布上显示处理过程
	//	在目标外周画出边框和文字
	void ShowTatget(void);

	//改进的等待鼠标函数
	void WaitESC(void);

	//输出所有图像————测试用
	//	cb = true 时启用鼠标回调函数
	void ShowWindows(bool cb = false);

	//扫描图像，找出可能的对象，
	//	默认扫描模式为网格扫描
	size_t Scan(ScanMode mode = ScanMode::SMode_grid);

	std::vector<Target>* getTargets(void) {
		return &target;
	}

private:

	//添加完符合要求的测试图像和背景图像后进行的后处理
	void PostProcessing(void);

	//将构造时的彩色图像（如有的话）转换成灰度图像
	//	仅供在构造过程中有序调用
	Mat ToGray(Mat& Img);
	
	//将灰度图像完整转换成3通道BGR图像
	Mat GrayToBGR(Mat& Img);

	//将输入图像与背景图像相减,返回二值图像
	//	容许的阈值为dif
	//	仅供在构造过程中有序调用
	Mat MinusMat_1(uchar dif = 35);

	//将输入图像与背景图像相减,返回灰度图像
	//	容许的阈值为 images[4] 中所用阈值
	//	仅供在构造过程中有序调用
	Mat MinusMat_2(void);

	//将输入图像与背景图像相减,返回彩色图像（如传入test图像是）
	//	容许的阈值为 images[4] 中所用阈值
	//	仅供在构造过程中有序调用
	Mat MinusMat_3(void);

	//优化分割后的二值图像，可改进
	//	Subprocess_1: 先进行生长，去除掉所有面积小于 MIN_TARGRT_SIZE 的点
	//	Subprocess_2: 然后进行形态学处理
	//	仅供在构造过程中有序调用
	void ProBinarization(void);
	void ProBinarization_Subprocess_1(void);
	void ProBinarization_Subprocess_1_OnGrow(FList& tlist, uint nX, uint nY, uchar flag, uchar& count);
	void ProBinarization_Subprocess_1_ApplyList(FList& tlist);
	void ProBinarization_Morph(void);

	//随机扫描
	void Scan_Random(void);

	//网格扫描，网格间隔在宏 MIN_SCAN_GAP 中定义
	void Scan_Grid(void);

	//逐点扫描
	void Scan_Pixel(void);

	//为所有图片创建显示窗口，并移动到合适位置
	void CreateWindows(void);

	//判断目标点是否存在目标集合中
	bool IsExistTarget(Point pt);

	//判断两幅图像的尺寸是否相同
	bool IsSameRect(Mat* Img_1, Mat* Img_2) {
		return (Img_1->cols == Img_2->cols && Img_1->rows == Img_2->rows);
	}
};