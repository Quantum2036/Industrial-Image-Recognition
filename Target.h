#pragma once
#include "pch.h"
#include "TFeature.h"
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
	friend class TargetDisplay;

/*************数据*************/
public:

	//目标位置信息
	TInformation TInfo;

	//目标的特征
	TFeature TFea;

	std::vector<Point> SUSAN_list;

private:

	//核心-构造所用的图像指针
	Mat* pImg;

	//目标内部的所有点
	FList tlist;
	
	//目标的边界点（不在目标内）
	FList plist;

	//整幅图像的大小Rect
	Rect Isize;
	       
	//目标状态
	TargetState TState;

/*************方法*************/
public:

	//返回目标的状态信息
	TargetState GetTargetState(void) {
		return TState;
	}

private:

	//以点（nX，nY）生长，将所有结果放在pt_Target，外周点放在pt_Peripheral
	void Grow(Point seed);

	//判断目标是否超出图像
	bool IsOutImg(void);
	
	//判断目标是否中空
	bool IsHollow(void);

	//计算角点数——SUSAN方法
	uint CalCorners(void);

	//计算角点数——形态学方法
	uint CalCorners_morph(void) {
		return 0;
	}

};

