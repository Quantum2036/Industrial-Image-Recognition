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

//	����
public:

	/// <summary>
	/// Ϊ���������е����ɫ��ֵ
	/// </summary>
	/// <param name="list">����</param>
	/// <param name="color">��ɫֵ</param>
	void DrawList(std::vector<Point> list, Scalar color);

	//����ɫ���Ŀ����ڲ��ͱ߽�
	void DrawInside(Target& obj,  Scalar color_inside = Scalar(230, 230, 230));
	void DrawPeripheral(Target& obj, Scalar color_peripheral = Scalar(150, 150, 150));

	/// <summary>
	/// ��Ŀ�����Ļ���ʮ�ּǺ�
	/// </summary>
	/// <param name="obj">Ŀ��</param>
	/// <param name="color_cross">ʮ�ּǺŵ���ɫ</param>
	void DrawCross(Target& obj, Scalar color_cross = Scalar(51, 153, 255));

	//�÷����ڻ����ϱ�ǳ�Ŀ��
	void DrawBox(Target& obj, Scalar color_box = Scalar(0, 255, 0));

	/// <summary>
	/// �ڻ�����ΪĿ���������
	/// </summary>
	/// <param name="obj">Ҫ����������ֵ�Ŀ��</param>
	/// <param name="name">Ҫ��ʾ������</param>
	void DrawText(Target& obj, String name);

private:

	/// <summary>
	/// ���Ļ�����ָ�������ɫ
	/// </summary>
	/// <param name="fl">Ҫ������ɫ�ĵ������</param>
	/// <param name="color">Ҫ���ĵ���ɫ</param>
	void SetColor(FList& fl, Scalar color);
};

