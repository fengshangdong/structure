#include <stdlib.h>
#include <string.h>
#include <Htbl/ohtbl.h>

//为空出的元素预留一个位置
static char		vacated;

int ohtbl_init(OHTbl *htbl, int positions, int (*h1)(const void *key), int
		(*h2)(const void *key), int (*match)(const void *key1, const void *key2),
		void (*destroy)(void *data)) 
{
	int i;
	//为哈希表分配空间
	if ((htbl->table = (void **)malloc(positions * sizeof(void *))) == NULL)
		return -1;

	//初始化哈希表的每个元素
	htbl->positions = positions;
	for (i = 0; i < htbl->positions; i++)
		htbl->table[i] = NULL;

	//赋值预留位置
	htbl->vacated = &vacated;

	htbl->h1 = h1;
	htbl->h2 = h2;
	htbl->match = match;
	htbl->destroy = destroy;
	htbl->size = 0;
	return 0;
}

//销毁哈希表
void ohtbl_destroy(OHTbl *htbl) 
{
	int i;

	if(htbl->destroy != NULL)
	{
		//一系列的探查遍历,如果不是预留位置,则销毁该元素
		for(i = 0; i < htbl->positions; i++)
		{
			if(htbl->table[i] != NULL && htbl->table[i] != htbl->vacated)
				htbl->destroy(htbl->table[i]);
		}
	}

	//释放哈希表本身的内存
	free(htbl->table);
	//重置为0
	memset(htbl, 0, sizeof(OHTbl));
	return;
}

int ohtbl_insert(OHTbl *htbl, const void *data)
{
	void	*temp;
	int     position, i;

	//不超出表范围
	if(htbl->size == htbl->positions)
		return -1;

	//查找是否已存在相同的元素,如果有的话直接返回
	temp = (void *)data;
	if(ohtbl_lookup(htbl, &temp) == 0) { return 1; }

	//一系列的探查遍历,遍历positions个位置
	for(i = 0; i < htbl->positions; i++)
	{
		//双散列法,data取得key值
		position = (htbl->h1(data) + (i * htbl->h2(data))) % htbl->positions;
		//如果该key值是空槽位或者是预留位置,将指针指向待插入数据
		if(htbl->table[position] == NULL || htbl->table[position] == htbl->vacated)
		{
			//指向插入数据
			htbl->table[position] = (void *)data;
			htbl->size++;
			return 0;
		}
	}

	//返回插入失败
	return -1;
}

int ohtbl_remove(OHTbl *htbl, void **data)
{
	int position, i;
	//一系列的探查遍历
	for(i = 0; i < htbl->positions; i++)
	{
		//双散列法,data取得key值
		position = (htbl->h1(*data) + (i * htbl->h2(*data))) % htbl->positions;
		if(htbl->table[position] == NULL){ //未找到数据
			return -1;
		}else if(htbl->table[position] == htbl->vacated){ //超出预留空位
			continue;
		}else if(htbl->match(htbl->table[position], *data)){ //找到匹配值
			//返回哈希表元素值
			*data = htbl->table[position];
			//删除后预留的空位置
			htbl->table[position] = htbl->vacated;
			htbl->size--;
			return 0;
		}
	}

	//如果未找到元素
	return -1;
}

int ohtbl_lookup(const OHTbl *htbl, void **data)
{
	int position, i;
	//一系列的探查遍历
	for(i = 0; i < htbl->positions; i++){
		//双散列取得key值
		position = (htbl->h1(*data) + (i * htbl->h2(*data))) % htbl->positions;
		if(htbl->table[position] == NULL){
			return -1;
		}else if(htbl->match(htbl->table[position], *data)){
			//返回data值
			*data = htbl->table[position];
			return 0;
		}
	}

	//未找到
	return -1;
}
