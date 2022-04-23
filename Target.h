#pragma once
#include "pch.h"
#include "TClassifier.h"
#include "TInformation.h"

//���򲶻񵽵�Ŀ�꣬��˿����ñ֮��������
class Target
{
/*************���� & ����*************/
public:
	Target();
	Target(const Target& tTemp);
	Target(Mat& pImage, Point seed);
	~Target();

	Target& operator=(const Target& tTemp);
	friend class TargetDisplay;

/*************����*************/
public:

	//Ŀ����Ϣ
	TInformation TInfo;

	//Ŀ�������
	TFeature TFea;

private:

	//����-�������õ�ͼ��ָ��
	Mat* pImg;

	//�������������һЩ��Ϣ
	//	��App����ʹ�ú��� SetCanvas() �ֶ��ṩ,
	//	����Ϊ����ʱ�õ�ͼ��
	Mat* canvas;

	//Ŀ���ڲ������е�
	FList tlist;
	
	//Ŀ��ı߽�㣨����Ŀ���ڣ�
	FList plist;

	//����ͼ��Ĵ�СRect
	Rect Isize;
	       
	//Ŀ��״̬
	TargetState TState;

/*************����*************/
public:

	//����Ŀ���״̬��Ϣ
	TargetState GetTargetState(void) {
		return TState;
	}

private:

	//�Ե㣨nX��nY�������������н������pt_Target�����ܵ����pt_Peripheral
	void Grow(Point seed);

	//�ж�Ŀ���Ƿ񳬳�ͼ��
	bool IsOutImg(void);

};

