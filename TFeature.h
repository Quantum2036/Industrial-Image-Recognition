#pragma once
#include "pch.h"
#include "FList.h"

typedef struct _Fearture {
	uint size				= 0;		//面积
	uint Peripheral			= 0;		//周长
	Size MER				= Size();	//最小外接矩形
	uint corners			= 0;		//角点数
	double Rectangularity	= 0.0;		//矩形度
	double consistency		= 0.0;		//圆形度（致密度）
	double eccentricity		= 0.0;		//偏心率
}feature;

class TFeature
{
//	构造 & 析构
public:
	TFeature();
	TFeature(const TFeature& fTemp);
	TFeature(FList tlist, FList plist);
	~TFeature();

	TFeature& operator=(const TFeature& fTemp);

//	数据
public:

	//特征结构
	feature TFea;

	//目标几何中心
	Point centre;

//	方法
private:

	//计算最小外接矩形
	Size CalMER(FList& plist);

	//计算角点数-----未完成
	uint CalCorners(void);

	//计算矩形度
	double CalR(void);

	//计算圆形度（致密度）
	double CalC(void);

	//计算偏心率
	double CalE(void);

	//计算外接矩形的长短轴
	Size CalER(FList& plist);

	//旋转链表list中的点，以点Point为中心每次旋转angle度
	void RotateList(FList& list, int angle = 3);


#if DEBUG_PRINTF
public:
	void print_d(void);
#endif // DEBUG_PRINTF

};

