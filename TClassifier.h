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

	//�ļ�������·��
	String filePath;

public:

	static String strfeatureName[];

//	����
public:

	//��δ�����滻��name
	void ChangeName(String name);

	//�����������ݱ��浽�ļ�
	void SaveClassifier(const char* fileName);
	void SaveClassifier(void);

	//���ļ��ж�ȡ������
	bool LoadClassifier(const char* fileName);

	//��������ͣ�������Ƿ��ظ�
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

	//����Ŀ������ǿ�Ʒ���
	String classify(const feature& TFea);

	//�ж������Ƿ�Ϊ��
	bool IsEmpty(void) {
		return data.empty();
	}

	//�����ú�������������������ı����ݵ�����̨����
	void PrintTClassifier_d(void);

	//�ж��������Ƿ��������TFea
	bool IsExistTFeature(const feature& TFea, double errlimit = 0.15);

	//�ж����������Ƿ�����
	static	bool IsFeatureEqual(const feature& TFea_1, const feature& TFea_2, double errlimit);

private:

	//�ж��������Ƿ��������name
	bool IsExistName(const String& name);

	//�򿪷������ļ�
	FILE* OpenFile(const char* ClassifierPath, const char* mode);

	//���ļ���fp�ж�ȡ�����ṹ
	void ReadStructure(FILE* fp);

	//���ļ���fp��д�������ṹ
	void WriteStructure(FILE* fp, size_t count);

	//��ӡ������������Ϣ
	void PrintClassifierMSG(void);


};

