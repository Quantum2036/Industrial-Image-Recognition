#pragma once
#include "pch.h"
#include "Target.h"

/// <summary>
/// �ڻ���ͼ������ʾĿ��Target��ΪTarget�����Ԫ
/// </summary>
class TargetDisplay
{
//	���� & ����
public:
	TargetDisplay();
	TargetDisplay(Mat* pImgCanvas);

//	����
private:
	//����
	Mat* canvas;

public:

	static const Scalar red;
	static const Scalar green;
	static const Scalar orange;
	static const Scalar black50;
	static const Scalar black20;

//	����
public:

	/// <summary>
	/// ���Ļ�����ָ�������ɫ
	/// </summary>
	/// <param name="fl">Ҫ������ɫ�ĵ�����꼯��</param>
	/// <param name="color">Ҫ���ĵ���ɫ</param>
	void DrawList(FList& list, Scalar color);

	/// <summary>
	/// ����ɫ���Ŀ����ڲ�
	/// </summary>
	/// <param name="obj">Ҫ����Ŀ��</param>
	/// <param name="color_inside">�����ɫ</param>
	void DrawInside(Target& obj,  Scalar color_inside = black20);

	/// <summary>
	/// ����ɫ���Ŀ��ı߽�
	/// </summary>
	/// <param name="obj">Ҫ����Ŀ��</param>
	/// <param name="color_peripheral">�����ɫ</param>
	void DrawPeripheral(Target& obj, Scalar color_peripheral = black50);

	/// <summary>
	/// ��Ŀ�����Ļ�����ɫ��ʮ�ּǺ�
	/// </summary>
	/// <param name="obj">Ҫ��ǵ�Ŀ��</param>
	void DrawCross(Target& obj);

	/// <summary>
	/// �÷����ڻ����ϱ�ǳ�Ŀ��
	/// </summary>
	/// <param name="obj">Ҫ��ǵ�Ŀ��</param>
	/// <param name="color">������ɫ</param>
	void DrawBox(Target& obj, Scalar color = green);

	/// <summary>
	/// �ڻ�����ΪĿ���������
	/// </summary>
	/// <param name="obj">Ҫ����������ֵ�Ŀ��</param>
	/// <param name="name">Ҫ��ʾ������</param>
	/// <param name="color">���ֵ���ɫ</param>
	void DrawText(Target& obj, String name, Scalar color = red);

private:

	/// <summary>
	/// �����������ص�ķ�ɫ
	/// </summary>
	/// <param name="pt"></param>
	/// <returns></returns>
	void Reverse(Point pt);

};

