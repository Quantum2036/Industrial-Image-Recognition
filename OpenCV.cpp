#include "pch.h"
#include "Trainer.h"
#include "Target.h"

//分类器路径
constexpr char CLASSIFIER_PATH[] = "C:\\量子泡沫\\Study\\数字图像处理\\data\\Target_Classifier.txt";

int main(int argc, char** argv) {

	Mat bgImg = imread(DATAFOLDER"pattern.pgm", 0);	//读背景图像
	Mat tImg  = imread(DATAFOLDER"test2.pgm", 0);	//读入测试图像


	//训练得到分类器
	/*Trainer tran_1(bgImg, DATAFOLDER"nut", "pgm");
	tran_1.SaveTClassifier("nut", CLASSIFIER_PATH);
	
	Trainer tran_2(bgImg, DATAFOLDER"ring", "pgm");
	tran_2.SaveTClassifier("ring", CLASSIFIER_PATH);
	
	Trainer tran_3(bgImg, DATAFOLDER"screw", "pgm");
	tran_3.SaveTClassifier("screw", CLASSIFIER_PATH);*/


	//使用保存的分类器
	TClassifier	theClassifier;
	theClassifier.LoadClassifier(DATAFOLDER"Target_Classifier.txt");


	CVApp theApp;
	theApp.AddBackground(bgImg);			//添加背景图片
	theApp.AddTestImg(tImg);				//添加待测试图片
	theApp.AddClassifier(theClassifier);	//添加分类器
	theApp.ShowTatget();					//在输出窗口上画出目标相关的信息
	theApp.ShowWindows();					//显示全部输出窗口

	return 0;
}
