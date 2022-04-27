#pragma once
#include "pch.h"

//圆形模板匹配
class CircleMatching
{
//	构造 &　析构
public:
	CircleMatching();
	CircleMatching(Mat* ptest, const uchar& radius);

//	数据
private:

	//要进行匹配的模板，必须为二值图像
	Mat* pImg;

	//匹配所用的结构元素
	Mat SE;

	//结构元素的长度
	int SE_length;

	//结构元素的半径
	int SE_radius;

//	方法
public:

	//返回匹配程度，完美匹配返回1.0
	//	在点Pt进行模板匹配，匹配用的颜色为color
	double Todo(const Point& pt, const uchar color);

private:

	//检验点的有效性
	bool IsValidPoint(const int nX, const int nY) {
		return (nX > 0 && nY > 0 && nX + SE_radius < pImg->cols && nY + SE_radius  < pImg->rows);
	}
};

