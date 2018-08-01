#ifndef CHTBL_H
#define CHTBL_H

#include <stdlib.h>
#include <List/list.h>

//定义链式哈希表(chained hash tables)结构体
//@buckets		表示坑位
//@函数指针h	指定哈希函数,为了尽可能的散列
//@table		是哈希表本身
typedef struct CHTbl_{
	int                buckets;

	int                (*h)(const void *key);
	int                (*match)(const void *key1, const void *key2);
	void               (*destroy)(void *data);

	int                size;
	List               *table;
}CHTbl;

//初始化链式哈希表
int chtbl_init(CHTbl *htbl, int buckets, int (*h)(const void *key), int
		(*match)(const void *key1, const void *key2), void (*destroy)(void *data));
//销毁哈希表
void chtbl_destroy(CHTbl *htbl);
//插入表元素
int chtbl_insert(CHTbl *htbl, const void *data);
//移除表元素
int chtbl_remove(CHTbl *htbl, void **data);
//查找表元素
int chtbl_lookup(const CHTbl *htbl, void **data);

#define chtbl_size(htbl) ((htbl)->size)

#endif
