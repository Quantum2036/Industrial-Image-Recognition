#pragma once
#include "pch.h"
#include "Target.h"

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

	//����ɫ���Ŀ����ڲ��ͱ߽�
	void DrawInside(Target& obj,  Scalar color_inside = Scalar(230, 230, 230));
	void DrawPeripheral(Target& obj, Scalar color_peripheral = Scalar(150, 150, 150));

	//��Ŀ�����Ļ���ʮ��
	void DrawCross(Target& obj, Scalar color_cross = Scalar(51, 153, 255));

	//�÷����ڻ����ϱ�ǳ�Ŀ��
	void DrawBox(Target& obj, Scalar color_box = Scalar(0, 255, 0));

	//�������
	void DrawText(Target& obj, String name);

private:

	void SetColor(FList& fl, Scalar color);
};

