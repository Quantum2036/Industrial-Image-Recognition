#include "CircleMatching.h"

CircleMatching::CircleMatching(Mat* ptest, const uchar& radius)
{
	if (ptest->empty()) {
		fprintf_s(stderr, "ERROR: ����Բ��ģ��ƥ���ͼ��Ϊ�� \n");
		exit(EXIT_FAILURE);
	}
	if (ptest->channels() != 1) {
		fprintf_s(stderr, "ERROR: ����Բ��ģ��ƥ���ͼ��ǻҶ�ͼ \n");
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
	//ģ�巶Χ��������ɫֵcolor�ĵ�ĸ���
	uint hit_count = 0;

	//ģ�巶Χ�ڵ������
	uint count_all = 0;

	//Point(nX,nY)Ϊģ������Ͻǵ�����
	int nX = pt.x - SE_radius;
	int nY = pt.y - SE_radius;

	for (int i = 0; i < SE_length; i++) {
		for (int j = 0; j < SE_length; j++) {

			if (IsValidPoint(nX + i, nY + j) && SE.at<uchar>(Point(i, j))) {
				if (pImg->at<uchar>(Point(nX + i, nY + j)) == color)
				{
					hit_count++;
				}

				count_all++;
			}

		}
	}

	return double(hit_count) / double(count_all);
}
