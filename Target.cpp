#include "Target.h"
#include "CircleMatching.h"
#include <algorithm>

Target::Target()
{
	pImg	= nullptr;

	//plist;
	//tlist;

	//TFea;
	//TInfo;

	//Isize;
	TState	= TargetState::TS_Null;
}

Target::Target(const Target& tTemp)
{
	pImg	= tTemp.pImg;

	plist	= tTemp.plist;
	tlist	= tTemp.tlist;
	SUSAN_list = tTemp.SUSAN_list;

	TFea	= tTemp.TFea;
	TInfo	= tTemp.TInfo;

	Isize	= tTemp.Isize;
	TState	= tTemp.TState;
}

Target::Target(Mat& Img, Point seed)
{
	pImg = &Img;
	Isize = Rect(0, 0, pImg->cols, pImg->rows);

	Grow(seed);

	if (tlist.GetSize() > MIN_TARGET_SIZE) {
#if DEBUG_PRINTF
		fprintf_s(stdout, "\n\n********捕获到目标********\n");
#endif // DEBUG_PRINTF

		TInfo	= TInformation(&Img, tlist, plist);
		TFea	= TFeature(tlist, plist);
		TState	= IsOutImg() ? TargetState::TS_OutImage : TargetState::TS_Normal;
		TFea.Struct_feature.isHollow	= IsHollow();
		TFea.Struct_feature.corners	= CalCorners().GetSize();
	}
	else {
		TState = TargetState::TS_Null;
	}
}

Target::~Target()
{
	pImg = nullptr;
}

Target& Target::operator=(const Target& tTemp)
{
	pImg		= tTemp.pImg;

	plist		= tTemp.plist;
	tlist		= tTemp.tlist;
	SUSAN_list = tTemp.SUSAN_list;

	TFea		= tTemp.TFea;
	TInfo		= tTemp.TInfo;

	Isize		= tTemp.Isize;
	TState		= tTemp.TState;

	return *this;
}

void Target::Grow(Point seed)
{
	if (seed.inside(Isize)) {	//检查生长点是否超出图像

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
	Rect rect = TInfo.getRectTarget();
	if (rect.x == 0 ||
		rect.y == 0 ||
		rect.x + rect.width + 1 == Isize.width ||
		rect.y + rect.height + 1 == Isize.height) {
		return true;
	}
	else {
		return false;
	}
}

bool Target::IsHollow(void)
{
	Point centre = plist.GetCentre();
	CircleMatching matchingOBJ(pImg, 3);
	return matchingOBJ.Todo(centre, 0) > 0.5 ? true :false;
}

FList Target::CalCorners(void)
{
	int count = plist.GetSize();
	std::vector<double> vect_1(count, 0);
	CircleMatching SUSAN(pImg, 7);

	for (auto it = plist.begin(); it != plist.end(); it++) {
		if (SUSAN.Todo(*it, 255) < 0.4) {
			SUSAN_list.AddData(*it);
		}
	}

	return SUSAN_list;
}
