#pragma once
#include "pch.h"
#include "FList.h"
#include <array>

using fea_array = std::array<double, FEATURE_SIZE>;

typedef struct _Fearture {
	uint size				= 0;		//���
	uint peripheral			= 0;		//�ܳ�
	uint major_axis			= 0;		//MER�ĳ���
	uint minor_axis			= 0;		//MER�Ķ���
	uint isHollow			= 0;		//�Ƿ��п�
	uint corners			= 0;		//�ǵ���
	double Rectangularity	= 0.0;		//���ζ�
	double consistency		= 0.0;		//Բ�ζȣ����ܶȣ�
	double eccentricity		= 0.0;		//ƫ����
}feature;

class TFeature
{
//	���� & ����
public:
	TFeature();
	TFeature(const TFeature& fTemp);
	TFeature(FList tlist, FList plist);
	~TFeature();

	TFeature& operator=(const TFeature& fTemp);

//	����
public:

	//�����ṹ
	feature Struct_feature;

	//Ŀ�꼸������
	Point centre;

//	����
public:

	/// <summary>
	/// �������ṹת��������������ʽ���ڼ���
	/// </summary>
	/// <param name="fea">��������</param>
	/// <returns>�������</returns>
	static fea_array Struct2Array(feature fea) {
		fea_array arr{0.0};
		arr[0] = (double)fea.size;			arr[1] = (double)fea.peripheral;
		arr[2] = (double)fea.major_axis;	arr[3] = (double)fea.minor_axis;
		arr[4] = (double)fea.isHollow;		arr[5] = fea.Rectangularity;
		arr[6] = fea.consistency;			arr[7] = fea.eccentricity;

		return arr;
	}

	/// <summary>
	/// ����������ת���������ṹ��ʽ���ڴ洢��ʾ
	/// </summary>
	/// <param name="arr">��������</param>
	/// <returns>�������</returns>
	static feature Array2Struct(fea_array arr) {
		feature fea;
		fea.size		= (uint)arr[0];		fea.peripheral		= (uint)arr[1];
		fea.major_axis	= (uint)arr[2];		fea.minor_axis		= (uint)arr[3];
		fea.isHollow	= arr[4] > 0.5 ? 1 : 0;	
		fea.Rectangularity	= arr[5];
		fea.consistency		= arr[6];	
		fea.eccentricity	= arr[7];

		return fea;
	}

private:

	//������С��Ӿ���
	Size CalMER(FList& plist);

	//������ζ�
	double CalR(void);

	//����Բ�ζȣ����ܶȣ�
	double CalC(void);

	//����ƫ����
	double CalE(void);

	//������Ӿ��εĳ�����
	Size CalER(FList& plist);

	//��ת����list�еĵ㣬�Ե�PointΪ����ÿ����תangle��
	void RotateList(FList& list, int angle = 1);


#if DEBUG_PRINTF
public:
	void print_d(void);
#endif // DEBUG_PRINTF

};

