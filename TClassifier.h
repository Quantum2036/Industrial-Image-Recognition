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

	//文件交换的路径
	String filePath;

public:

	static String strfeatureName[];

//	方法
public:

	//将未命名替换成name
	void ChangeName(String name);

	//将分类器数据保存到文件
	void SaveClassifier(const char* fileName);
	void SaveClassifier(void);

	//从文件中读取分类器
	bool LoadClassifier(const char* fileName);

	//添加新类型，会检验是否重复
	void AddNewClass(feature TFea, String name);

	//添加新数据，不检验重复
	void AddNewData(feature TFea, String name) {
		data.emplace_back(TFea);
		Tname.emplace_back(name);
		count++;
	}

	//返回记录的长度
	size_t getSize(void) {
		return count;
	}

	std::vector<feature> getDataVector(void) {
		return data;
	}

	//返回迭代器头
	std::vector<feature>::const_iterator	getDataBegin(void) {
		return data.cbegin();
	}

	//返回迭代器尾
	std::vector<feature>::const_iterator	getDataEnd(void) {
		return data.cend();
	}

	//返回第i个特征
	feature getFeatureAt(size_t i) {
		return data[i];
	}

	//返回第i个名称
	String getNameAt(size_t i) {
		return Tname.at(i);
	}

	//清空所有记录
	void clear(void) {
		count = 0;
		data.clear();
		Tname.clear();
	}

	//根据目标特征判断目标类型，err为基础容许误差
	String getTC(const feature& TFea);

	//根据目标特征强制分类
	String classify(const feature& TFea);

	//判断容器是否为空
	bool IsEmpty(void) {
		return data.empty();
	}

	//测试用函数———输出分类器文本数据到控制台窗口
	void PrintTClassifier_d(void);

	//判断容器中是否存在特征TFea
	bool IsExistTFeature(const feature& TFea, double errlimit = 0.15);

	//判断两个特征是否相似
	static	bool IsFeatureEqual(const feature& TFea_1, const feature& TFea_2, double errlimit);

private:

	//判断容器中是否存在命名name
	bool IsExistName(const String& name);

	//打开分类器文件
	FILE* OpenFile(const char* ClassifierPath, const char* mode);

	//从文件流fp中读取特征结构
	void ReadStructure(FILE* fp);

	//向文件流fp中写入特征结构
	void WriteStructure(FILE* fp, size_t count);

	//打印分类器基本信息
	void PrintClassifierMSG(void);


};

