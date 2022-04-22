#pragma once
#include "pch.h"
#include "TFeature.h"
#include <vector>

class TClassifier
{
//	构造 & 析构
public:
	TClassifier();
	TClassifier(const TClassifier& tc);
	~TClassifier();

	TClassifier& operator=(const TClassifier& tc);

//	数据
private:
	//记录的数据数目
	size_t count;

	//特征结构，存储的特征向量
	std::vector<feature> data;

	//与数据对应的描述字符串
	std::vector<String> Tname;

public:

	static String strfeatureName[];

//	方法
public:

	//添加新标识
	void AddNewClass(const feature& TFea, const String& name);

	//将分类器数据保存到文件
	void SaveClassifier(void);

	//从文件中读取分类器
	void LoadClassifier(const char* filePath);

	//根据目标特征判断目标类型，err为基础容许误差
	String getTC(const feature& TFea);

	//判断容器是否为空
	bool IsEmpty(void) {
		return data.empty();
	}

	//输出分类器到控制台窗口
	void PrintTClassifier_d(void);

	//判断两个特征是否相等
	static	bool IsFeatureEqual(const feature& TFea_1, const feature& TFea_2, double err);

private:

	//判断容器中是否存在特征TFea
	bool IsExistTFeature(const feature& TFea);

	//判断容器中是否存在命名name
	bool IsExistName(const String& name);

	//打开分类器文件
	FILE* OpenFile(const char* ClassifierPath, const char* mode);

	//读取数据结构
	void ReadStructure(FILE* fp);

	//写入数据结构
	void WriteStructure(FILE* fp, size_t count);

	
};

