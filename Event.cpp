#include "Event.h"
#include "Target.h"

void onMouse(int event, int x, int y, int, void* pImg)
{
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		OnLButtonDown(x, y, (Mat*)pImg);
		break;
	default:
		break;
	}
}

void OnLButtonDown(int x, int y, Mat* pImg)
{

}

