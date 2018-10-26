#include <stdlib.h>
#include <string.h>
#include <List/list.h>

void list_init(List *list, void (*destroy)(void *data))
{
	/* 初始化list */
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;

	return;
}

void list_destroy(List *list)
{
	void *data;
	/* 移除每个元素 */
	while (list_size(list) > 0) {
		if (list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL) {
			/* 释放data的内存 */
			list->destroy(data);
		}
	}

	memset(list, 0, sizeof(List));
	return;
}

int list_ins_next(List *list, ListElmt *element, const void *data)
{
	/* 为元素分配内存 */
	ListElmt *new_element;
	if ((new_element = (ListElmt *)malloc(sizeof(ListElmt))) == NULL)
		return -1;

	/* 插入元素 */
	/* 如果element为空，data插入头部作为头节点 */
	/* 如果element不为空，data插入element节点之后 */
	new_element->data = (void *)data;
	if (element == NULL) {
		if (list_size(list) == 0) {
			list->tail = new_element;
		}

		new_element->next = list->head;
		list->head = new_element;
	}
	else {
		if (element->next == NULL) {
			list->tail = new_element;
		}

		new_element->next = element->next;
		element->next = new_element;
	}

	list->size++;
	return 0;
}

int list_rem_next(List *list, ListElmt *element, void **data)
{
	/* 判断是否为空,空链不支持删除元素 */
	ListElmt *old_element;
	if (list_size(list) == 0)
		return -1;

	/* 删除元素 */
	/* 如果element为空，删除头节点 */
	/* 如果element不为空，删除element之后的节点 */
	if (element == NULL) {
		*data = list->head->data;
		old_element = list->head;
		list->head = list->head->next;

		if (list_size(list) == 1) {
			list->tail = NULL;
		}
	}
	else {
		if (element->next == NULL) {
			return -1;
		}

		*data = element->next->data;
		old_element = element->next;
		element->next = element->next->next;

		if (element->next == NULL) {
			list->tail = element;
		}
	}

	free(old_element);
	list->size--;
	return 0;
}
