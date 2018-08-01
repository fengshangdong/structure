#include <stdlib.h>
#include <string.h>
#include <List/clist.h>

//初始化链表
void clist_init(CList *list, void (*destroy)(void *data))
{
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;

	return;
}

//销毁链表
void clist_destroy(CList *list)
{
	void *data;
	while(clist_size(list) > 0){ //链表不为空时,依次删除头元素
		if(clist_rem_next(list, list->head, (void **)&data) == 0 && list->destroy != NULL){
			list->destroy(data); //释放链表元素malloc分配的空间
		}
	}

	//链表置空 (结点数size = 0 & 头结点head = NULL)
	memset(list, 0, sizeof(CList));
	return;
}

int clist_ins_next(CList *list, CListElmt *element, const void *data)
{
	//分配元素空间
	CListElmt *new_element;
	if((new_element = (CListElmt *)malloc(sizeof(CListElmt))) == NULL)
		return -1;

	//元素插入链表中
	new_element->data = (void *)data;
	if(clist_size(list) == 0){ //链表为空,插入结点做头结点
		new_element->next = new_element;
		list->head = new_element;
	}else{ //链表不为空,直接插入
		new_element->next = element->next;
		element->next = new_element;
	}

	list->size++;
	return 0;
}

int clist_rem_next(CList *list, CListElmt *element, void **data)
{
	//链表不为空
	CListElmt *old_element;
	if(clist_size(list) == 0)
		return -1;

	//取得被删除结点元素值
	*data = element->next->data;

	//删除结点
	if(element->next == element){ //链表只有一个结点
		old_element = element->next;
		list->head = NULL;
	}else{ //链表有一个以上结点
		old_element = element->next;
		element->next = element->next->next;
		//被删除结点是头结点
		if(old_element == clist_head(list)) { list->head = old_element->next; }
	}

	free(old_element);
	list->size--;
	return 0;
}
