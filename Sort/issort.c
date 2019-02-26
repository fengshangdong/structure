#include <stdlib.h>
#include <string.h>
#include <Sort/sort.h>

//插入排序
int issort(void *data, int size, int esize,
           int (*compare)(const void *key1, const void *key2)) 
{
	char *a = data;
	void *key;
	int i, j;

	if ((key = (char *)malloc(esize)) == NULL)
		return -1;

	//在有序数组中插入key
	for (j = 1; j < size; j++)
	{
		memcpy(key, &a[j * esize], esize);
		i = j - 1;
		//确定插入位置,当新元素大于a[i]
		while (i >= 0 && compare(&a[i * esize], key) > 0)
		{
			//第i个元素向后移一位
			memcpy(&a[(i + 1) * esize], &a[i * esize], esize);
			i--;
		}
		memcpy(&a[(i + 1) * esize], key, esize);
	}

	free(key);
	return 0;
}
