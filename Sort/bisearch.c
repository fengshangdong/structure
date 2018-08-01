#include <stdlib.h>
#include <string.h>
#include <Sort/search.h>

//二分查找,对已排序数组的查找算法
int bisearch(void *sorted, const void *target, int size, int esize, int
		(*compare)(const void *key1, const void *key2))
{
	int	left, middle, right;
	left = 0;
	right = size - 1;

	//整个数组
	while(left <= right)
	{
		//中间值middle
		middle = (left + right) / 2;
		switch(compare(((char *)sorted + (esize * middle)), target))
		{
			case -1: //中间值小于目标值,确定查找范围在右侧
				left = middle + 1;
				break;
			case 1:	 //中间值大于目标值,确定查找范围在左侧
				right = middle - 1;
				break;
			case 0:  //查找成功
				return middle;
		}
	}

	return -1;
}
