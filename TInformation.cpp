#include "TInformation.h"

TInformation::TInformation()
{

}

TInformation::TInformation(const TInformation& TInfo)
{
	TRectImg	= TInfo.TRectImg.clone();
	rect_Target = TInfo.rect_Target;
	rect_Box	= TInfo.rect_Box;
}

TInformation::~TInformation()
{
	TRectImg.release();
}

TInformation::TInformation(const Mat* pImg, FList& tlist, FList& plist)
{
	rect_Target = plist.GetRect();
	CalRectBox(pImg);
	CalRectMat(pImg, tlist);

#if DEBUG_PRINTF
	Print_d();
#endif // DEBUG_PRINTF

}

void TInformation::operator=(const TInformation& TInfo)
{
	TRectImg	= TInfo.TRectImg.clone();
	rect_Target = TInfo.rect_Target;
	rect_Box	= TInfo.rect_Box;
}

bool TInformation::IsInside(Point pt)
{
	if (pt.inside(rect_Box)) {
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

void TInformation::CalRectMat(const Mat* pImg, FList tlist)
{
	TRectImg = Mat(Size(rect_Box.width, rect_Box.height), CV_8UC1);
	
	auto it = tlist.begin();
	auto it_end = tlist.end();

	Point pt;
	for (; it != it_end; it++) {
		pt = Point(it->x - rect_Box.x, it->y - rect_Box.y);
		TRectImg.at<uchar>(pt) = 255;
	}
}

void TInformation::CalRectBox(const Mat* pImg)
{
	rect_Box.x		= rect_Target.x - MIN_SCAN_GAP;
	rect_Box.y		= rect_Target.y - MIN_SCAN_GAP;
	rect_Box.width	= rect_Target.width + 2 * MIN_SCAN_GAP;
	rect_Box.height = rect_Target.height + 2 * MIN_SCAN_GAP;
	
	normalizeRect(pImg, rect_Box);
}


#if DEBUG_PRINTF

void TInformation::Print_d(void)
{
	puts("TInformation 构造完成：");
	printf("外接矩形：[%3d,%3d]\t %3d  %3d\n", rect_Target.x, rect_Target.y, rect_Target.width, rect_Target.height);
	printf("框架矩形：[%3d,%3d]\t %3d  %3d\n", rect_Box.x, rect_Box.y, rect_Box.width, rect_Box.height);
	
	//namedWindow("debug_1");
	//imshow("debug_1", TRectImg);
	//waitKey(1000);
	//destroyWindow("debug_1");
}

#endif // DEBUG_PRINTF