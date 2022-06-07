#include "TargetDisplay.h"
#include <time.h>
const Scalar TargetDisplay::red		= Scalar(0, 0, 255);
const Scalar TargetDisplay::green	= Scalar(0, 255, 0);
const Scalar TargetDisplay::orange	= Scalar(51, 153, 255);
const Scalar TargetDisplay::black20	= Scalar(204, 204, 204);
const Scalar TargetDisplay::black50 = Scalar(128, 128, 128);


TargetDisplay::TargetDisplay()
{
	canvas = nullptr;
}

TargetDisplay::TargetDisplay(Mat* pImgCanvas)
{
	canvas = pImgCanvas;
}

void TargetDisplay::DrawList(FList list, Scalar color)
{
	for (auto it = list.begin(); it != list.end(); it++) {
		canvas->at<Vec3b>(*it)[0] = (uchar)color.val[0];
		canvas->at<Vec3b>(*it)[1] = (uchar)color.val[1];
		canvas->at<Vec3b>(*it)[2] = (uchar)color.val[2];
	}
}

void TargetDisplay::DrawInside(Target& obj, Scalar color_inside)
{
	DrawList(obj.tlist, color_inside);
}

void TargetDisplay::DrawPeripheral(Target& obj, Scalar color_peripheral)
{
	DrawList(obj.plist, color_peripheral);
}

void TargetDisplay::DrawCross(Target& obj)
{
	constexpr int cross_radius = 10;
	int centre_X = obj.centre.x;
	int centre_Y = obj.centre.y;
	
	for (int i = centre_X - cross_radius; i < centre_X + cross_radius; i++) {
		Reverse(Point(i, centre_Y));
	}

	for (int j = centre_Y - cross_radius; j < centre_Y + cross_radius; j++) {
		Reverse(Point(centre_X,j));
	}
}

void TargetDisplay::DrawBox(Target& obj, Scalar color)
{
	Rect box = obj.rect_Box;
	rectangle(*canvas, box, color);
}

void TargetDisplay::DrawText(Target& obj, String name, Scalar color)
{
	Rect box = obj.rect_Box;
	putText(*canvas,
		name,
		Point(box.x, box.y - 2),
		FONT_HERSHEY_PLAIN, 1.0,
		color);
}

void TargetDisplay::Reverse(Point pt)
{
	canvas->at<Vec3b>(pt)[0] = 255 - canvas->at<Vec3b>(pt)[0];
	canvas->at<Vec3b>(pt)[1] = 255 - canvas->at<Vec3b>(pt)[1];
	canvas->at<Vec3b>(pt)[2] = 255 - canvas->at<Vec3b>(pt)[2];	
}

Scalar TargetDisplay::Random_Color(void)
{
	//随机化
	time_t curTime;
	time(&curTime);
	static int randNum = 0;
	RNG rng = RNG(curTime + randNum);
	randNum = (int)rng.uniform(0x00000000, 0xFFFFFFFF);		//为下次随机化添加随机因素

	return Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
}
