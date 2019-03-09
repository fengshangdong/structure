#include <stdlib.h>
#include <string.h>
#include <List/list.h>
#include <Htbl/chtbl.h>

//初始化链式哈希表,哈希表每个坑位表示一条链
int chtbl_init(CHTbl *htbl, int buckets, int (*h)(const void *key), int
    (*match)(const void *key1, const void *key2), void (*destroy)(void*data)) 
{
  int	i;
  //为哈希表分配buckets个结点大小的空间
  if ((htbl->table = (List *)malloc(buckets * sizeof(List))) == NULL)
    return -1;
  //初始化坑位数
  htbl->buckets = buckets;
  for (i = 0; i < htbl->buckets; i++)
  {
    //每个坑位分别初始化一个链表
    list_init(&htbl->table[i], destroy);
  }

  htbl->h = h;
  htbl->match = match;
  htbl->destroy = destroy;
  htbl->size = 0;
  return 0;
}

void chtbl_destroy(CHTbl *htbl) 
{
  int i;
  //遍历每个坑位,依次销毁每个坑位的链表
  for (i = 0; i < htbl->buckets; i++)
  {
    list_destroy(&htbl->table[i]);
  }

  //销毁哈希表中那条主链
  free(htbl->table);
  //哈希表重置为0
  memset(htbl, 0, sizeof(CHTbl));

  return;
}

int chtbl_insert(CHTbl *htbl, const void *data) 
{
  void	*temp;
  int		bucket, retval;

  //查找哈希表中是否已经存在相同元素
  temp = (void *)data;
  if(chtbl_lookup(htbl, &temp) == 0) { return 1; }

  //散列取余后获得坑位,然后将data结点插入该坑位的链表中
  bucket = htbl->h(data) % htbl->buckets;
  if((retval = list_ins_next(&htbl->table[bucket], NULL, data)) == 0)
    htbl->size++;

  return retval;
}

int chtbl_remove(CHTbl *htbl, void **data) {

  ListElmt	*element, *prev;
  int         bucket;
  //散列取余后获得坑位
  bucket = htbl->h(*data) % htbl->buckets;

  prev = NULL;
  //遍历该坑位处链表,匹配是否已存在data元素
  for(element = list_head(&htbl->table[bucket]); element != NULL; element =
      list_next(element))
  {
    if(htbl->match(*data, list_data(element))){ //如果data元素存在
      if(list_rem_next(&htbl->table[bucket], prev, data) == 0){
        htbl->size--;
        return 0;
      }else{
        return -1;
      }
    }
    //保存element前一个元素,可以在下次循环中用来删除data元素
    prev = element;
  }

  return -1;
}

int chtbl_lookup(const CHTbl *htbl, void **data) 
{
  ListElmt	*element;
  int         bucket;
  //获取data哈希后的坑位
  bucket = htbl->h(*data) % htbl->buckets;
  //遍历该坑位处链表,匹配是否已存在data元素
  for(element = list_head(&htbl->table[bucket]); element != NULL; 
      element = list_next(element)) 
  {
    if(htbl->match(*data, list_data(element)))
    {
      //赋值到data中
      *data = list_data(element);
      return 0;
    }
  }

  return -1;
}
