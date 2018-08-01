#include <stdlib.h>
#include <string.h>
#include <Sort/sort.h>

//合并两个有序集,将左数组a[i, ..., j]和右数组a[j+1, ..., k]合并
static int merge(void *data, int esize, int i, int j, int k, int (*compare)
		(const void *key1, const void *key2))
{
	char	*a = data, *m;
	int     ipos, jpos, mpos;
	ipos = i;
	jpos = j + 1;
	mpos = 0;

	//分配合并需要的内存空间大小
	if((m = (char *)malloc(esize * ((k - i) + 1))) == NULL)
		return -1;

	//ipos是左数组a[]的起始位置,jpos是右数组a[]的起始位置
	while(ipos <= j || jpos <= k){
		if(ipos > j){ //左边数组排序完成,左数组已经没有数据可用
			while(jpos <= k){ //将右数组元素拷贝到m[]中
				memcpy(&m[mpos * esize], &a[jpos * esize], esize);
				jpos++;
				mpos++;
			}
			continue; //排序完成,提前跳出循环
		}else if(jpos > k){ //右边数组排序完成,右数组已经没有数据可用
			while(ipos <= j){ //将左数组元素拷贝到m[]中
				memcpy(&m[mpos * esize], &a[ipos * esize], esize);
				ipos++;
				mpos++;
			}
			continue; //排序完成,提前跳出循环
		}

		//比较左数组和右数组元素大小,追加拷贝到m[]
		if(compare(&a[ipos * esize], &a[jpos * esize]) < 0){ //左边数组数据较小
			memcpy(&m[mpos * esize], &a[ipos * esize], esize); //左边数组数据拷贝到m[]
			ipos++;
			mpos++;
		}else{ //右边数据较小
			memcpy(&m[mpos * esize], &a[jpos * esize], esize); //右边数组数据拷贝到m[]
			jpos++;
			mpos++;
		}
	}

	//将m[]拷贝到a[]中
	memcpy(&a[i * esize], m, esize * ((k - i) + 1));
	//释放m[]内存
	free(m);
	return 0;
}

//归并排序
int mgsort(void *data, int size, int esize, int i, int k, int (*compare)
		(const void *key1, const void *key2))
{
	int	j;
	//将数组a[]不断的对半分割
	if(i < k)
	{
		//j是中间点
		j = (int)(((i + k - 1)) / 2);
		//递归的对分化后的数组合并排序
		if(mgsort(data, size, esize, i, j, compare) < 0)		{ return -1; }
		if(mgsort(data, size, esize, j + 1, k, compare) < 0)	{ return -1; }
		//合并连个已排序数组
		if(merge(data, esize, i, j, k, compare) < 0)			{ return -1; }
	}

	return 0;
}
