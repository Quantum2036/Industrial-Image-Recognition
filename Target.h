#pragma once
#include "pch.h"
#include "FList.h"
#include <array>

//	特征数组
using fea_array = std::array<double, FEATURE_SIZE>;

//	特征结构
typedef struct _Fearture {
	uint size				= 0;		//面积
	uint peripheral			= 0;		//周长
	uint major_axis			= 0;		//MER的长轴
	uint minor_axis			= 0;		//MER的短轴
	uint isHollow			= 0;		//是否中空
	double Rectangularity	= 0.0;		//矩形度
	double consistency		= 0.0;		//圆形度（致密度）
	double eccentricity		= 0.0;		//偏心率
}feature;

/// <summary>
/// 程序捕获到的目标，螺丝，螺帽之类的玩意儿
/// </summary>
class Target
{
//	构造 & 析构
public:
	Target();
	Target(const Target& tTemp);
	Target(Mat& pImage, Point seed);
	~Target();

	Target& operator=(const Target& tTemp);
	friend class TargetDisplay;

//	数据
private:

	//核心-构造所用的图像指针
	Mat* pImg;

	//目标大小的布尔矩阵
	Mat TRectImg;

	//目标内部的所有点
	FList tlist;
	
	//目标的边界点（不在目标内）
	FList plist;

	//整幅图像的大小
	Rect rect_image;

	//目标在原图像的所属区域（外接矩形）
	//	用于进行特征提取
	Rect rect_Target;

	//标记目标的矩形外框，略大于外接矩形
	//	用于进行目标分离标记
	Rect rect_Box;

	//目标几何中心
	Point centre;

	//特征结构
	feature struct_feature;

	//目标状态
	TargetState TState;

//	方法
public:

	//返回目标几何中心
	Point GetCentre(void) {
		return centre;
	}

	//返回目标特征结构
	feature GetFeature(void) {
		return struct_feature;
	}

	//返回目标状态
	TargetState GetTargetState(void) {
		return TState;
	}

	//判断一个点是否在目标内部
	bool IsInside(Point pt);

	/// <summary>
	/// 使用SUSAN方法计算角点
	/// </summary>
	/// <returns>角点集合</returns>
	FList CalCorners(void);

	/// <summary>
	/// 将特征结构转化成特征数组形式便于计算
	/// </summary>
	/// <param name="fea">输入特征</param>
	/// <returns>输出数组</returns>
	static fea_array Struct2Array(feature fea) {
		fea_array arr{ 0.0 };
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
		fea.size = (uint)arr[0];		fea.peripheral = (uint)arr[1];
		fea.major_axis = (uint)arr[2];		fea.minor_axis = (uint)arr[3];
		fea.isHollow = arr[4] > 0.5 ? 1 : 0;
		fea.Rectangularity = arr[5];
		fea.consistency = arr[6];
		fea.eccentricity = arr[7];

		return fea;
	}

private:

	//判断目标是否超出图像
	bool IsOutImg(void);

	//判断目标是否中空
	bool IsHollow(void);

	/// <summary>
	/// 在一点进行生长，
	/// 生长结果放在tlist，
	/// 外周点放在plist
	/// </summary>
	/// <param name="seed">生长点</param>
	void Grow(Point seed);

	//计算目标外接矩形
	void CalRectTarget(void) {
		rect_Target = plist.GetRect();
	}

	//计算目标bool矩阵
	void CalRectMat(void);

	//计算目标外框矩形
	void CalRectBox(void);

	//计算特征
	void CalFeature(void);

	//计算最小外接矩形
	Size CalMER();

	//计算外接矩形的长短轴
	Size CalER(FList& plist);

	//计算矩形度
	double CalR(void);

	//计算圆形度（致密度）
	double CalC(void);

	//计算偏心率
	double CalE(void);

	//旋转链表list中的点，以点Point为中心每次旋转angle度
	void RotateList(FList& list, int angle = 1);

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
};

