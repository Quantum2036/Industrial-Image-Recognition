#pragma once
#include "pch.h"
#include "FList.h"

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
	
	//����Ŀ��bool����
	void CalRectMat(const Mat* pImg, FList tlist);

	//����������
	void CalRectBox(void);

	//ʹrect������ʼ�մ���0
	void normalizeRect(Rect& rect)
	{
		rect.x = rect.x < 0 ? -rect.x : rect.x;
		rect.y = rect.y < 0 ? -rect.y : rect.y;
		rect.width = rect.width < 0 ? -rect.width : rect.width;
		rect.height = rect.height < 0 ? -rect.height : rect.height;
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

