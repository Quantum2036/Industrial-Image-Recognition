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

	//���������ݣ���Ų�ͬĿ�����͵ı�ʶ����
	//	��App����ʹ�ú����ֶ�����
	//	���򽫸���ͼ�񴴽��µķ�����
	TClassifier* TCdata;

	//Ŀ���ڲ������е�
	FList tlist;
	
	//Ŀ��ı߽�㣨����Ŀ���ڣ�
	FList plist;

	//����ͼ��Ĵ�СRect
	Rect Isize;
	       
	//Ŀ��״̬
	TargetState TState;

	//Ŀ����������
	String TClass;

/*************����*************/
public:

	//�ֶ����û���
	void SetCanvas(Mat* pImgCanvas);

	//����Ŀ���״̬��Ϣ
	TargetState GetTargetState(void) {
		return TState;
	}

	//����ɫ���Ŀ����ڲ��ͱ߽�
	void DrawColor(uchar color_inside = COLOR_INSIDE_8UC1, uchar color_peripheral = COLOR_PERIPHERAL_8UC1);

	//����ɫ�����ڻ����ϱ�ǳ�Ŀ�꣬��Ϊ���������
	void DrawBox(Scalar color_box = Scalar(0, 255, 0), Scalar color_text = Scalar(0, 0, 255));

private:

	//�Ե㣨nX��nY�������������н������pt_Target�����ܵ����pt_Peripheral
	void Grow(Point seed);

	//�ж�Ŀ���Ƿ񳬳�ͼ��
	bool IsOutImg(void);

	//��Target���е�ͼ��Ӧ����ɫ����
	void SetColor(FList& fl, uchar color);

};

