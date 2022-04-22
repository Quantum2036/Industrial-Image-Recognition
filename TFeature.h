#pragma once
#include "pch.h"
#include "FList.h"

typedef struct _Fearture {
	uint size				= 0;		//���
	uint Peripheral			= 0;		//�ܳ�
	Size MER				= Size();	//��С��Ӿ���
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
	feature TFea;

	//Ŀ�꼸������
	Point centre;

//	����
private:

	//������С��Ӿ���
	Size CalMER(FList& plist);

	//����ǵ���-----δ���
	uint CalCorners(void);

	//������ζ�
	double CalR(void);

	//����Բ�ζȣ����ܶȣ�
	double CalC(void);

	//����ƫ����
	double CalE(void);

	//������Ӿ��εĳ�����
	Size CalER(FList& plist);

	//��ת����list�еĵ㣬�Ե�PointΪ����ÿ����תangle��
	void RotateList(FList& list, int angle = 3);


#if DEBUG_PRINTF
public:
	void print_d(void);
#endif // DEBUG_PRINTF

};

