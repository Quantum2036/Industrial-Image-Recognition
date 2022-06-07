#include "CVApp.h"
#include "TargetDisplay.h"
#include <time.h>
#include <io.h>

String CVApp::strWinName_Test[] = {
	"测试图像",			//	0
	"灰度测试图像",		//	1
	"去背景二值图像",		//	2
	"画布图像"			//	3
};

String CVApp::strWinName_bg[] = {
	"背景图像",			//	0
	"灰度背景图像",		//	1
	"去背景灰度图像",		//	2
	"去背景彩色图像",		//	3
};

CVApp::CVApp()
{
	pClassifier = nullptr;
}

CVApp::CVApp(Mat& test, Mat& background)
{
	pClassifier = nullptr;
	sizeImg = test.size();

	//	vector[0]————输入原图
	//	vector[1]————灰度图
	AddBackground(background);
	AddTestImg(test);
}

//	拆分构造函数

void CVApp::AddTestImg(Mat& tImg)
{
	if (tImg.empty()) {
		fprintf_s(stderr, "ERROR: Input  test image is empty. \n");
		exit(EXIT_FAILURE);
	}

	testImgs.emplace_back(tImg.clone());
	testImgs.emplace_back(ToGray(testImgs[0]).clone());

	if ( bgImgs.size() ) {
		if (!IsSameRect(&tImg, &bgImgs[0])) {
			fprintf_s(stderr, "WARNING: 测试图像和背景图像尺寸不同；\n"
				"测试图像：%d x %d\n"
				"背景图像：%d x %d\n", tImg.cols, tImg.rows, bgImgs[0].cols, bgImgs[0].rows);
			testImgs.clear();
		}
		else {
			PostProcessing();
		}
	}
}

void CVApp::AddBackground(Mat& bkImg)
{
	if (bkImg.empty()) {
		fprintf_s(stderr, "ERROR: Input  background image is empty. \n");
		exit(EXIT_FAILURE);
	}

	bgImgs.emplace_back(bkImg.clone());
	bgImgs.emplace_back(ToGray(bgImgs[0]).clone());

	if ( testImgs.size() ) {
		if (!IsSameRect(&testImgs[0], &bkImg)) {
			fprintf_s(stderr, "WARNING: 测试图像和背景图像尺寸不同；\n"
				"测试图像：%d x %d\n"
				"背景图像：%d x %d\n", testImgs[0].cols, testImgs[0].rows, bkImg.cols, bkImg.rows);
			bgImgs.clear();
		}
		else {
			PostProcessing();
		}
	}
}

void CVApp::PostProcessing(void)
{
	sizeImg = testImgs[0].size();

	//得到去背景的二值测试图像，这是进行图像处理的关键图像————[2]
	testImgs.emplace_back(MinusMat_1().clone());
#if ENABLE_OPTIMIZE_BINARY_IMG
	ProBinarization();		//对二值图像进行进一步处理
#endif // ENABLE_OPTIMIZE_BINARY_IMG

	//添加画布图像————[3]
	testImgs.emplace_back(GrayToBGR(testImgs[0]));

#if KEEP_BACKGROUND
	bgImgs.emplace_back(MinusMat_2().clone());		//添加去背景灰度图像————[2]
	bgImgs.emplace_back(MinusMat_3().clone());		//添加去背景彩色图像————[3]
#else
	//清空背景图像缓存
	bgImgs.clear();
#endif

	Scan();
}

//显示输出函数

void CVApp::ShowWindows(bool cb)
{
	CreateWindows();

#if DEBUG_PRINTF
	imshow(strWinName_Test[2], testImgs[2]);
	imshow(strWinName_Test[3], testImgs[3]);
#else
	for (int i = 0; i < 4; i++) {
		imshow(strWinName_Test[i], testImgs[i]);
	}
#endif // DEBUG_PRINTF	

#if KEEP_BACKGROUND
	for (int i = 0; i < 4; i++) {
		imshow(strWinName_bg[i], bgImgs[i]);
	}
#endif // KEEP_BACKGROUND

	WaitESC();
}

void CVApp::ShowTatget(void)
{
	TargetDisplay displayer(&testImgs[3]);

	for (auto it = target.begin(); it != target.end(); it++) {
		//displayer.DrawInside(*it, TargetDisplay::Random_Color());
		displayer.DrawList(it->CalCorners(), Scalar(0, 0, 255));
		displayer.DrawBox(*it);
		displayer.DrawText(*it, pClassifier->classify(it->GetFeature()));
		displayer.DrawCross(*it);
	}

}

void CVApp::CreateWindows(void)
{
#if DEBUG_PRINTF

	int nX = 600;
	int nY = 300;
	namedWindow(strWinName_Test[2]);
	moveWindow(strWinName_Test[2], nX, nY);

	namedWindow(strWinName_Test[3]);
	moveWindow(strWinName_Test[3], nX + 50 + sizeImg.width, nY);

#else
	int nX = 50;
	int nY = 250;
	for (int i = 0; i < 4; i++) {
		namedWindow(strWinName_Test[i]);
		moveWindow(strWinName_Test[i], nX, nY);
		nX += sizeImg.width + 50;
	}
#endif // DEBUG_PRINTF

#if KEEP_BACKGROUND
	nY += sizeImg.height + 50;
	nX = 50;
	for (int i = 0; i < 4; i++) {
		namedWindow(strWinName_bg[i]);
		moveWindow(strWinName_bg[i], nX, nY);
		nX += sizeImg.width + 50;
	}
#endif // KEEP_BACKGROUND
}

void CVApp::WaitESC(void)
{
	printf_s("\n按下ESC关闭所有窗口。\n");

	if (waitKey(0) == 27) {
		destroyAllWindows();
	};
}


//图像转换处理函数

Mat CVApp::ToGray(Mat& Img)
{
	Mat tImg = Img.clone();
	if (Img.channels() != 1) {
		cvtColor(tImg, tImg, COLOR_BGR2GRAY);
	}
	return tImg;
}

Mat CVApp::GrayToBGR(Mat& Img)
{
	int nChannel = Img.channels();

	if (nChannel == 3) {
		return Img.clone();
	}
	else if (nChannel == 1) {
		Mat bgrImg = Mat(Img.size(), CV_8UC3);
		auto it_g = Img.begin<uchar>(), it_end_g = Img.end<uchar>();
		auto it_bgr = bgrImg.begin<Vec3b>();

		for (; it_g != it_end_g; it_g++, it_bgr++) {
			(*it_bgr)[0] = *it_g;
			(*it_bgr)[1] = *it_g;
			(*it_bgr)[2] = *it_g;
		}

		return bgrImg;
	}
	else {
		fprintf_s(stderr, "ERROR: 无法为通道数[%d]的图像转换彩色. \n", nChannel);
		return Img.clone();
	}
}

Mat CVApp::MinusMat_1(uchar dif)
{
	Mat tImg = Mat::zeros(sizeImg, CV_8UC1);
	Mat* tpImg = &testImgs[1];		//灰度测试图像
	Mat* bgpImg = &bgImgs[1];		//灰度背景图像

	MatIterator_<uchar> it_t = tpImg->begin<uchar>(), it_end_t = tpImg->end<uchar>();
	MatIterator_<uchar> it_bg = bgpImg->begin<uchar>(), it_end_bg = bgpImg->end<uchar>();
	MatIterator_<uchar> it_b = tImg.begin<uchar>(), it_end_b = tImg.end<uchar>();

	for (; it_b != it_end_b; it_b++, it_bg++, it_t++) {
		if (fabs(*it_t - *it_bg) > dif) {
			*it_b = 255;
		}
	}

	return tImg;
}

Mat CVApp::MinusMat_2(void)
{
	Mat tImg = Mat::zeros(sizeImg, CV_8UC1);
	Mat* gpImg = &testImgs[1];		//灰度测试图像
	Mat* bpImg = &testImgs[2];		//二值测试图像

	MatIterator_<uchar> it_g = gpImg->begin<uchar>(), it_end_g = gpImg->end<uchar>();
	MatIterator_<uchar> it_b = bpImg->begin<uchar>(), it_end_b = bpImg->end<uchar>();
	MatIterator_<uchar> it_t = tImg.begin<uchar>(), it_end_t = tImg.end<uchar>();

	for (; it_t != it_end_t; it_g++, it_b++, it_t++) {
		if (*it_b) {
			*it_t = *it_g;
		}
	}

	return tImg;
}

Mat CVApp::MinusMat_3(void)
{
	Mat tImg = Mat::zeros(sizeImg, CV_8UC3);

	if (tImg.channels() == 1) {
		tImg = GrayToBGR(bgImgs[2]);
	}
	else if (tImg.channels() == 3) {

		Mat* bgrpImg = &testImgs[0];	//彩色测试图像
		Mat* bpImg = &testImgs[2];		//二值测试图像

		MatIterator_<uchar> it_b = bpImg->begin<uchar>(), it_end_b = bpImg->end<uchar>();
		MatIterator_<Vec3b> it_bgr = bgrpImg->begin<Vec3b>(), it_end_bgr = bgrpImg->end<Vec3b>();
		MatIterator_<Vec3b> it_t = tImg.begin<Vec3b>(), it_end_t = tImg.end<Vec3b>();

		for (; it_t != it_end_t; it_bgr++, it_b++, it_t++) {
			if (*it_b) {
				(*it_t)[0] = (*it_bgr)[0];
				(*it_t)[1] = (*it_bgr)[1];
				(*it_t)[2] = (*it_bgr)[2];
			}
		}
	}
	return tImg;
}

//二值图优化算法

void CVApp::ProBinarization(void)
{
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	morphologyEx(testImgs[2], testImgs[2], MORPH_CLOSE, kernel);
	morphologyEx(testImgs[2], testImgs[2], MORPH_OPEN, kernel);
}

//目标扫描捕获算法

size_t CVApp::Scan(ScanMode mode)
{
	switch (mode)
	{
	case ScanMode::SMode_random:
		Scan_Random();
		break;
	case ScanMode::SMode_grid:
		Scan_Grid();
		break;
	case ScanMode::SMode_pixel:
		Scan_Pixel();
		break;
	default:
		fprintf_s(stderr, "WARNING: 未定义该扫描模式  默认启用网格扫描 \n");
		Scan_Grid();
	}

#if DEBUG_PRINTF
	fprintf_s(stdout, "扫描已完成 \n捕获目标数: %zu\n\n", target.size());
#endif // DEBUG_PRINTF

	return target.size();
}

void CVApp::Scan_Random(void)
{
	//随机化
	time_t curTime;
	time(&curTime);
	static int randNum = 0; 
	RNG rng = RNG(curTime + randNum);
	randNum = (int)rng.uniform(0x00000000, 0xFFFFFFFF);		//为下次随机化添加随机变量

	Point pt;	//将要进行扫描的点的坐标
	size_t length = sizeImg.area() / 100;	//扫描的点的个数

	while (length)
	{
		pt.x = rng.uniform(0, sizeImg.width);
		pt.y = rng.uniform(0, sizeImg.height);

		//当预备扫描点在某个已记录图像中，弃用该随机点
		if (IsExistTarget(pt)) {
			continue;
		}
		length--;

		Target tPixel(testImgs[2], pt);
		if (tPixel.GetTargetState() == TargetState::TS_Normal) {
			target.emplace_back(tPixel);
		}

#if DISPLAY_SCAN_POINT
		testImgs[2].at<uchar>(pt) = COLOR_SCAN_POINT_8UC1;
#endif // DISPLAY_SCAN_POINT
	}
}

void CVApp::Scan_Grid(void)
{
	Point pt;

	for (int i = 0; i < sizeImg.width; i += MIN_SCAN_GAP) {
		for (int j = 0; j < sizeImg.height; j += MIN_SCAN_GAP) {
			pt = Point(i, j);

			if (!IsExistTarget(pt)) {
				Target tPixel(testImgs[2], pt);
				if (tPixel.GetTargetState() == TargetState::TS_Normal) {
					target.emplace_back(tPixel);
				}
			}

#if DISPLAY_SCAN_POINT
			testImgs[2].at<uchar>(pt) = COLOR_SCAN_POINT_8UC1;
#endif // 在二值图像中显示扫描点

		}
	}
}

void CVApp::Scan_Pixel(void)
{
	Point pt;

	for (int i = 0; i < sizeImg.height; i++) {
		for (int j = 0; j < sizeImg.width; j++) {
			pt = Point(i,j);

			if (!IsExistTarget(pt)) {
				Target tPixel(testImgs[2], pt);

				if (tPixel.GetTargetState() == TargetState::TS_Normal) {
					target.emplace_back(tPixel);
				}
			}
		}
	}
}

bool CVApp::IsExistTarget(Point pt)
{
	for (auto it = target.begin(); it != target.end(); it++) {
		if (it->IsInside(pt)) {
			return true;
		}
	}

	return false;
}
