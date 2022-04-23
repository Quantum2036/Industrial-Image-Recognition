#include "CVApp.h"
#include "TargetDisplay.h"
#include "Event.h"
#include <time.h>
#include <io.h>

String CVApp::strWinName_Test[] = {
	"����ͼ��",			//	0
	"�ҶȲ���ͼ��",		//	1
	"ȥ������ֵͼ��",		//	2
	"����ͼ��"			//	3
};

String CVApp::strWinName_bg[] = {
	"����ͼ��",			//	0
	"�Ҷȱ���ͼ��",		//	1
	"ȥ�����Ҷ�ͼ��",		//	2
	"ȥ������ɫͼ��",		//	3
};

CVApp::CVApp()
{
	pClassifier = nullptr;
}

CVApp::CVApp(Mat& test, Mat& background)
{
	pClassifier = nullptr;
	sizeImg = test.size();

	//	vector[0]������������ԭͼ
	//	vector[1]���������Ҷ�ͼ
	AddBackground(background);
	AddTestImg(test);
}

//	��ֹ��캯��

void CVApp::AddTestImg(Mat& tImg)
{
	testImgs.emplace_back(tImg.clone());
	testImgs.emplace_back(ToGray(testImgs[0]).clone());

	if ( bgImgs.size() ) {
		if (!IsSameRect(&tImg, &bgImgs[0])) {
			fprintf_s(stderr, "WARNING: ����ͼ��ͱ���ͼ��ߴ粻ͬ��\n"
				"����ͼ��%d x %d\n"
				"����ͼ��%d x %d\n", tImg.cols, tImg.rows, bgImgs[0].cols, bgImgs[0].rows);
			testImgs.clear();
		}
		else {
			PostProcessing();
		}
	}
}

void CVApp::AddBackground(Mat& bkImg)
{
	bgImgs.emplace_back(bkImg.clone());
	bgImgs.emplace_back(ToGray(bgImgs[0]).clone());

	if ( testImgs.size() ) {
		if (!IsSameRect(&testImgs[0], &bkImg)) {
			fprintf_s(stderr, "WARNING: ����ͼ��ͱ���ͼ��ߴ粻ͬ��\n"
				"����ͼ��%d x %d\n"
				"����ͼ��%d x %d\n", testImgs[0].cols, testImgs[0].rows, bkImg.cols, bkImg.rows);
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

	//�õ�ȥ�����Ķ�ֵ����ͼ�����ǽ���ͼ�����Ĺؼ�ͼ�񡪡�����[2]
	testImgs.emplace_back(MinusMat_1().clone());
#if ENABLE_OPTIMIZE_BINARY_IMG
	ProBinarization();		//�Զ�ֵͼ����н�һ������
#endif // ENABLE_OPTIMIZE_BINARY_IMG

	//���ӻ���ͼ�񡪡�����[3]
	testImgs.emplace_back(GrayToBGR(testImgs[0]));

#if KEEP_BACKGROUND
	bgImgs.emplace_back(MinusMat_2().clone());		//����ȥ�����Ҷ�ͼ�񡪡�����[2]
	bgImgs.emplace_back(MinusMat_3().clone());		//����ȥ������ɫͼ�񡪡�����[3]
#else
	//��ձ���ͼ�񻺴�
	bgImgs.clear();
#endif

	Scan();
}

//��ʾ�������

void CVApp::ShowAll_d(bool cb)
{
	CreateWindows();

	if (cb) {
		setMouseCallback(strWinName_Test[2], onMouse, &(testImgs[2]));
	}

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

	auto it = target.begin();
	auto it_end = target.end();

	//for (; it != it_end; it++) {
	//	displayer.DrawInside(*it);
	//	displayer.DrawPeripheral(*it);
	//}

	//it = target.begin();
	//it_end = target.end();

	for (; it != it_end; it++) {
		displayer.DrawCross(*it);
		displayer.DrawBox(*it);
		displayer.DrawText(*it, pClassifier->getTC(it->TFea.TFea));
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
	printf_s("\n����ESC�ر����д��ڡ�\n");

	if (waitKey(0) == 27) {
		destroyAllWindows();
	};
}


//ͼ��ת����������

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
		fprintf_s(stderr, "ERROR: �޷�Ϊͨ����[%d]��ͼ��ת����ɫ. \n", nChannel);
		return Img.clone();
	}
}

Mat CVApp::MinusMat_1(uchar dif)
{
	Mat tImg = Mat::zeros(sizeImg, CV_8UC1);
	Mat* tpImg = &testImgs[1];		//�ҶȲ���ͼ��
	Mat* bgpImg = &bgImgs[1];		//�Ҷȱ���ͼ��

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
	Mat* gpImg = &testImgs[1];		//�ҶȲ���ͼ��
	Mat* bpImg = &testImgs[2];		//��ֵ����ͼ��

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

		Mat* bgrpImg = &testImgs[0];	//��ɫ����ͼ��
		Mat* bpImg = &testImgs[2];		//��ֵ����ͼ��

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

//��ֵͼ�Ż��㷨

void CVApp::ProBinarization(void)
{
	fprintf_s(stderr, "�����Ż���ֵͼ... \n");
	ProBinarization_Subprocess_1();
	ProBinarization_Subprocess_2();
}

void CVApp::ProBinarization_Subprocess_1(void)
{
	Point pt;
	FList tlist;
	uchar count = 0;

	for (int i = 0; i < sizeImg.width; i++) {
		for (int j = 0; j < sizeImg.height; j++) {
			pt = Point(i, j);

			ProBinarization_Subprocess_1_OnGrow(tlist, j, i, testImgs[2].at<uchar>(pt), count);
			
			if (count < MIN_TARGET_SIZE / 2) {
				ProBinarization_Subprocess_1_ApplyList(tlist);
			}

			tlist.Empty();
			count = 0;
		}
	}
}

void CVApp::ProBinarization_Subprocess_1_OnGrow(FList& tlist, uint nX, uint nY, uchar flag, uchar& count)
{
	if ( count < MIN_TARGET_SIZE && nX < (uint)sizeImg.height && nY < (uint)sizeImg.width ) {	//����������Ƿ񳬳�ͼ��

		if (testImgs[2].at<uchar>(nX, nY) == flag && !tlist.IsExist(Point(nY, nX))) {
			//(nX,nY)���ڲ��㣬��������
			tlist.AddData(Point(nY, nX));
			count++;

			ProBinarization_Subprocess_1_OnGrow(tlist, nX - 1, nY, flag, count);
			ProBinarization_Subprocess_1_OnGrow(tlist, nX + 1, nY, flag, count);
			ProBinarization_Subprocess_1_OnGrow(tlist, nX, nY - 1, flag, count);
			ProBinarization_Subprocess_1_OnGrow(tlist, nX, nY + 1, flag, count);
		}

	}
}

void CVApp::ProBinarization_Subprocess_1_ApplyList(FList& tlist)
{
	auto it = tlist.begin();
	auto it_end = tlist.end();

	uchar color = testImgs[2].at<uchar>(*it) == 0 ? 255 : 0;

	for (; it != it_end; it++) {
		testImgs[2].at<uchar>(*it) = color;
	}
}

void CVApp::ProBinarization_Subprocess_2(void)
{
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	morphologyEx(testImgs[2], testImgs[2], MORPH_OPEN, kernel);

}

//Ŀ��ɨ�貶���㷨

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
		puts("δ�����ɨ��ģʽ");
		exit(-2);
	}

	puts("ɨ�������");
	printf("����Ŀ����: %zd\n\n", target.size());


	return target.size();
}

void CVApp::Scan_Random(void)
{
	//�����
	time_t curTime;
	time(&curTime);
	static int randNum = 0;
	RNG rng = RNG(curTime + randNum);
	randNum = (int)rng.uniform(0x00000000, 0xFFFFFFFF);		//Ϊ�´�����������������

	Point pt;
	size_t length = sizeImg.area() / 100;

	for (size_t i = 0; i < length; i++)
	{
		pt.x = rng.uniform(0, sizeImg.width);
		pt.y = rng.uniform(0, sizeImg.height);

		if (!IsExistTarget(pt)) {
			Target tPixel(testImgs[2], pt);

			if (tPixel.GetTargetState() != TargetState::TS_Null) {
				target.emplace_back(tPixel);
			}
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
				if (tPixel.GetTargetState() != TargetState::TS_Null) {
					target.emplace_back(tPixel);
				}
			}

#if DISPLAY_SCAN_POINT
			testImgs[2].at<uchar>(pt) = COLOR_SCAN_POINT_8UC1;
#endif // DISPLAY_SCAN_POINT

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

				if (tPixel.GetTargetState() != TargetState::TS_Null) {
					target.emplace_back(tPixel);
				}
			}
		}
	}
}

bool CVApp::IsExistTarget(Point pt)
{
	auto it = target.begin();
	auto it_end = target.end();

	for (; it != it_end; it++) {
		if (it->TInfo.IsInside(pt)) {
			return true;
		}
	}

	return false;
}
