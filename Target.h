#pragma once
#include "pch.h"
#include "FList.h"
#include <array>

//	��������
using fea_array = std::array<double, FEATURE_SIZE>;

//	�����ṹ
typedef struct _Fearture {
	uint size				= 0;		//���
	uint peripheral			= 0;		//�ܳ�
	uint major_axis			= 0;		//MER�ĳ���
	uint minor_axis			= 0;		//MER�Ķ���
	uint isHollow			= 0;		//�Ƿ��п�
	double Rectangularity	= 0.0;		//���ζ�
	double consistency		= 0.0;		//Բ�ζȣ����ܶȣ�
	double eccentricity		= 0.0;		//ƫ����
}feature;

/// <summary>
/// ���򲶻񵽵�Ŀ�꣬��˿����ñ֮��������
/// </summary>
class Target
{
//	���� & ����
public:
	Target();
	Target(const Target& tTemp);
	Target(Mat& pImage, Point seed);
	~Target();

	Target& operator=(const Target& tTemp);
	friend class TargetDisplay;

//	����
private:

	//����-�������õ�ͼ��ָ��
	Mat* pImg;

	//Ŀ���С�Ĳ�������
	Mat TRectImg;

	//Ŀ���ڲ������е�
	FList tlist;
	
	//Ŀ��ı߽�㣨����Ŀ���ڣ�
	FList plist;

	//����ͼ��Ĵ�С
	Rect rect_image;

	//Ŀ����ԭͼ�������������Ӿ��Σ�
	//	���ڽ���������ȡ
	Rect rect_Target;

	//���Ŀ��ľ�������Դ�����Ӿ���
	//	���ڽ���Ŀ�������
	Rect rect_Box;

	//Ŀ�꼸������
	Point centre;

	//�����ṹ
	feature struct_feature;

	//Ŀ��״̬
	TargetState TState;

//	����
public:

	//����Ŀ�꼸������
	Point GetCentre(void) {
		return centre;
	}

	//����Ŀ�������ṹ
	feature GetFeature(void) {
		return struct_feature;
	}

	//����Ŀ��״̬
	TargetState GetTargetState(void) {
		return TState;
	}

	//�ж�һ�����Ƿ���Ŀ���ڲ�
	bool IsInside(Point pt);

	/// <summary>
	/// ʹ��SUSAN��������ǵ�
	/// </summary>
	/// <returns>�ǵ㼯��</returns>
	FList CalCorners(void);

	/// <summary>
	/// �������ṹת��������������ʽ���ڼ���
	/// </summary>
	/// <param name="fea">��������</param>
	/// <returns>�������</returns>
	static fea_array Struct2Array(feature fea) {
		fea_array arr{ 0.0 };
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
		fea.size = (uint)arr[0];		fea.peripheral = (uint)arr[1];
		fea.major_axis = (uint)arr[2];		fea.minor_axis = (uint)arr[3];
		fea.isHollow = arr[4] > 0.5 ? 1 : 0;
		fea.Rectangularity = arr[5];
		fea.consistency = arr[6];
		fea.eccentricity = arr[7];

		return fea;
	}

private:

	//�ж�Ŀ���Ƿ񳬳�ͼ��
	bool IsOutImg(void);

	//�ж�Ŀ���Ƿ��п�
	bool IsHollow(void);

	/// <summary>
	/// ��һ�����������
	/// �����������tlist��
	/// ���ܵ����plist
	/// </summary>
	/// <param name="seed">������</param>
	void Grow(Point seed);

	//����Ŀ����Ӿ���
	void CalRectTarget(void) {
		rect_Target = plist.GetRect();
	}

	//����Ŀ��bool����
	void CalRectMat(void);

	//����Ŀ��������
	void CalRectBox(void);

	//��������
	void CalFeature(void);

	//������С��Ӿ���
	Size CalMER();

	//������Ӿ��εĳ�����
	Size CalER(FList& plist);

	//������ζ�
	double CalR(void);

	//����Բ�ζȣ����ܶȣ�
	double CalC(void);

	//����ƫ����
	double CalE(void);

	//��ת����list�еĵ㣬�Ե�PointΪ����ÿ����תangle��
	void RotateList(FList& list, int angle = 1);

	//ʹrect������ʼ��λ��ͼ��߽���
	void normalizeRect(const Mat* pImg, Rect& rect)
	{
		if (rect.x < 0) {
			rect.x = 0;
		}
		if (rect.y < 0) {
			rect.y = 0;
		}
		if (rect.x + rect.width > pImg->cols) {
			rect.width = pImg->cols - rect.x;
		}
		if (rect.y + rect.height > pImg->rows) {
			rect.height = pImg->rows - rect.y;
		}
	}

	//��ͼ������任����ʶ��������
	Point PtConvers(const Point& pt)
	{
		return Point(pt.x - rect_Box.x, pt.y - rect_Box.y);
	}
};

