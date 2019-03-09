#include <stdlib.h>
#include <string.h>
#include <List/list.h>
#include <Set/set.h>

//初始化集合,set_init()函数比list_init()多了一个 match 参数
void set_init(Set *set, int (*match)(const void *key1, const void *key2),
              void (*destroy)(void *data))
{
  list_init(set, destroy);
  set->match = match;

  return;
}

//集合插入结点,该结点不能与集合中结点相同
int set_insert(Set *set, const void *data) 
{
  if(set_is_member(set, data))	{ return 1; }
  return list_ins_next(set, list_tail(set), data);
}

int set_remove(Set *set, void **data) 
{
  ListElmt	*member, *prev;
  //遍历整个集合,直到匹配到值为data的结点
  prev = NULL;
  for(member = list_head(set); member != NULL; member = list_next(member)) 
  {
    //break 提前跳出循环,便取得prev值,即member是data结点,prev是前一个元素
    if(set->match(*data, list_data(member))) break;
    prev = member;
  }

  if(member == NULL)				{ return -1; }
  return list_rem_next(set, prev, data);
}

int set_union(Set *setu, const Set *set1, const Set *set2) 
{
  ListElmt	*member;
  void        *data;

  //初始化setu集合
  set_init(setu, set1->match, NULL);

  //遍历set1集合,将所有元素插入setu集合中
  for(member = list_head(set1); member != NULL; member = list_next(member)) 
  {
    data = list_data(member);
    if(list_ins_next(setu, list_tail(setu), data) != 0){
      set_destroy(setu);
      return -1;
    }
  }

  //遍历set2集合
  for(member = list_head(set2); member != NULL; member = list_next(member)) 
  {
    if(set_is_member(set1, list_data(member))){ //如果set1集合包含了set2中元素
      continue;
    }else{ //将set2中非set1中元素插入到setu中
      data = list_data(member);
      if(list_ins_next(setu, list_tail(setu), data) != 0){
        set_destroy(setu);
        return -1;
      }
    }
  }

  return 0;
}

int set_intersection(Set *seti, const Set *set1, const Set *set2) 
{
  ListElmt	*member;
  void        *data;

  //初始化seti集合
  set_init(seti, set1->match, NULL);

  //遍历set1集合中所有元素
  for(member = list_head(set1); member != NULL; member = list_next(member)) 
  {
    //如果set2集合包含该元素,将该元素插入seti中
    if(set_is_member(set2, list_data(member))){
      data = list_data(member);
      if(list_ins_next(seti, list_tail(seti), data) != 0){
        set_destroy(seti);
        return -1;
      }
    }
  }

  return 0;
}

int set_difference(Set *setd, const Set *set1, const Set *set2) 
{
  ListElmt	*member;
  void        *data;

  //初始化setd集合
  set_init(setd, set1->match, NULL);

  //遍历set1集合中所有元素
  for(member = list_head(set1); member != NULL; member = list_next(member)) 
  {
    //如果set2集合不包含该元素,将该元素插入seti中
    if(!set_is_member(set2, list_data(member))){
      data = list_data(member);
      if(list_ins_next(setd, list_tail(setd), data) != 0){
        set_destroy(setd);
        return -1;
      }
    }
  }

  return 0;
}

//包含元素
int set_is_member(const Set *set, const void *data) 
{
  ListElmt	*member;

  //遍历集合进行匹配
  for(member = list_head(set); member != NULL; member = list_next(member)) 
  {
    if(set->match(data, list_data(member))) return 1;
  }

  return 0;
}

//判断 set1是否是set2的子集
int set_is_subset(const Set *set1, const Set *set2) {

  ListElmt	*member;

  if(set_size(set1) > set_size(set2)) return 0;

  //遍历set1,如果每个set1元素不是属于set2的则返回false
  for(member = list_head(set1); member != NULL; member = list_next(member)) 
  {
    if(!set_is_member(set2, list_data(member))) return 0;
  }

  return 1;
}

int set_is_equal(const Set *set1, const Set *set2) 
{
  if(set_size(set1) != set_size(set2)) return 0;
  return set_is_subset(set1, set2);
}
