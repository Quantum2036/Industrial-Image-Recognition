#pragma once
#include "pch.h"
#include "Target.h"

/// <summary>
/// 在画布图像上显示目标Target，为Target类的友元
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

public:

	static const Scalar red;
	static const Scalar green;
	static const Scalar orange;
	static const Scalar black50;
	static const Scalar black20;

//	方法
public:

	/// <summary>
	/// 更改画布中指定点的颜色
	/// </summary>
	/// <param name="fl">要更改颜色的点的坐标集合</param>
	/// <param name="color">要更改的颜色</param>
	void DrawList(FList& list, Scalar color);

	/// <summary>
	/// 用颜色填充目标的内部
	/// </summary>
	/// <param name="obj">要填充的目标</param>
	/// <param name="color_inside">填充颜色</param>
	void DrawInside(Target& obj,  Scalar color_inside = black20);

	/// <summary>
	/// 用颜色填充目标的边界
	/// </summary>
	/// <param name="obj">要填充的目标</param>
	/// <param name="color_peripheral">填充颜色</param>
	void DrawPeripheral(Target& obj, Scalar color_peripheral = black50);

	/// <summary>
	/// 在目标中心画出反色的十字记号
	/// </summary>
	/// <param name="obj">要标记的目标</param>
	void DrawCross(Target& obj);

	/// <summary>
	/// 用方框在画布上标记出目标
	/// </summary>
	/// <param name="obj">要标记的目标</param>
	/// <param name="color">方框颜色</param>
	void DrawBox(Target& obj, Scalar color = green);

	/// <summary>
	/// 在画布上为目标添加文字
	/// </summary>
	/// <param name="obj">要进行添加文字的目标</param>
	/// <param name="name">要显示的文字</param>
	/// <param name="color">文字的颜色</param>
	void DrawText(Target& obj, String name, Scalar color = red);

private:

	/// <summary>
	/// 将画布上像素点的反色
	/// </summary>
	/// <param name="pt"></param>
	/// <returns></returns>
	void Reverse(Point pt);

};

