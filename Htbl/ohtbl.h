#ifndef OHTBL_H
#define OHTBL_H

#include <stdlib.h>

//定义一个开地址哈希表,不需要用到链表
//@positions	哈希表的空间大小
//@vacated		空出元素的预留位置
//@size			哈希表已使用空间大小
typedef struct OHTbl_{
  int                positions;
  void               *vacated;

  int                (*h1)(const void *key);
  int                (*h2)(const void *key);
  int                (*match)(const void *key1, const void *key2);
  void               (*destroy)(void *data);

  int                size;
  void               **table;
}OHTbl;

int ohtbl_init(OHTbl *htbl, int positions, int (*h1)(const void *key),
               int (*h2)(const void *key), int (*match)(const void *key1, const void *key2),
               void (*destroy)(void *data));

void ohtbl_destroy(OHTbl *htbl);

int ohtbl_insert(OHTbl *htbl, const void *data);

int ohtbl_remove(OHTbl *htbl, void **data);

int ohtbl_lookup(const OHTbl *htbl, void **data);

#define ohtbl_size(htbl) ((htbl)->size)

#endif
