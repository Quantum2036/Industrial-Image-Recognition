#pragma once

#include<iostream>
#include<opencv2/opencv.hpp>
using namespace cv;

#define DATAFOLDER	"C:\\量子泡沫\\Study\\数字图像处理\\data\\"
#define DESKTOP		"C:\\Users\\29462\\Desktop\\"

#define ON_MOUSE_CALLBACK	true
#define OFF_MOUSE_CALLBACK	false

//保留背景图像
#define KEEP_BACKGROUND		false

//对二值图像进行优化处理
//	将会使用相当多的计算资源
//	若非图像存在明显噪声，请尽量不要使用
#define ENABLE_OPTIMIZE_BINARY_IMG	false

//输出调试信息到控制台
#define DEBUG_PRINTF		false

//显示扫描点
#define DISPLAY_SCAN_POINT	false

//不同标记点的颜色
#define COLOR_INSIDE_8UC1		150
#define COLOR_PERIPHERAL_8UC1	230
#define COLOR_SCAN_POINT_8UC1	80

//最小扫描间隔
#define MIN_SCAN_GAP			5

//	能够锁定的最小目标面积
#define MIN_TARGET_SIZE			50

//	指定扫描模式
enum class ScanMode
{
	SMode_random,	//随机扫描
	SMode_grid,		//网格扫描
	SMode_pixel,	//逐点扫描
};

//	指定目标状态
enum class TargetState
{
	TS_Null,		//无目标
	TS_OutImage,	//部分在图像外的
	TS_Normal,		//正常图片
};
