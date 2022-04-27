#include "CircleMatching.h"

CircleMatching::CircleMatching()
{
	pImg = nullptr;
	SE_radius = 0;
	SE_length = 0;
}

CircleMatching::CircleMatching(Mat* ptest, const uchar& radius)
{
	if (ptest->empty()) {
		fprintf_s(stderr, "ERROR: 进行圆形模板匹配的图像为空 \n");
		exit(EXIT_FAILURE);
	}
	if (ptest->channels() != 1) {
		fprintf_s(stderr, "ERROR: 进行圆形模板匹配的图像非灰度图 \n");
		exit(EXIT_FAILURE);
	}

	pImg = ptest;
	SE_radius = radius;
	SE_length = 2 * SE_radius + 1;

	Size kSize(SE_length, SE_length);
	SE = getStructuringElement(MORPH_ELLIPSE, kSize);
}

double CircleMatching::Todo(const Point& pt, const uchar color)
{
	uint hit_count = 0;
	uint count_all = 0;

	int nX = pt.x - SE_radius;
	int nY = pt.y - SE_radius;

	for (int i = 0; i < SE_length; i++) {
		for (int j = 0; j < SE_length; j++) {

			if (IsValidPoint(nX + i, nY + j)) {
				if (SE.at<uchar>(Point(i,j))) {	
					if (pImg->at<uchar>(Point(nX + i, nY + j)) == color)
					{
						hit_count++;
					}

					count_all++;
				}
			}

		}
	}
	return double(hit_count) / double(count_all);
}
