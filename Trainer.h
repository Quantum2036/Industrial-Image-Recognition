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
	std::vector<feature> feature_Data;

//	����
public:

	//�����ݱ��浽ָ�����ļ���
	void SaveData_d(void);

private:
	
	//Ϊ�ַ���������ĸ���
	String StrAddChar(const String& str, size_t length);

	//������д��ָ���ļ�
	void FileSave(String name, std::vector<feature>& feaIn);

	//��һ��ͼ��������Ŀ���������� feature_Data
	void PushFeature(std::vector<Target>& target);

	//��ȡ�ļ�����������չ��Ϊtype���ļ�
	//	��typeΪ���ַ��������ȡ�����ļ�
	std::vector<String> getAllFile(String folderPath, String type);

	//��֤��ȡ�ļ�����Ч�ԣ��� getAllFile �������ʹ��
	//	���ܷ���imread��ȡ
	std::vector<String> VaildFileList(std::vector<String> fileList);

	//����
	void Analysis(void);
};

