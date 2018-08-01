#include <stdlib.h>
#include <string.h>
#include <List/dlist.h>

//初始化双向链表
void dlist_init(DList *list, void (*destroy)(void *data))
{
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;

	return ;
}

//销毁链表
void dlist_destroy(DList *list)
{
	void *data;
	while(dlist_size(list) > 0){ //移除每个元素
		if(dlist_remove(list, dlist_tail(list), (void **)&data) == 0 
				&& list->destroy != NULL){

			//释放分配的结点元素空间
			list->destroy(data);
		}
	}

	memset(list, 0, sizeof(DList));
	return ;
}

int dlist_ins_next(DList *list, DListElmt *element, const void *data)
{
	//不支持在非空链的插入位置元素为空
	DListElmt *new_element;
	if(element == NULL && dlist_size(list) != 0) { return -1; }

	if((new_element = (DListElmt *)malloc(sizeof(DListElmt))) == NULL)
		return -1;

	//插入元素
	new_element->data = (void *)data;
	if(dlist_size(list) == 0){ //插入空链
		list->head = new_element;
		list->head->prev = NULL;
		list->head->next = NULL;
		list->tail = new_element;
	}else{ //插入非空链表element元素之后
		new_element->next = element->next;
		new_element->prev = element;

		if(element->next == NULL) //如果插入尾结点之后
			list->tail = new_element;
		else
			element->next->prev = new_element;

		element->next = new_element;
	}

	list->size++;
	return 0;
}

int dlist_ins_prev(DList *list, DListElmt *element, const void *data)
{
	//不支持在非空链的插入位置元素为空
	DListElmt *new_element;
	if(element == NULL && dlist_size(list) != 0) { return -1; }

	if((new_element = (DListElmt *)malloc(sizeof(DListElmt))) == NULL)
		return -1;

	new_element->data = (void *)data;
	if(dlist_size(list) == 0){ //插入空链
		list->head = new_element;
		list->head->prev = NULL;
		list->head->next = NULL;
		list->tail = new_element;
	}else{
		new_element->next = element;
		new_element->prev = element->prev;

		if(element->prev == NULL) //如果插入头结点之前
			list->head = new_element;
		else
			element->prev->next = new_element;

		element->prev = new_element;
	}

	list->size++;
	return 0;
}

int dlist_remove(DList *list, DListElmt *element, void **data)
{
	//不支持删除空结点,不支持空链表
	if(element == NULL || dlist_size(list) == 0) { return -1; }

	*data = element->data;
	if(element == list->head){ //删除头结点
		list->head = element->next; //重置头结点

		if(list->head == NULL) //如果是单结点链表
			list->tail = NULL;
		else
			element->next->prev = NULL;
	}else{ //删除其他位置结点
		element->prev->next = element->next;

		if(element->next == NULL) //如果删除尾结点
			list->tail = element->prev; //重置尾结点
		else
			element->next->prev = element->prev;
	}

	free(element);
	list->size--;
	return 0;
}
