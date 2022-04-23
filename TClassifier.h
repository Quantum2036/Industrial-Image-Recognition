#pragma once
#include "pch.h"
#include "TFeature.h"
#include <vector>

class TClassifier
{
//	���� & ����
public:
	TClassifier();
	TClassifier(const TClassifier& tc);
	~TClassifier();

	TClassifier& operator=(const TClassifier& tc);

//	����
private:
	//��¼��������Ŀ
	size_t count;

	//�����ṹ���洢����������
	std::vector<feature> data;

	//�����ݶ�Ӧ�������ַ���
	std::vector<String> Tname;

public:

	static String strfeatureName[];

//	����
public:

	//�����������ݱ��浽�ļ�
	void SaveClassifier(void);

	//���ļ��ж�ȡ������
	void LoadClassifier(const char* filePath);

	//����±�ʶ
	void AddNewClass(feature TFea, String name);

	//��������ݣ��������ظ�
	void AddNewData(feature TFea, String name) {
		data.emplace_back(TFea);
		Tname.emplace_back(name);
		count++;
	}

	//���ؼ�¼�ĳ���
	size_t getSize(void) {
		return count;
	}

	std::vector<feature> getDataVector(void) {
		return data;
	}

	//���ص�����ͷ
	std::vector<feature>::const_iterator	getDataBegin(void) {
		return data.cbegin();
	}

	//���ص�����β
	std::vector<feature>::const_iterator	getDataEnd(void) {
		return data.cend();
	}

	//���ص�i������
	feature getFeatureAt(size_t i) {
		return data[i];
	}

	//���ص�i������
	String getNameAt(size_t i) {
		return Tname.at(i);
	}

	//������м�¼
	void clear(void) {
		count = 0;
		data.clear();
		Tname.clear();
	}

	//����Ŀ�������ж�Ŀ�����ͣ�errΪ�����������
	String getTC(const feature& TFea);

	//�ж������Ƿ�Ϊ��
	bool IsEmpty(void) {
		return data.empty();
	}

	//���������������̨����
	void PrintTClassifier_d(void);

	//�ж��������Ƿ��������TFea
	bool IsExistTFeature(const feature& TFea, double errlimit = 0.15);

	//�ж����������Ƿ����
	static	bool IsFeatureEqual(const feature& TFea_1, const feature& TFea_2, double errlimit);

private:

	//�ж��������Ƿ��������name
	bool IsExistName(const String& name);

	//�򿪷������ļ�
	FILE* OpenFile(const char* ClassifierPath, const char* mode);

	//��ȡ���ݽṹ
	void ReadStructure(FILE* fp);

	//д�����ݽṹ
	void WriteStructure(FILE* fp, size_t count);

	
};

