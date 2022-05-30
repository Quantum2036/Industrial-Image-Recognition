#pragma once
#include "pch.h"

/// <summary>
/// Բ��ģ��ƥ��
/// </summary>
class CircleMatching
{
//	���� &������
public:
	/// <summary>
	/// Ψһ���캯��
	/// </summary>
	/// <param name="ptest">Ҫ����ģ��ƥ���ͼ��ָ�룬����Ϊ��ֵͼ��</param>
	/// <param name="radius">�ṹԪ�صİ뾶</param>
	CircleMatching(Mat* ptest, const uchar& radius);

//	����
private:

	//Ҫ����ģ��ƥ���ͼ��ָ�룬����Ϊ��ֵͼ��
	Mat* pImg;

	//ƥ�����õĽṹԪ��
	Mat SE;

	//�ṹԪ�صĳ���
	int SE_length;

	//�ṹԪ�صİ뾶
	int SE_radius;

//	����
public:
	
	/// <summary>
	/// ����ģ��ƥ��
	/// </summary>
	/// <param name="pt">����ģ��ƥ������ĵ�</param>
	/// <param name="color">����ģ��ƥ�����ɫֵ</param>
	/// <returns>����ģ�巶Χ����ɫֵΪ color �ı�����������ƥ�䷵��1.0</returns>
	double Todo(const Point& pt, const uchar color);

private:

	/// <summary>
	/// ����Ҫ�Ƚϵĵ��Ƿ���ͼ��Χ��
	/// </summary>
	/// <param name="nX">�����ڵ�����</param>
	/// <param name="nY">�����ڵ�����</param>
	/// <returns>����ͼ��Χ��Ϊtrue������Ϊfalse</returns>
	bool IsValidPoint(const int nX, const int nY) {
		return (nX > 0 && nY > 0 && nX + SE_radius < pImg->cols && nY + SE_radius  < pImg->rows);
	}
};

