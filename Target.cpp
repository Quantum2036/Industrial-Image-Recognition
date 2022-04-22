#include "Target.h"

String strTargetClass[]{
	"null",
	"incomplete",
	"unknown",
};

Target::Target()
{
	pImg	= nullptr;
	canvas	= nullptr;
	TCdata	= nullptr;

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
	canvas	= tTemp.canvas;
	TCdata	= tTemp.TCdata;

	plist	= tTemp.plist;
	tlist	= tTemp.tlist;

	TFea	= tTemp.TFea;
	TInfo	= tTemp.TInfo;

	Isize	= tTemp.Isize;
	TState	= tTemp.TState;
}

Target::Target(Mat& Img, Point seed)
{
	pImg = &Img;
	canvas = nullptr;
	TCdata = nullptr;
	Isize = Rect(0, 0, pImg->cols, pImg->rows);

	Grow(seed);

	if (tlist.GetSize() > MIN_TARGET_SIZE) {
#if DEBUG_PRINTF
		fprintf_s(stdout, "\n\n********捕获到目标********\n");
#endif // DEBUG_PRINTF

		TInfo	= TInformation(&Img, tlist, plist);
		TFea	= TFeature(tlist, plist);
		TState	= IsOutImg() ? TargetState::TS_OutImage : TargetState::TS_Normal;

		if (TState == TargetState::TS_Normal) {
			
		}
	}
	else
	{
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
	canvas		= tTemp.canvas;

	plist		= tTemp.plist;
	tlist		= tTemp.tlist;

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
		rect.x + rect.width == Isize.width ||
		rect.y + rect.height == Isize.height) {
		return true;
	}
	else {
		return false;
	}
}

void Target::SetCanvas(Mat* pImgCanvas)
{
	if (pImgCanvas->size() != pImg->size()) {
		fprintf_s(stderr, "WARNING: 画布图像与测试图像尺寸不一致. \n");
		canvas = pImg;
		return;
	}

	int nChannel = pImgCanvas->channels();

	if (nChannel == 1) {	//当画布是灰度图时转换为彩色
		cvtColor(*pImgCanvas, *pImgCanvas, COLOR_GRAY2BGR);
		canvas = pImgCanvas;
	}
	else if (nChannel == 3) {
		canvas = pImgCanvas;
	}
	else {
		canvas = pImg;
	}
}

void Target::DrawColor(uchar color_inside, uchar color_peripheral)
{
	if (!canvas) {
		canvas = pImg;
	}

	SetColor(tlist, color_inside);
	SetColor(plist, color_peripheral);
}

void Target::SetColor(FList& fl, uchar color)
{
	auto it = fl.begin();
	auto it_end = fl.end();

	for (; it != it_end; it++) {
		canvas->at<Vec3b>(*it)[0] = color;
		canvas->at<Vec3b>(*it)[1] = color;
		canvas->at<Vec3b>(*it)[2] = color;
	}
}

void Target::DrawBox(Scalar color_box, Scalar color_text)
{
	if (!canvas) {
		canvas = pImg;
	}

	Rect box = TInfo.getRectBox();
	rectangle(*canvas, box, color_box);
	putText(*canvas,
		strTargetClass[(int)TState],
		Point(box.x, box.y - 2),
		FONT_HERSHEY_PLAIN, 0.8,
		color_text);
}
