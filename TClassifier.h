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

	//����±�ʶ
	void AddNewClass(const feature& TFea, const String& name);

	//�����������ݱ��浽�ļ�
	void SaveClassifier(void);

	//���ļ��ж�ȡ������
	void LoadClassifier(const char* filePath);

	//����Ŀ�������ж�Ŀ�����ͣ�errΪ�����������
	String getTC(const feature& TFea);

	//�ж������Ƿ�Ϊ��
	bool IsEmpty(void) {
		return data.empty();
	}

	//���������������̨����
	void PrintTClassifier_d(void);

	//�ж����������Ƿ����
	static	bool IsFeatureEqual(const feature& TFea_1, const feature& TFea_2, double err);

private:

	//�ж��������Ƿ��������TFea
	bool IsExistTFeature(const feature& TFea);

	//�ж��������Ƿ��������name
	bool IsExistName(const String& name);

	//�򿪷������ļ�
	FILE* OpenFile(const char* ClassifierPath, const char* mode);

	//��ȡ���ݽṹ
	void ReadStructure(FILE* fp);

	//д�����ݽṹ
	void WriteStructure(FILE* fp, size_t count);

	
};

