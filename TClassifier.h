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

	//特征结构所包含的不同特征名
	static String strfeatureName[];

//	方法
public:

	//判断分类器是否为空
	bool IsEmpty(void) {
		return count == 0;
	}

	//清空分类器类中所有记录
	void clear(void) {
		count = 0;
		data.clear();
		Tname.clear();
	}

	//返回中分类器中记录的数量
	size_t getSize(void) {
		return count;
	}

	//返回数据的Vector容器
	std::vector<feature>& getDataVector(void) {
		return data;
	}

	std::vector<feature>::const_iterator getDataBegin(void) {
		return data.cbegin();
	}

	std::vector<feature>::const_iterator getDataEnd(void) {
		return data.cend();
	}

	//返回第i个记录的特征
	feature getFeatureAt(size_t i) {
		return data[i];
	}

	//返回第i个记录的名称
	String getNameAt(size_t i) {
		return Tname.at(i);
	}

	//添加新数据，不检验重复
	void AddNewData(const feature& TFea, const String& name) {
		data.emplace_back(TFea);
		Tname.emplace_back(name);
		count++;
	}
	
	//添加新类型，会检验是否重复
	void AddNewClass(const feature& TFea, const String& name);

	/// <summary>
	/// 将分类器数据保存到指定路径
	/// </summary>
	/// <param name="fileName">将要保存分类器的位置</param>
	void SaveClassifier(const char* fileName);

	/// <summary>
	/// 保存分类器，仅当分类器是通过文件打开时可用
	/// </summary>
	void SaveClassifier(void);

	/// <summary>
	/// 从文件中读取分类器
	/// </summary>
	/// <param name="fileName">文件在计算机中的绝对路径，需要访问权限</param>
	/// <returns>是否读取成功</returns>
	bool LoadClassifier(const char* fileName);

	/// <summary>
	/// 根据已知分类器进行分类
	/// </summary>
	/// <param name="TFea">待分类的特征</param>
	/// <returns>已知分类器中与待分类特征最贴近特征的命名</returns>
	String classify(const feature& TFea);

	/// <summary>
	/// 测试用函数———输出分类器文本数据到控制台窗口
	/// </summary>
	/// <param name="">不需要参数</param>
	void PrintTClassifier_d(void);

	/// <summary>
	/// 判断特征容器中是否已存在某个特征
	/// </summary>
	/// <param name="TFea">待进行判断的特征</param>
	/// <param name="errlimit">容许误差</param>
	/// <returns>判断结果的布尔值</returns>
	bool IsExistTFeature(const feature& TFea, double errlimit = 0.15);

	/// <summary>
	/// 判断两个特征是否相似
	/// </summary>
	/// <param name="TFea_1">第一个待比较的特征</param>
	/// <param name="TFea_2">第二个待比较的特征</param>
	/// <param name="errlimit">容许误差</param>
	/// <returns>比较结果的布尔值</returns>
	static	bool IsFeatureEqual(const feature& TFea_1, const feature& TFea_2, double errlimit);

private:

	/// <summary>
	/// 判断名称容器中是否已存在某个命名
	/// </summary>
	/// <param name="name">待进行判断的命名</param>
	/// <returns>判断结果的布尔值</returns>
	bool IsExistName(const String& name);

	/// <summary>
	/// 打开分类器文件
	/// </summary>
	/// <param name="ClassifierPath">分类器文件所在的绝对路径</param>
	/// <param name="mode">文件打开模式</param>
	/// <returns>成功打开后的文件指针</returns>
	FILE* OpenFile(const char* ClassifierPath, const char* mode);

	/// <summary>
	/// 从文件流中读取特征结构
	/// </summary>
	/// <param name="fp">将要读取数据的文件流</param>
	void ReadStructure(FILE* fp);

	/// <summary>
	/// 向文件流中写入特征结构
	/// </summary>
	/// <param name="fp">将要写入数据的文件流</param>
	/// <param name="num">本类对象中第num个数据</param>
	void WriteStructure(FILE* fp, size_t num);

	/// <summary>
	/// 输出读取到的分类器的基本信息
	/// </summary>
	/// <param name="">不需要参数</param>
	void PrintClassifierMSG(void);

};

