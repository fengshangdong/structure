#ifndef SET_H
#define SET_H

#include <stdlib.h>
#include <List/list.h>

//Set也是链表结构
typedef List Set;

//初始化
void set_init(Set *set, int (*match)(const void *key1, const void *key2),
              void (*destroy)(void *data));

//销毁
#define set_destroy list_destroy

//插入&删除
int set_insert(Set *set, const void *data);
int set_remove(Set *set, void **data);

//并集&交集&差集
int set_union(Set *setu, const Set *set1, const Set *set2);
int set_intersection(Set *seti, const Set *set1, const Set *set2);
int set_difference(Set *setd, const Set *set1, const Set *set2);

int set_is_member(const Set *set, const void *data);
int set_is_subset(const Set *set1, const Set *set2);
int set_is_equal(const Set *set1, const Set *set2);

#define set_size(set) ((set)->size)

#endif
