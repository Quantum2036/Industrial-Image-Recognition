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

	//�����ṹ�������Ĳ�ͬ������
	static String strfeatureName[];

//	����
public:

	//�жϷ������Ƿ�Ϊ��
	bool IsEmpty(void) {
		return count == 0;
	}

	//��շ������������м�¼
	void clear(void) {
		count = 0;
		data.clear();
		Tname.clear();
	}

	//�����з������м�¼������
	size_t getSize(void) {
		return count;
	}

	//�������ݵ�Vector����
	std::vector<feature>& getDataVector(void) {
		return data;
	}

	std::vector<feature>::const_iterator getDataBegin(void) {
		return data.cbegin();
	}

	std::vector<feature>::const_iterator getDataEnd(void) {
		return data.cend();
	}

	//���ص�i����¼������
	feature getFeatureAt(size_t i) {
		return data[i];
	}

	//���ص�i����¼������
	String getNameAt(size_t i) {
		return Tname.at(i);
	}

	//��������ݣ��������ظ�
	void AddNewData(const feature& TFea, const String& name) {
		data.emplace_back(TFea);
		Tname.emplace_back(name);
		count++;
	}
	
	//��������ͣ�������Ƿ��ظ�
	void AddNewClass(const feature& TFea, const String& name);

	/// <summary>
	/// �����������ݱ��浽ָ��·��
	/// </summary>
	/// <param name="fileName">��Ҫ�����������λ��</param>
	void SaveClassifier(const char* fileName);

	/// <summary>
	/// �����������������������ͨ���ļ���ʱ����
	/// </summary>
	void SaveClassifier(void);

	/// <summary>
	/// ���ļ��ж�ȡ������
	/// </summary>
	/// <param name="fileName">�ļ��ڼ�����еľ���·������Ҫ����Ȩ��</param>
	/// <returns>�Ƿ��ȡ�ɹ�</returns>
	bool LoadClassifier(const char* fileName);

	/// <summary>
	/// ������֪���������з���
	/// </summary>
	/// <param name="TFea">�����������</param>
	/// <returns>��֪�����������������������������������</returns>
	String classify(const feature& TFea);

	/// <summary>
	/// �����ú�������������������ı����ݵ�����̨����
	/// </summary>
	/// <param name="">����Ҫ����</param>
	void PrintTClassifier_d(void);

	/// <summary>
	/// �ж������������Ƿ��Ѵ���ĳ������
	/// </summary>
	/// <param name="TFea">�������жϵ�����</param>
	/// <param name="errlimit">�������</param>
	/// <returns>�жϽ���Ĳ���ֵ</returns>
	bool IsExistTFeature(const feature& TFea, double errlimit = 0.15);

	/// <summary>
	/// �ж����������Ƿ�����
	/// </summary>
	/// <param name="TFea_1">��һ�����Ƚϵ�����</param>
	/// <param name="TFea_2">�ڶ������Ƚϵ�����</param>
	/// <param name="errlimit">�������</param>
	/// <returns>�ȽϽ���Ĳ���ֵ</returns>
	static	bool IsFeatureEqual(const feature& TFea_1, const feature& TFea_2, double errlimit);

private:

	/// <summary>
	/// �ж������������Ƿ��Ѵ���ĳ������
	/// </summary>
	/// <param name="name">�������жϵ�����</param>
	/// <returns>�жϽ���Ĳ���ֵ</returns>
	bool IsExistName(const String& name);

	/// <summary>
	/// �򿪷������ļ�
	/// </summary>
	/// <param name="ClassifierPath">�������ļ����ڵľ���·��</param>
	/// <param name="mode">�ļ���ģʽ</param>
	/// <returns>�ɹ��򿪺���ļ�ָ��</returns>
	FILE* OpenFile(const char* ClassifierPath, const char* mode);

	/// <summary>
	/// ���ļ����ж�ȡ�����ṹ
	/// </summary>
	/// <param name="fp">��Ҫ��ȡ���ݵ��ļ���</param>
	void ReadStructure(FILE* fp);

	/// <summary>
	/// ���ļ�����д�������ṹ
	/// </summary>
	/// <param name="fp">��Ҫд�����ݵ��ļ���</param>
	/// <param name="num">��������е�num������</param>
	void WriteStructure(FILE* fp, size_t num);

	/// <summary>
	/// �����ȡ���ķ������Ļ�����Ϣ
	/// </summary>
	/// <param name="">����Ҫ����</param>
	void PrintClassifierMSG(void);

};

