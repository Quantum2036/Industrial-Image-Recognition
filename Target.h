#pragma once
#include "pch.h"
#include "TClassifier.h"
#include "TInformation.h"

//程序捕获到的目标，螺丝，螺帽之类的玩意儿
class Target
{
/*************构造 & 析构*************/
public:
	Target();
	Target(const Target& tTemp);
	Target(Mat& pImage, Point seed);
	~Target();

	Target& operator=(const Target& tTemp);

/*************数据*************/
public:

	//目标信息
	TInformation TInfo;

	//目标的特征
	TFeature TFea;

private:

	//核心-构造所用的图像指针
	Mat* pImg;

	//画布，用来输出一些信息
	//	由App类中使用函数 SetCanvas() 手动提供,
	//	否则为构造时用的图像
	Mat* canvas;

	//分类器数据，存放不同目标类型的标识特征
	//	由App类中使用函数手动创建
	//	否则将根据图像创建新的分类器
	TClassifier* TCdata;

	//目标内部的所有点
	FList tlist;
	
	//目标的边界点（不在目标内）
	FList plist;

	//整幅图像的大小Rect
	Rect Isize;
	       
	//目标状态
	TargetState TState;

	//目标类型名称
	String TClass;

/*************方法*************/
public:

	//手动设置画布
	void SetCanvas(Mat* pImgCanvas);

	//返回目标的状态信息
	TargetState GetTargetState(void) {
		return TState;
	}

	//用颜色填充目标的内部和边界
	void DrawColor(uchar color_inside = COLOR_INSIDE_8UC1, uchar color_peripheral = COLOR_PERIPHERAL_8UC1);

	//用绿色方框在画布上标记出目标，并为其添加文字
	void DrawBox(Scalar color_box = Scalar(0, 255, 0), Scalar color_text = Scalar(0, 0, 255));

private:

	//以点（nX，nY）生长，将所有结果放在pt_Target，外周点放在pt_Peripheral
	void Grow(Point seed);

	//判断目标是否超出图像
	bool IsOutImg(void);

	//对Target类中的图像应用颜色更改
	void SetColor(FList& fl, uchar color);

};

