#include "TFeature.h"
#include <math.h>

TFeature::TFeature()
{
}

TFeature::TFeature(const TFeature& fTemp)
{
	Struct_feature = fTemp.Struct_feature;
	centre	= fTemp.centre;
}

TFeature::TFeature(FList tlist, FList plist)
{
	Struct_feature.size			= tlist.GetSize();
	Struct_feature.peripheral	= plist.GetSize();

	Size MER					= CalMER(plist);
	Struct_feature.major_axis	= MER.width;
	Struct_feature.minor_axis	= MER.height;

	Struct_feature.corners		= 0;
	Struct_feature.Rectangularity	= CalR();
	Struct_feature.consistency	= CalC();
	Struct_feature.eccentricity	= CalE();

#if DEBUG_PRINTF
	print_d();
#endif // DEBUG_PRINTF

}

TFeature::~TFeature()
{
}

TFeature& TFeature::operator=(const TFeature& fTemp)
{
	Struct_feature = fTemp.Struct_feature;
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

double TFeature::CalR(void)
{
	double MER_area = Struct_feature.major_axis * Struct_feature.minor_axis;
	if (MER_area) {
		return (double)Struct_feature.size / MER_area;
	}
	else {
		return 0.0;
	}
}

double TFeature::CalC(void)
{
	if (Struct_feature.size) {
		return (double)Struct_feature.peripheral * (double)Struct_feature.peripheral / (double)Struct_feature.size;
	}
	else {
		return 0.0;
	}
}

double TFeature::CalE(void)
{
	if (Struct_feature.major_axis) {
		return (double)Struct_feature.minor_axis / (double)Struct_feature.major_axis;
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
	printf("周长：%3d, 面积：%3d\n", Struct_feature.peripheral, Struct_feature.size);
	printf("最小外接矩形：[%3d,%3d]\n", Struct_feature.major_axis, Struct_feature.minor_axis);
	printf("角点数：%d\n", Struct_feature.corners);
	printf("矩形度：%.3f, 圆形度：%.3f, 偏心率：%.3f\n", Struct_feature.Rectangularity, Struct_feature.consistency, Struct_feature.eccentricity);
	printf("centre: [%3d,%3d]\n", centre.x, centre.y);

}

#endif // DEBUG_PRINTF
