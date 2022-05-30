#pragma once
#include "pch.h"
#include "Target.h"

/// <summary>
/// 在画布图像上显示目标Target
/// </summary>
class TargetDisplay
{
//	构造 & 析构
public:
	TargetDisplay();
	TargetDisplay(Mat* pImgCanvas);

//	数据
private:
	//画布
	Mat* canvas;

//	方法
public:

	/// <summary>
	/// 为点列中所有点的颜色赋值
	/// </summary>
	/// <param name="list">点列</param>
	/// <param name="color">颜色值</param>
	void DrawList(std::vector<Point> list, Scalar color);

	//用颜色填充目标的内部和边界
	void DrawInside(Target& obj,  Scalar color_inside = Scalar(230, 230, 230));
	void DrawPeripheral(Target& obj, Scalar color_peripheral = Scalar(150, 150, 150));

	/// <summary>
	/// 在目标中心画出十字记号
	/// </summary>
	/// <param name="obj">目标</param>
	/// <param name="color_cross">十字记号的颜色</param>
	void DrawCross(Target& obj, Scalar color_cross = Scalar(51, 153, 255));

	//用方框在画布上标记出目标
	void DrawBox(Target& obj, Scalar color_box = Scalar(0, 255, 0));

	//添加文字
	void DrawText(Target& obj, String name);

private:

	void SetColor(FList& fl, Scalar color);
};

