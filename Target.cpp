#include "Target.h"
#include "CircleMatching.h"
#include <algorithm>

Target::Target()
{
	pImg	= nullptr;
	TState	= TargetState::TS_Null;
}

Target::Target(const Target& tTemp)
{
	pImg			= tTemp.pImg;
	TRectImg		= tTemp.TRectImg.clone();

	plist			= tTemp.plist;
	tlist			= tTemp.tlist;

	rect_image		= tTemp.rect_image;
	rect_Target		= tTemp.rect_Target;
	rect_Box		= tTemp.rect_Box;

	centre			= tTemp.centre;
	struct_feature	= tTemp.struct_feature;
	
	TState			= tTemp.TState;
}

Target::Target(Mat& Img, Point seed)
{
	pImg = &Img;
	rect_image = Rect(0, 0, pImg->cols, pImg->rows);

	Grow(seed);

	if (tlist.GetSize() > MIN_TARGET_SIZE) {
#if DEBUG_PRINTF
		fprintf_s(stdout, "\n\n********捕获到目标********\n");
#endif // DEBUG_PRINTF

		CalRectTarget();
		CalRectBox();
		CalRectMat();

		if (IsOutImg()) {
			TState = TargetState::TS_OutImage;
		}
		else {
			TState = TargetState::TS_Normal;
			CalFeature();	
		}
	}
	else {
		TState = TargetState::TS_Null;
	}
}

Target::~Target()
{
	pImg = nullptr;
	TRectImg.release();
}

Target& Target::operator=(const Target& tTemp)
{
	pImg			= tTemp.pImg;
	TRectImg		= tTemp.TRectImg.clone();

	plist			= tTemp.plist;
	tlist			= tTemp.tlist;

	rect_image		= tTemp.rect_image;
	rect_Target		= tTemp.rect_Target;
	rect_Box		= tTemp.rect_Box;

	centre			= tTemp.centre;
	struct_feature	= tTemp.struct_feature;

	TState			= tTemp.TState;

	return *this;
}

void Target::Grow(Point seed)
{
	if (seed.inside(rect_image)) {	//检查生长点是否超出图像

		if (pImg->at<uchar>(seed) == 255 && !tlist.IsExist(seed)) {
			//(nX,nY)是内部点，继续生长
			tlist.AddData(seed);

			Grow(Point(seed.x - 1, seed.y));
			Grow(Point(seed.x + 1, seed.y));
			Grow(Point(seed.x, seed.y - 1));
			Grow(Point(seed.x, seed.y + 1));
		}
		else if (!tlist.IsEmpty() && pImg->at<uchar>(seed) == 0 && !plist.IsExist(seed)) {
			//点(nX,nY)为边界点
			plist.AddData(seed);
		}
	}
}

bool Target::IsOutImg(void)
{
	if (rect_Target.x == 0 ||
		rect_Target.y == 0 ||
		rect_Target.x + rect_Target.width + 1 == rect_image.width ||
		rect_Target.y + rect_Target.height + 1 == rect_image.height) {
		return true;
	}
	else {
		return false;
	}
}

bool Target::IsHollow(void)
{
	CircleMatching matchingOBJ(pImg, 3);
	return matchingOBJ.Todo(centre, 0) > 0.5 ? true :false;
}

FList Target::CalCorners(void)
{
	FList list;
	int count = plist.GetSize();
	std::vector<double> vect_1(count, 0);
	CircleMatching SUSAN(pImg, 7);

	for (auto it = plist.begin(); it != plist.end(); it++) {
		if (SUSAN.Todo(*it, 255) < 0.4) {
			list.AddData(*it);
		}
	}

	return list;
}

bool Target::IsInside(Point pt)
{
	if (pt.inside(rect_Box)) {	//点在目标的外接矩形内
		if (TRectImg.at<uchar>(PtConvers(pt)) != 0) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void Target::CalRectMat(void)
{
	TRectImg = Mat(Size(rect_Box.width, rect_Box.height), CV_8UC1);

	Point pt;
	for (auto it = tlist.begin(); it != tlist.end(); it++) {
		pt = Point(it->x - rect_Box.x, it->y - rect_Box.y);
		TRectImg.at<uchar>(pt) = 255;
	}
}

void Target::CalRectBox(void)
{
	rect_Box.x = rect_Target.x - MIN_SCAN_GAP;
	rect_Box.y = rect_Target.y - MIN_SCAN_GAP;
	rect_Box.width = rect_Target.width + 2 * MIN_SCAN_GAP;
	rect_Box.height = rect_Target.height + 2 * MIN_SCAN_GAP;

	normalizeRect(pImg, rect_Box);
}

void Target::CalFeature(void)
{
	struct_feature.size			= tlist.GetSize();
	struct_feature.peripheral	= plist.GetSize();
	centre						= plist.GetCentre();

	Size MER = CalMER();
	struct_feature.major_axis	= MER.width;
	struct_feature.minor_axis	= MER.height;
	struct_feature.isHollow		= IsHollow();

	struct_feature.Rectangularity	= CalR();
	struct_feature.consistency		= CalC();
	struct_feature.eccentricity		= CalE();
}

Size Target::CalMER()
{
	FList list = plist;
	Size temp = CalER(list);
	Size minSize = temp;

	for (int i = 0; i < 90; i++) {
		list = plist;
		RotateList(list, i);
		temp = CalER(list);

		if (temp.area() < minSize.area()) {
			minSize = temp;
		}
	}

	return minSize;
}

Size Target::CalER(FList& list)
{
	Rect rect = list.GetRect();

	int rX = rect.width;
	int rY = rect.height;

	if (rX > rY)
		return Size(rX, rY);
	else
		return Size(rY, rX);
}

double Target::CalR(void)
{
	double MER_area = struct_feature.major_axis * struct_feature.minor_axis;
	if (MER_area) {
		return (double)struct_feature.size / MER_area;
	}
	else {
		return 0.0;
	}
}

double Target::CalC(void)
{
	if (struct_feature.size) {
		return (double)struct_feature.peripheral * (double)struct_feature.peripheral / (double)struct_feature.size;
	}
	else {
		return 0.0;
	}
}

double Target::CalE(void)
{
	if (struct_feature.major_axis) {
		return (double)struct_feature.minor_axis / (double)struct_feature.major_axis;
	}
	else {
		return 0.0;
	}
}

void Target::RotateList(FList& list, int angle)
{
	auto it = list.wbegin();
	auto it_end = list.wend();

	int nX = centre.x;
	int nY = centre.y;
	int x = 0;
	int y = 0;

	double cos0 = cos(3.1415926536 * angle / 360.0);
	double sin0 = sin(3.1415926536 * angle / 360.0);

	for (; it != it_end; it++) {
		x = it->x;
		y = it->y;
		it->x = int(nX + (x - nX) * cos0 - (y - nY) * sin0);
		it->y = int(nY + (x - nX) * sin0 + (y - nY) * cos0);
	}
}
