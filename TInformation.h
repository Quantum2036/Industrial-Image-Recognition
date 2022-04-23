#pragma once
#include "pch.h"
#include "FList.h"

//Ŀ���������Ϣ
class TInformation
{
//	���� & ����
public:

	TInformation();
	TInformation(const TInformation& TInfo);
	~TInformation();

	//��ʹ���ܳ���������˽ṹ
	TInformation(const Mat* pImg, FList& tlist, FList& plist);

	void operator=(const TInformation& TInfo);

//	����
private:

	//����Ŀ��,Trect��С�Ĳ�������
	Mat TRectImg;

	//Ŀ����ԭͼ�������������Ӿ��Σ�
	//	���ڽ���������ȡ
	Rect rect_Target;

	//���Ŀ��ľ�������Դ�����Ӿ���
	//	���ڽ���Ŀ�������
	Rect rect_Box;

//	����
public:

	//����Ŀ�����
	Mat getRectMat(void) {
		return TRectImg;
	}

	//����Ŀ������
	Rect getRectTarget(void) {
		return rect_Target;
	}

	//���ؿ������
	Rect getRectBox(void) {
		return rect_Box;
	}

	//�ж�һ�����Ƿ�����Ŀ��
	bool IsInside(Point pt);

private:
	
	//�Ƿ��п�
	void IsHollow(FList& tlist);

	//����Ŀ��bool����
	void CalRectMat(const Mat* pImg, FList tlist);

	//����������
	void CalRectBox(const Mat* pImg);

	//ʹrect������ʼ��λ��ͼ��߽���
	void normalizeRect(const Mat* pImg, Rect& rect)
	{
		if (rect.x < 0) {
			rect.x = 0;
		}
		if (rect.y < 0) {
			rect.y = 0;
		}
		if (rect.x + rect.width > pImg->cols) {
			rect.width = pImg->cols - rect.x;
		}
		if (rect.y + rect.height > pImg->rows) {
			rect.height = pImg->rows - rect.y;
		}
	}

	//��ͼ������任����ʶ��������
	Point PtConvers(const Point& pt)
	{
		return Point(pt.x - rect_Box.x, pt.y - rect_Box.y);
	}

#if DEBUG_PRINTF
	//���������
	void Print_d(void);
#endif // DEBUG_PRINTF

};

