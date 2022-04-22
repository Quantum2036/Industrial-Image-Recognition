#include "pch.h"
#include "Trainer.h"
#include "Target.h"

int main(int argc, char** argv) {

	Mat bgImg = imread(DATAFOLDER"pattern.pgm", 0);
	Mat tImg  = imread(DATAFOLDER"test2.pgm", 0);

	//Mat bgImg = imread(DATAFOLDER"bg.jpg");
	//Mat tImg = imread(DATAFOLDER"item.jpg");
	//resize(bgImg, bgImg, Size(), 0.25, 0.25);
	//resize(tImg, tImg, Size(), 0.25, 0.25);

	//TClassifier	theClassifier;

	/*theClassifier.LoadClassifier(DATAFOLDER"Target_Classifier.txt");
	theClassifier.PrintTClassifier_d();*/

	Trainer tran(bgImg, DATAFOLDER"nut", "pgm");
	/*CVApp theApp;
	theApp.AddBackground(bgImg);
	theApp.AddTestImg(tImg);
	theApp.ShowTatget();
	theApp.ShowAll_d();

	theApp.AddClassifier(theClassifier);*/

	return 0;
}
