#include "pch.h"
#include "Trainer.h"
#include "Target.h"

int main(int argc, char** argv) {

	Mat bgImg = imread(DATAFOLDER"pattern.pgm", 0);
	Mat tImg  = imread(DATAFOLDER"test.pgm", 0);
	TClassifier	theClassifier;

	theClassifier.LoadClassifier(DATAFOLDER"Target_Classifier.txt");
	//theClassifier.PrintTClassifier_d();

	//训练得到分类器
	//Trainer tran_1(bgImg, DATAFOLDER"nut", "pgm");
	//tran_1.SaveTClassifier("nut");
	//
	//Trainer tran_2(bgImg, DATAFOLDER"ring", "pgm");
	//tran_2.SaveTClassifier("ring");
	//
	//Trainer tran_3(bgImg, DATAFOLDER"screw", "pgm");
	//tran_3.SaveTClassifier("screw");

	CVApp theApp;
	theApp.AddBackground(bgImg);
	theApp.AddTestImg(tImg);
	theApp.AddClassifier(theClassifier);
	theApp.ShowTatget();
	theApp.ShowAll_d();

	return 0;
}
