#include <stdlib.h>
#include <string.h>
#include <List/clist.h>

/* 初始化链表 */
void clist_init(CList *list, void (*destroy)(void *data))
{
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;

	return;
}

/* 销毁链表 */
void clist_destroy(CList *list)
{
	void *data;
	while (clist_size(list) > 0) {
		if (clist_rem_next(list, list->head, (void **)&data) == 0 && list->destroy != NULL) {
			/* 释放元素内存 */
			list->destroy(data);
		}
	}

	memset(list, 0, sizeof(CList));
	return;
}

int clist_ins_next(CList *list, CListElmt *element, const void *data)
{
	/* 分配元素内存 */
	CListElmt *new_element;
	if((new_element = (CListElmt *)malloc(sizeof(CListElmt))) == NULL)
		return -1;

	/* 插入元素 */
	/* 链表为空,插入元素做头结点,自己的后继节点指向自己 */
	/* 链表不为空,在element之后插入元素 */
	new_element->data = (void *)data;
	if (clist_size(list) == 0) {
		new_element->next = new_element;
		list->head = new_element;
	} else {
		new_element->next = element->next;
		element->next = new_element;
	}

	list->size++;
	return 0;
}

int clist_rem_next(CList *list, CListElmt *element, void **data)
{
	/* 链表不为空 */
	CListElmt *old_element;
	if (clist_size(list) == 0)
		return -1;

	/* 取得被删除结点元素值 */
	*data = element->next->data;

	/* 删除结点 */
	/* 链表只有一个结点或者有多个节点, 如果删除的是头节点取新头节点 */
	if (element->next == element) {
		old_element = element->next;
		list->head = NULL;
	}
	else {
		old_element = element->next;
		element->next = element->next->next;
		if (old_element == clist_head(list)) {
			list->head = old_element->next;
		}
	}

	free(old_element);
	list->size--;
	return 0;
}
