#pragma once
#include "pch.h"

//Բ��ģ��ƥ��
class CircleMatching
{
//	���� &������
public:
	CircleMatching();
	CircleMatching(Mat* ptest, const uchar& radius);

//	����
private:

	//Ҫ����ƥ���ģ�壬����Ϊ��ֵͼ��
	Mat* pImg;

	//ƥ�����õĽṹԪ��
	Mat SE;

	//�ṹԪ�صĳ���
	int SE_length;

	//�ṹԪ�صİ뾶
	int SE_radius;

//	����
public:

	//����ƥ��̶ȣ�����ƥ�䷵��1.0
	//	�ڵ�Pt����ģ��ƥ�䣬ƥ���õ���ɫΪcolor
	double Todo(const Point& pt, const uchar color);

private:

	//��������Ч��
	bool IsValidPoint(const int nX, const int nY) {
		return (nX > 0 && nY > 0 && nX + SE_radius < pImg->cols && nY + SE_radius  < pImg->rows);
	}
};

