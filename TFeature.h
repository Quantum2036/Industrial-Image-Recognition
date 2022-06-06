#pragma once
#include "pch.h"
#include "FList.h"
#include <array>

using fea_array = std::array<double, FEATURE_SIZE>;

typedef struct _Fearture {
	uint size				= 0;		//面积
	uint peripheral			= 0;		//周长
	uint major_axis			= 0;		//MER的长轴
	uint minor_axis			= 0;		//MER的短轴
	uint isHollow			= 0;		//是否中空
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
	feature Struct_feature;

	//目标几何中心
	Point centre;

//	方法
public:

	/// <summary>
	/// 将特征结构转化成特征数组形式便于计算
	/// </summary>
	/// <param name="fea">输入特征</param>
	/// <returns>输出数组</returns>
	static fea_array Struct2Array(feature fea) {
		fea_array arr{0.0};
		arr[0] = (double)fea.size;			arr[1] = (double)fea.peripheral;
		arr[2] = (double)fea.major_axis;	arr[3] = (double)fea.minor_axis;
		arr[4] = (double)fea.isHollow;		arr[5] = fea.Rectangularity;
		arr[6] = fea.consistency;			arr[7] = fea.eccentricity;

		return arr;
	}

	/// <summary>
	/// 将特征数组转化成特征结构形式便于存储表示
	/// </summary>
	/// <param name="arr">输入数组</param>
	/// <returns>输出特征</returns>
	static feature Array2Struct(fea_array arr) {
		feature fea;
		fea.size		= (uint)arr[0];		fea.peripheral		= (uint)arr[1];
		fea.major_axis	= (uint)arr[2];		fea.minor_axis		= (uint)arr[3];
		fea.isHollow	= arr[4] > 0.5 ? 1 : 0;	
		fea.Rectangularity	= arr[5];
		fea.consistency		= arr[6];	
		fea.eccentricity	= arr[7];

		return fea;
	}

private:

	//计算最小外接矩形
	Size CalMER(FList& plist);

	//计算矩形度
	double CalR(void);

	//计算圆形度（致密度）
	double CalC(void);

	//计算偏心率
	double CalE(void);

	//计算外接矩形的长短轴
	Size CalER(FList& plist);

	//旋转链表list中的点，以点Point为中心每次旋转angle度
	void RotateList(FList& list, int angle = 1);


#if DEBUG_PRINTF
public:
	void print_d(void);
#endif // DEBUG_PRINTF

};

