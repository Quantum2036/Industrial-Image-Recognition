#pragma once
#include "pch.h"
#include "CVApp.h"

/// <summary>
/// ѵ������--����ͬ������Ĵ���ͼ�����еõ���������
///		ʹ�������ù��캯����ʼ������Ĭ�ϣ�
///		�ٱ������ݵ��ļ�����
/// </summary>
class Trainer
{
//	���� & ����
public:
	Trainer();
	Trainer(Mat& background, const char* folderPath, const char* fileType = "");

//	����
private:

	//Ŀ������
	String target_Name;

	//����ɨ����Ŀ�������
	TClassifier feature_Data;

	//������
	TClassifier target_Class;

//	����
public:

	//����������ļ�
	void SaveTClassifier(const char* Target_Name, const char* SavePath);

private:

	//	8 �̴߳�������ͼƬ
	void threadProcess_8(std::vector<String> image_names, Mat& background);

	//�����ݱ��浽ָ�����ļ���
	void SaveData_d(String& Save_Full_Path, std::vector<feature>& feaIn);

	//Ϊ�ַ���������ĸ���
	String StrAddChar(const String& str, size_t length);

	//��һ��ͼ��������Ŀ���������� feature_Data
	void PushFeatureData(std::vector<Target>& target, String fileName);

	//��ȡ�ļ�����������չ��Ϊtype���ļ�
	//	��typeΪ���ַ��������ȡ�����ļ�
	std::vector<String> getAllFile(String folderPath, String type);

	//��֤��ȡ�ļ�����Ч�ԣ��� getAllFile �������ʹ��
	//	���ܷ���imread��ȡ
	std::vector<String> VaildFileList(std::vector<String> fileList);

	//����
	void Analysis(void);

	//����ɸѡ��errlimitΪ����������[20-30]
	void Sieve_1(double errlimit = 0.30);

	//����ɸѡ
	void Sieve_2(double errlimit = 0.15);

	/// <summary>
	/// ��һ������������ƽ������ֵ
	/// </summary>
	/// <param name="feas">���ڼ���ƽ��ֵ����������</param>
	/// <returns>ƽ������ֵ</returns>
	feature getAverageFeature(const std::vector<feature>& feas);
};

