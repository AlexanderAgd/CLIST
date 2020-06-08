/******************************************/
/*                                        */
/*        Alexander Agdgomlishvili        */
/*                                        */
/*         cdevelopment@mail.com          */
/*                                        */
/******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "blist.h"

typedef struct BList_Node_
{
  struct BList_Node_ *prev;
  struct BList_Node_ *next;
  char *data;
} BList_Node_;

typedef struct
{
  int count;       /* Number of items in the list. */
  size_t block_size;  /* Each block size in quantity of items - not bytes! */
  size_t block_count; /* Number of blocks in the list */
  size_t item_size;   /* Size of each item in bytes. */
  BList_Node_ *first;
  BList_Node_ *last;
} BList_priv_;

void  BList_Add_(BList *l, void *o);
void  BList_Insert_(BList *l, void *o, int n);
void  BList_Replace_(BList *l, void *o, int n);
void  BList_Remove_(BList *l, int n);
void* BList_At_(BList *l, int n);
int   BList_FirstIndex_(BList *l, void *o);
int   BList_LastIndex_(BList *l, void *o);
int   BList_Count_(BList *l);
void  BList_Clear_(BList *l);
void  BList_Free_(BList *l);
void  BList_print_(BList *l, int n, const char *type);

BList *BList_Init(size_t objSize, size_t blockSize)
{
  BList *lst = malloc(sizeof(BList));
  BList_priv_ *p = malloc(sizeof(BList_priv_));
  if (!lst || !p)
  {
    fprintf(stderr, "BList: ERROR! Can not allocate BList!\n");
    return NULL;
  }
  p->count = 0;
  p->block_size = blockSize ? blockSize : 64;
  p->block_count = 0;
  p->item_size = objSize;
  p->first = NULL;
  p->last = NULL;
  lst->add = &BList_Add_;
  lst->insert = &BList_Insert_;
  lst->replace = &BList_Replace_;
  lst->remove = &BList_Remove_;
  lst->at = &BList_At_;
  lst->firstIndex = &BList_FirstIndex_;
  lst->lastIndex = &BList_LastIndex_;
  lst->count = &BList_Count_;
  lst->clear = &BList_Clear_;
  lst->free = &BList_Free_;
  lst->print = &BList_print_;
  lst->priv = p;
  return lst;
}

char BList_AllocNewBlock_(BList_priv_ *p)
{
  char *block = malloc(p->block_size * p->item_size);
  BList_Node_ *node = malloc(sizeof(BList_Node_));

  if (!node || !block)
  {
    fprintf(stderr, "BList: ERROR! Can not allocate memory block!\n");
    assert(node != NULL && block != NULL);
    return 0;
  }

  if (p->count)
  {
    p->last->next = node;
    node->prev = p->last;
    node->data = block;
    p->last = node;
    node->next = NULL;
  }  
  else
  {
    node->prev = NULL;
    node->next = NULL;
    node->data = block;
    p->first = node;
    p->last = node;
  }

  p->block_count++;
  return 1;        
}

void BList_Add_(BList *l, void *o)
{
  BList_priv_ *p = (BList_priv_*) l->priv;      
  size_t pos_inblk = p->count % p->block_size;
       
  if (pos_inblk == 0)
    BList_AllocNewBlock_(p);
     
  char *data = p->last->data + pos_inblk * p->item_size;
  memcpy(data, o, p->item_size);
  p->count++;
}

void  BList_Insert_(BList *l, void *o, int n)
{
  BList_priv_ *p = (BList_priv_*) l->priv;
  if (n > p->count || n < 0)
  {
    fprintf(stderr, "BList: ERROR! Insert position outside range - %d; n - %d.\n", 
                        p->count, n);
    assert(n >= 0 && n <= p->count);
    return;
  }

  int newblk = p->count % p->block_size;
  if (newblk == 0)
    BList_AllocNewBlock_(p);

  BList_Node_ *node = p->last;
  size_t count = p->count;
  size_t pos_inblk = n % p->block_size;
  size_t start = (p->block_count - 1) * p->block_size;
  size_t end = newblk ? count : start;
  int length = end - start;
  char *data = NULL;
  char stop = 0;

  size_t i = 0;
  for (; i < p->block_count; i++)
  {
    data = node->data;
    if ((size_t) n >= start)
    {
      length = (pos_inblk != p->block_size - 1) ? length - pos_inblk : 0;
      if (length > 0)
        data = node->data + pos_inblk * p->item_size;
      stop = 1;
    }

    if (length > 0)
      memmove(data + p->item_size, data, length * p->item_size);
    if (stop || node->prev == NULL) break;

    data = node->prev->data + (p->block_size - 1) * p->item_size;
    memcpy(node->data, data, p->item_size);

    node=node->prev;
    start -= p->block_size;
    length = p->block_size - 1;
  }

  data = node->data + pos_inblk * p->item_size;
  memcpy(data, o, p->item_size);
  p->count++;
}

void BList_Replace_(BList *l, void *o, int n)
{
  BList_priv_ *p = (BList_priv_*) l->priv;
  if (n >= p->count || n < 0)
  {
    fprintf(stderr, "BList: ERROR! Replace position outside range - %d; n - %d.\n", 
                          p->count, n);
    assert(n >= 0 && n < p->count);
    return;
  }

  size_t block_N = n / p->block_size;
  BList_Node_ *cur = p->first;
  size_t i = 0; 
  for (; i < block_N; i++)
    cur = cur->next;

  n = n - block_N * p->block_size; /* Position in block */
  char *data = cur->data + n * p->item_size;
  memcpy(data, o, p->item_size);
}

void BList_Remove_(BList *l, int n)
{
  BList_priv_ *p = (BList_priv_*) l->priv;
  if (n >= p->count || n < 0)
  {
    fprintf(stderr, "BList: ERROR! Remove position outside range - %d; n - %d.\n", 
                          p->count, n);
    assert(n >= 0 && n < p->count);
    return;
  }

  size_t block_N = n / p->block_size;
  BList_Node_ *node = p->first;
  size_t i = 0; 
  for (; i < block_N; i++)
    node = node->next;

  size_t start = n;
  size_t count = p->count;
  size_t end = (node->next != NULL) ? 
    (block_N + 1) * p->block_size - 1: count - 1;
  int length = end - start;
  char *data = node->data + (n % p->block_size) * p->item_size;

  for (i = 0; i < p->block_count - block_N; i++)
  {
    if (length > 0)
      memmove(data, data + p->item_size, length * p->item_size);
    if (node->next == NULL) break;

    data = node->data + (p->block_size - 1) * p->item_size;
    memcpy(data, node->next->data, p->item_size);

    start = (block_N + i + 1) * p->block_size;
    node=node->next;
    if (start >= (size_t) p->count - 1) break;

    length = (node->next != NULL) ? p->block_size - 1 : (p->count - 1) - start;
    data = node->data;
  }

  p->count--;
  if (p->count % p->block_size == 0)
  {  
    if (node->prev != NULL)
    {  
      node->prev->next = NULL;
      p->last = node->prev;
    }
    else
    {
      p->first = NULL;
      p->last = NULL;
    }  
    free(node->data);
    free(node);
    p->block_count--;
  }
}

void *BList_At_(BList *l, int n)
{
  BList_priv_ *p = (BList_priv_*) l->priv;
  if (n >= p->count || n < 0)
  {
    fprintf(stderr, "BList: ERROR! Replace position outside range - %d; n - %d.\n", 
                          p->count, n);
    assert(n >= 0 && n < p->count);
    return NULL;
  }
      
  size_t block_N = n / p->block_size;
  BList_Node_ *cur = p->first;
  size_t i = 0; 
  for (; i < block_N; i++)
    cur = cur->next;

  n = n % p->block_size; /* Position in block */
  char *data = cur->data + n * p->item_size;
  return data;
}

int BList_FirstIndex_(BList *l, void *o)
{
  BList_priv_ *p = (BList_priv_*) l->priv;
  size_t step = p->item_size;
  size_t i = 0;
  size_t end;
  int index = 0;
  BList_Node_ *node = p->first;
  char *data = NULL;
  while (node != NULL)
  {  
    data = node->data;
    end = (node->next != NULL) ? p->block_size - 1 : (p->count - 1) % p->block_size;
    for (i = 0; i <= end; i++, index++)
    {          
      if (strncmp(data + i * step, o, step) == 0)
        return index;
    }
    node = node->next;
  }
  return -1;
}

int BList_LastIndex_(BList *l, void *o)
{
  BList_priv_ *p = (BList_priv_*) l->priv;
  size_t step = p->item_size;
  int i = (p->count - 1) % p->block_size;
  int index = p->count - 1;
  BList_Node_ *node = p->last;
  char *data = NULL;
  while (node != NULL)
  {  
    data = node->data;
    for (; i >=0 ; i--, index--)
    {          
      if (strncmp(data + i * step, o, step) == 0)
        return index;
    }
    i = p->block_size - 1;
    node = node->prev;
  }
  return -1;    
}
  
int BList_Count_(BList *l)
{
  BList_priv_ *p = (BList_priv_*) l->priv;
  return p->count;
}

void BList_Clear_(BList *l)
{
  BList_priv_ *p = (BList_priv_*) l->priv;
  BList_Node_ *node = p->last;
  if (node == NULL)
    return;
  BList_Node_ *tmp;
  while (node->prev != NULL)
  { 
    tmp = node->prev;  
    free(node->data);       
    free(node);
    node = tmp;
  }
  free(node->data);
  free(node);  
  p->count = 0;
  p->item_size = 0;
  p->block_count = 0;
  p->first = NULL;
  p->last = NULL;
}

void  BList_Free_(BList *l)
{
  BList_Clear_(l);
  BList_priv_ *p = (BList_priv_*) l->priv;
  free(p);
  free(l);
  l = NULL;
}

void BList_print_(BList *l, int n, const char *type)
{
  BList_priv_ *p = (BList_priv_*) l->priv;
  printf("\nBList:   count = %zu   block_count = %zu   block_size = %zu   item_size = %zu   "
       "first = %p    last = %p \n", p->count, p->block_count, p->block_size,
       p->item_size, p->first, p->last);

  if (n > 0)
  {    
    n = (n > p->count) ? p->count : n;
    n--;
    int index = 0;
    int i = 0;
    char *data = NULL;
    BList_Node_ *node = p->first;
    while (node != NULL)
    {
      int block_end = (node->next != NULL) ? p->block_size - 1 : n % p->block_size;
      for (i = 0; i <= block_end; i++, index++)
      {       
        data = node->data + i * p->item_size;
        if (type == NULL) printf("%p  ", data);  /* Print out pointers */
        else if (strcmp(type, "char") == 0) printf("%c ", *(char*) data);
        else if (strcmp(type, "short") == 0) printf("%hi  ", *(short*) data);
        else if (strcmp(type, "int") == 0) printf("%i  ", *(int*) data);
        else if (strcmp(type, "long") == 0) printf("%li  ", *(long*) data);
        else if (strcmp(type, "uintptr_t") == 0) printf("%zx  ", (uintptr_t*) data);
        else if (strcmp(type, "size_t") == 0) printf("%zu  ", *(size_t*) data);
        else if (strcmp(type, "double") == 0) printf("%f  ", *(double*) data);
        else if (strcmp(type, "string") == 0) printf("%s\n", data);
        else { printf("Unknown type."); return; }
      }
      if (index == n) 
        return;
      node = node->next;
    }
    printf("\n\n");
  }
}
