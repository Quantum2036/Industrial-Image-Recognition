#include "pch.h"
#include "Trainer.h"
#include "Target.h"

int main(int argc, char** argv) {

	Mat bgImg = imread(DATAFOLDER"pattern.pgm", 0);
	Mat tImg  = imread(DATAFOLDER"test.pgm", 0);


	////训练得到分类器
	//Trainer tran_1(bgImg, DATAFOLDER"nut", "pgm");
	//tran_1.SaveTClassifier("nut");
	//
	//Trainer tran_2(bgImg, DATAFOLDER"ring", "pgm");
	//tran_2.SaveTClassifier("ring");

	//Trainer tran_3(bgImg, DATAFOLDER"screw", "pgm");
	//tran_3.SaveTClassifier("screw");


	//使用保存的分类器
	TClassifier	theClassifier;
	theClassifier.LoadClassifier(DATAFOLDER"Target_Classifier.txt");
	theClassifier.PrintTClassifier_d();


	CVApp theApp;
	theApp.AddBackground(bgImg);			//添加背景图片
	theApp.AddTestImg(tImg);				//添加待测试图片
	theApp.AddClassifier(theClassifier);	//添加分类器
	theApp.ShowTatget();
	theApp.ShowWindows();

	return 0;
}
