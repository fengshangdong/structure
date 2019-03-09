#include <stdlib.h>
#include <stdio.h>
#include <List/clist.h>

static void print_list(const CList *list){
  CListElmt *element;
  int *data, size, i;

  fprintf(stdout, "List size is %d (circling twice)\n", clist_size(list));

  size = clist_size(list);
  element = clist_head(list);

  i = 0;
  while(i < size * 2){
    data = clist_data(element);
    fprintf(stdout, "list[%03d]=%03d\n", (i % size), *data);
    element = clist_next(element);
    i++;
  }

  return ;
}

int main(){
  CList list;
  CListElmt *element;
  int *data, i;

  clist_init(&list, free);
  element = clist_head(&list);
  for(i = 0; i < 10; i++){
    if((data = (int *)malloc(sizeof(int))) == NULL)
      return 1;

    *data = i + 1;

    if(clist_ins_next(&list, element, data) != 0)
      return 1;

    if(element == NULL)
      element = clist_next(clist_head(&list));
    else
      element = clist_next(element);
  }

  print_list(&list);
}
