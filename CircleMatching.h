#pragma once
#include "pch.h"

/// <summary>
/// 圆形模板匹配
/// </summary>
class CircleMatching
{
//	构造 &　析构
public:
	/// <summary>
	/// 唯一构造函数
	/// </summary>
	/// <param name="ptest">要进行模板匹配的图像指针，必须为二值图像</param>
	/// <param name="radius">结构元素的半径</param>
	CircleMatching(Mat* ptest, const uchar& radius);

//	数据
private:

	//要进行模板匹配的图像指针，必须为二值图像
	Mat* pImg;

	//匹配所用的结构元素
	Mat SE;

	//结构元素的长度
	int SE_length;

	//结构元素的半径
	int SE_radius;

//	方法
public:
	
	/// <summary>
	/// 进行模板匹配
	/// </summary>
	/// <param name="pt">进行模板匹配的中心点</param>
	/// <param name="color">进行模板匹配的颜色值</param>
	/// <returns>返回模板范围内颜色值为 color 的比例，若完美匹配返回1.0</returns>
	double Todo(const Point& pt, const uchar color);

private:

	/// <summary>
	/// 检验要比较的点是否在图像范围内
	/// </summary>
	/// <param name="nX">点所在的列数</param>
	/// <param name="nY">点所在的行数</param>
	/// <returns>点在图像范围内为true，否则为false</returns>
	bool IsValidPoint(const int nX, const int nY) {
		return (nX > 0 && nY > 0 && nX + SE_radius < pImg->cols && nY + SE_radius  < pImg->rows);
	}
};

