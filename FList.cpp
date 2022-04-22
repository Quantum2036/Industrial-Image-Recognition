#include "FList.h"

FList::FList()
{
	
}

FList::FList(const FList& flTemp)
{
	list = flTemp.list;
}

FList& FList::operator=(const FList& flTemp)
{
	list = flTemp.list;
	return *this;
}

void FList::operator+=(const FList& flIn)
{
	auto it		= flIn.list.cbegin();
	auto it_end	= flIn.list.cend();
	
	for (; it != it_end; it++) {
		AddData(*it);
	}

	list.unique();
}

void FList::AddData(const Point& pt)
{
	//如果链表为空，在第一个位置插入元素
	if ( IsEmpty() ) {
		list.emplace_front(pt);
		return;
	}

	auto it = list.cbegin();
	auto it_end = list.cend();
	auto temp = it;	//it的上一个指针

	if (pt < *it++) {	//pt小于第一个元素
		list.emplace_front(pt);
		return;
	}

	while (it != it_end) {
		if (pt < *it) {
			list.emplace_after(temp, pt);
			return;
		}
		temp = it;
		it++;
	}

	//pt大于所有元素
	list.emplace_after(temp, pt);
 }

Rect FList::GetRect(void)
{
	if (list.empty()) {
		return Rect(-1,-1,0,0);
	}

	auto it		= list.cbegin();
	auto it_end = list.cend();

	int minX = it->x;
	int maxX = it->x;
	int minY = it->y;
	int maxY = it->y;

	for (; it != it_end; it++) {
		if (it->x < minX) minX = it->x;
		if (it->x > maxX) maxX = it->x;
		if (it->y < minY) minY = it->y;
		if (it->y > maxY) maxY = it->y;
	}

	return Rect(minX, minY, maxX - minX, maxY - minY);
}

Point FList::GetCentre(void)
{
	auto it = list.cbegin();
	auto it_end = list.cend();

	uint sumX = 0;
	uint sumY = 0;
	uint sum = 0;

	for (; it != it_end; it++) {
		sumX += it->x;
		sumY += it->y;
		sum++;
	}

	if (sum) {
		return Point((int)(sumX / (double)sum), (int)(sumY / (double)sum));
	}
	else
	{
		return Point(-1, -1);
	}
}

bool FList::IsExist(const Point& pt)
{
	auto it		= list.cbegin();
	auto it_end	= list.cend();

	for (; it != it_end; it++) {
		if (pt.x > it->x) {
			continue;
		}
		else if (pt.x == it->x)
		{
			if (pt.y > it->y) 
			{
				continue;
			}
			else if (pt.y == it->y)
			{
				return true;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	return false;
}




#if DEBUG_PRINTF

void FList::PrintList_d(void)
{
	if (IsEmpty()) {
		fprintf_s(stdout, "链表为空");
	}

	uint count	= 0;
	auto it		= list.cbegin();
	auto it_end = list.cend();

	for (; it != it_end; it++) {
		fprintf_s(stdout, "No.%u \t x: [%u] \t y: [%u]\n", ++count, it->x, it->y);
	}
}

void FList::ShowList_d(const Size& size)
{
	Mat Img = Mat::zeros(size, CV_8UC1);

	auto it		= list.cbegin();
	auto it_end = list.cend();

	for (; it != it_end; it++) {
		Img.at<uchar>(*it) = 255;
	}

	namedWindow("Show List");
	imshow("Show List", Img);
	waitKey(500);
	destroyWindow("Show List");

}

#endif // DEBUG_PRINTF

