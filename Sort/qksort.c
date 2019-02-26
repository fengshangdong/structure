#include <stdlib.h>
#include <string.h>
#include <Sort/sort.h>

/// 快排是一种分治算法,分为三个步骤:
/// 1.分:设定一个分割值,将数据分为两部分
/// 2.治:分别在两部分用递归的方式继续使用快排排序
/// 3.和:对分割部分排序直至完成

//比较函数,供函数指针调用
static int compare_int(const void *int1, const void *int2)
{
	//中位数法选取一个分割值
	if (*(const int *)int1 > *(const int *)int2)
		return 1;
	else if (*(const int *)int1 < *(const int *)int2)
		return -1;
	else
		return 0;
}

// 1.分:设定一个分割值,将数据分为两部分
static int partition(void *data, int esize, int i, int k,
                     int (*compare)(const void *key1, const void *key2))
{
	char	*a = data;
	void    *pval, *temp;

	//pval保存中位数,temp临时空间用于数据交换
	if ((pval = malloc(esize)) == NULL)
		return -1;

	if ((temp = malloc(esize)) == NULL)
	{
		free(pval);
		return -1;
	}

	memcpy(pval, &a[i * esize], esize);

	//根据中位数将数组分成两部分,左边小于该中位数pval,右边大于pval
	i--;
	k++;
	while (1)
	{
		do { //左移
			k--; 
		} while(compare(&a[k * esize], pval) > 0);

		do { //右移
			i++;
		} while(compare(&a[i * esize], pval) < 0);

		if (i >= k) { //分化完成跳出循环
			break;
		} else { //左右数组分别找到一个处于错误位置的元素,进行交换
			memcpy(temp, &a[i * esize], esize);
			memcpy(&a[i * esize], &a[k * esize], esize);
			memcpy(&a[k * esize], temp, esize);
		}
	}

	free(pval);
	free(temp);
	return k; //返回中位数
}

//快排
int qksort(void *data, int size, int esize, int i, int k,
           int (*compare)(const void *key1, const void *key2))
{
	int	j;
	//不断调用递归直到无法分化
	if (i < k)
	{
		//分化数组,取得中位数j
		if ((j = partition(data, esize, i, k, compare)) < 0)
			return -1;
		//对左边进行排序
		if (qksort(data, size, esize, i, j, compare) < 0)
			return -1;
		//对右边进行排序
		if (qksort(data, size, esize, j + 1, k, compare) < 0)
			return -1;
	}
	return 0;
}
