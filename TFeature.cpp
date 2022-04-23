#include "TFeature.h"
#include <math.h>

TFeature::TFeature()
{
}

TFeature::TFeature(const TFeature& fTemp)
{
	TFea	= fTemp.TFea;
	centre	= fTemp.centre;
}

TFeature::TFeature(FList tlist, FList plist)
{
	TFea.size			= tlist.GetSize();
	TFea.Peripheral		= plist.GetSize();
	TFea.MER			= CalMER(plist);
	TFea.corners		= CalCorners();
	TFea.Rectangularity	= CalR();
	TFea.consistency	= CalC();
	TFea.eccentricity	= CalE();

#if DEBUG_PRINTF
	print_d();
#endif // DEBUG_PRINTF

}

TFeature::~TFeature()
{
}

TFeature& TFeature::operator=(const TFeature& fTemp)
{
	TFea	= fTemp.TFea;
	centre	= fTemp.centre;

	return *this;
}

Size TFeature::CalMER(FList& plist)
{
	FList list	= plist;
	Size temp	= CalER(list);
	centre		= plist.GetCentre();
	Size minSize = temp;

	for (uchar i = 0; i < 90; i++) {
		list = plist;
		RotateList(list, i);
		temp = CalER(list);
		
		if (temp.area() < minSize.area()) {
			minSize = temp;
		}
	}

	return minSize;
}

uint TFeature::CalCorners(void)
{
	Mat SE = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));


	return 0;
}

double TFeature::CalR(void)
{
	if (TFea.MER.area()) {
		return (double)TFea.size / (double)TFea.MER.area();
	}
	else {
		return 0.0;
	}
}

double TFeature::CalC(void)
{
	if (TFea.size) {
		return (double)TFea.Peripheral * (double)TFea.Peripheral / (double)TFea.size;
	}
	else {
		return 0.0;
	}
}

double TFeature::CalE(void)
{
	if (TFea.MER.width) {
		return (double)TFea.MER.height / (double)TFea.MER.width;
	}
	else {
		return 0.0;
	}
}

Size TFeature::CalER(FList& list)
{
	Rect rect = list.GetRect();

	int rX = rect.width;
	int rY = rect.height;

	if (rX > rY)
		return Size(rX, rY);
	else
		return Size(rY, rX);
}

void TFeature::RotateList(FList& list, int angle)
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


#if DEBUG_PRINTF

void TFeature::print_d(void)
{
	puts("TFeature 构造完成：");
	printf("周长：%3d, 面积：%3d\n", TFea.Peripheral, TFea.size);
	printf("最小外接矩形：[%3d,%3d]\n", TFea.MER.width, TFea.MER.height);
	printf("角点数：%d\n", TFea.corners);
	printf("矩形度：%.3f, 圆形度：%.3f, 偏心率：%.3f\n", TFea.Rectangularity, TFea.consistency, TFea.eccentricity);
	printf("centre: [%3d,%3d]\n", centre.x, centre.y);

}

#endif // DEBUG_PRINTF
