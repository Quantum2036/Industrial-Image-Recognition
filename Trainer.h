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
	std::vector<feature> feature_Data;

//	方法
public:

	//将数据保存到指定的文件夹
	void SaveData_d(void);

private:
	
	//为字符串加上字母序号
	String StrAddChar(const String& str, size_t length);

	//将数据写入指定文件
	void FileSave(String name, std::vector<feature>& feaIn);

	//将一幅图像中所有目标数据载入 feature_Data
	void PushFeature(std::vector<Target>& target);

	//读取文件夹下所有拓展名为type的文件
	//	若type为空字符串，则读取所有文件
	std::vector<String> getAllFile(String folderPath, String type);

	//验证读取文件的有效性，与 getAllFile 函数组合使用
	//	即能否用imread读取
	std::vector<String> VaildFileList(std::vector<String> fileList);

	//分析
	void Analysis(void);
};

