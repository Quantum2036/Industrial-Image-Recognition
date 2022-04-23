#pragma once
#include "pch.h"
#include <forward_list>
#include <iterator>

class FList
{
//	构造 & 析构
public:

	FList();
	FList(const FList& flTemp);

	FList&	operator=(const FList& flIn);
	void	operator+=(const FList& flIn);
	bool	operator==(const FList& flIn) const
	{
		return list == flIn.list ? true : false;
	};

//	数据
private:
	std::forward_list<Point> list;

//	方法
public:

	//返回只读的第一个数据的指针
	std::forward_list<Point>::const_iterator	begin(void) {
		return list.cbegin();
	}

	//返回可写的第一个数据的指针
	std::forward_list<Point>::iterator			wbegin(void) {
		return list.begin();
	}

	//返回只读的最后一个数据的指针
	std::forward_list<Point>::const_iterator	end(void) {
		return list.cend();
	}

	//返回可写的最后一个数据的指针
	std::forward_list<Point>::iterator			wend(void) {
		return list.end();
	}

	//返回表长
	uint GetSize(void) {
		return (uint)std::distance( std::begin(list), std::end(list) );
	}

	//返回点的区域
	Rect GetRect(void);

	//返回点的中心
	Point GetCentre(void);

	//将点(nX,nY)有序插入到表中
	void AddData(const Point& pt);

	//将链表置空
	void Empty(void) {
		list.clear();
	}

	//判断链表是否为空
	bool IsEmpty(void) {
		return list.empty();
	}

	//判断点(nX,nY)是否被包括在表中
	bool IsExist(const Point& pt);


#if DEBUG_PRINTF

	//在控制台输出链表
	void PrintList_d(void);

	//画出链表
	void ShowList_d(const Size& size);

#endif // DEBUG_PRINTF

};

//重载运算符对点进行比较
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