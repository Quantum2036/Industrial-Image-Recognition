#include "TargetDisplay.h"

TargetDisplay::TargetDisplay()
{
	canvas = nullptr;
}

TargetDisplay::TargetDisplay(Mat* pImgCanvas)
{
	canvas = pImgCanvas;
}

void TargetDisplay::DrawList(std::vector<Point> list, Scalar color)
{
	for (auto it = list.begin(); it < list.end(); it++) {
		canvas->at<Vec3b>(*it)[0] = (uchar)color.val[0];
		canvas->at<Vec3b>(*it)[1] = (uchar)color.val[1];
		canvas->at<Vec3b>(*it)[2] = (uchar)color.val[2];
	}
}

void TargetDisplay::DrawInside(Target& obj, Scalar color_inside)
{
	SetColor(obj.tlist, color_inside);
}

void TargetDisplay::DrawPeripheral(Target& obj, Scalar color_peripheral)
{
	SetColor(obj.plist, color_peripheral);
}

void TargetDisplay::DrawCross(Target& obj, Scalar color_cross)
{
	Point centre = obj.TFea.centre;
	line(*canvas, Point(centre.x - 5, centre.y), Point(centre.x + 5, centre.y), color_cross);
	line(*canvas, Point(centre.x, centre.y - 5), Point(centre.x, centre.y + 5), color_cross);
}

void TargetDisplay::DrawBox(Target& obj, Scalar color_box)
{
	Rect box = obj.TInfo.getRectBox();
	rectangle(*canvas, box, color_box);
}

void TargetDisplay::DrawText(Target& obj, String name)
{
	Scalar color_text = Scalar(0, 0, 255);
	Rect box = obj.TInfo.getRectBox();
	putText(*canvas,
		name,
		Point(box.x, box.y - 2),
		FONT_HERSHEY_PLAIN, 1.0,
		color_text);
}

void TargetDisplay::SetColor(FList& fl, Scalar color)
{
	auto it = fl.begin();
	auto it_end = fl.end();

	for (; it != it_end; it++) {
		canvas->at<Vec3b>(*it)[0] = (uchar)color.val[0];
		canvas->at<Vec3b>(*it)[1] = (uchar)color.val[1];
		canvas->at<Vec3b>(*it)[2] = (uchar)color.val[2];
	}
}
