#include <stdio.h>
#include <stdlib.h>
#include <List/dlist.h>

void print_list(const DList *list){

	DListElmt *element;
	int *data, i;

	fprintf(stdout, "List size is %d\n", dlist_size(list));
	
	i = 0;
	element = dlist_head(list);
	while(1){
		data = dlist_data(element);
		fprintf(stdout, "list[%03d]=%03d\n", i, *data);

		i++;
		if(dlist_is_tail(element))
			break;
		else
			element = dlist_next(element);
	}

	return ;
}

int main(int argc, char **argv){

	DList list;
	DListElmt *element;
	int *data, i;

	//初始化链表
	dlist_init(&list, free);

	for(i = 10; i > 0; i--){
		if((data = (int *)malloc(sizeof(int))) == NULL)
			return 1;

		*data = i;

		if(dlist_ins_prev(&list, dlist_head(&list), data) != 0)
			return 1;
	}

	print_list(&list);
	return 0;
}
