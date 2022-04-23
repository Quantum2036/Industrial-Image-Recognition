#pragma once
#include "pch.h"
#include <forward_list>
#include <iterator>

class FList
{
//	���� & ����
public:

	FList();
	FList(const FList& flTemp);

	FList&	operator=(const FList& flIn);
	void	operator+=(const FList& flIn);
	bool	operator==(const FList& flIn) const
	{
		return list == flIn.list ? true : false;
	};

//	����
private:
	std::forward_list<Point> list;

//	����
public:

	//����ֻ���ĵ�һ�����ݵ�ָ��
	std::forward_list<Point>::const_iterator	begin(void) {
		return list.cbegin();
	}

	//���ؿ�д�ĵ�һ�����ݵ�ָ��
	std::forward_list<Point>::iterator			wbegin(void) {
		return list.begin();
	}

	//����ֻ�������һ�����ݵ�ָ��
	std::forward_list<Point>::const_iterator	end(void) {
		return list.cend();
	}

	//���ؿ�д�����һ�����ݵ�ָ��
	std::forward_list<Point>::iterator			wend(void) {
		return list.end();
	}

	//���ر�
	uint GetSize(void) {
		return (uint)std::distance( std::begin(list), std::end(list) );
	}

	//���ص������
	Rect GetRect(void);

	//���ص������
	Point GetCentre(void);

	//����(nX,nY)������뵽����
	void AddData(const Point& pt);

	//�������ÿ�
	void Empty(void) {
		list.clear();
	}

	//�ж������Ƿ�Ϊ��
	bool IsEmpty(void) {
		return list.empty();
	}

	//�жϵ�(nX,nY)�Ƿ񱻰����ڱ���
	bool IsExist(const Point& pt);


#if DEBUG_PRINTF

	//�ڿ���̨�������
	void PrintList_d(void);

	//��������
	void ShowList_d(const Size& size);

#endif // DEBUG_PRINTF

};

//����������Ե���бȽ�
inline bool	operator==(const Point& pt_1, const Point& pt_2) 
{
	return (pt_1.x == pt_2.x && pt_1.y == pt_2.y) ? true : false;
}

inline bool operator<(const Point& pt_1, const Point& pt_2)
{
	if (pt_1.x < pt_2.x)
	{
		return true;
	}
	else if (pt_1.x == pt_2.x)
	{
		if (pt_1.y < pt_2.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}