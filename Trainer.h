#pragma once
#include "pch.h"
#include "CVApp.h"

/// <summary>
/// 训练器类--根据同类物体的大量图像序列得到特征集合
///		使用请先用构造函数初始化（非默认）
///		再保存数据到文件即可
/// </summary>
class Trainer
{
//	构造 & 析构
public:
	Trainer();
	Trainer(Mat& background, const char* folderPath, const char* fileType = "");

//	数据
private:

	//目标名称
	String target_Name;

	//所有扫到的目标的特征
	TClassifier feature_Data;

	//分类器
	TClassifier target_Class;

//	方法
public:

	//保存分类器文件
	void SaveTClassifier(const char* Target_Name, const char* SavePath);

private:

	//	8 线程处理读入的图片
	void threadProcess_8(std::vector<String> image_names, Mat& background);

	//将数据保存到指定的文件夹
	void SaveData_d(String& Save_Full_Path, std::vector<feature>& feaIn);

	//为字符串加上字母序号
	String StrAddChar(const String& str, size_t length);

	//将一幅图像中所有目标数据载入 feature_Data
	void PushFeatureData(std::vector<Target>& target, String fileName);

	//读取文件夹下所有拓展名为type的文件
	//	若type为空字符串，则读取所有文件
	std::vector<String> getAllFile(String folderPath, String type);

	//验证读取文件的有效性，与 getAllFile 函数组合使用
	//	即能否用imread读取
	std::vector<String> VaildFileList(std::vector<String> fileList);

	//分析
	void Analysis(void);

	//初步筛选，errlimit为分类误差，建议[20-30]
	void Sieve_1(double errlimit = 0.30);

	//二次筛选
	void Sieve_2(double errlimit = 0.15);

	/// <summary>
	/// 由一组特征计算其平均特征值
	/// </summary>
	/// <param name="feas">用于计算平均值的特征集合</param>
	/// <returns>平均特征值</returns>
	feature getAverageFeature(const std::vector<feature>& feas);
};

