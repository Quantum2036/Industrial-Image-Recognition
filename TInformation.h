#pragma once
#include "pch.h"
#include "FList.h"

//目标的区域信息
class TInformation
{
//	构造 & 析构
public:

	TInformation();
	TInformation(const TInformation& TInfo);
	~TInformation();

	//请使用周长表来构造此结构
	TInformation(const Mat* pImg, FList& tlist, FList& plist);

	void operator=(const TInformation& TInfo);

//	数据
private:

	//包含目标,Trect大小的布尔矩阵
	Mat TRectImg;

	//目标在原图像的所属区域（外接矩形）
	//	用于进行特征提取
	Rect rect_Target;

	//标记目标的矩形外框，略大于外接矩形
	//	用于进行目标分离标记
	Rect rect_Box;

//	方法
public:

	//返回目标矩阵
	Mat getRectMat(void) {
		return TRectImg;
	}

	//返回目标区域
	Rect getRectTarget(void) {
		return rect_Target;
	}

	//返回框架区域
	Rect getRectBox(void) {
		return rect_Box;
	}

	//判断一个点是否属于目标
	bool IsInside(Point pt);

private:
	
	//是否中空
	void IsHollow(FList& tlist);

	//计算目标bool矩阵
	void CalRectMat(const Mat* pImg, FList tlist);

	//计算矩形外框
	void CalRectBox(const Mat* pImg);

	//使rect中数据始终位于图像边界内
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

	//将图像坐标变换到标识矩阵坐标
	Point PtConvers(const Point& pt)
	{
		return Point(pt.x - rect_Box.x, pt.y - rect_Box.y);
	}

#if DEBUG_PRINTF
	//测试用输出
	void Print_d(void);
#endif // DEBUG_PRINTF

};

