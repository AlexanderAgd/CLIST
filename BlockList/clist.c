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
#include "clist.h"

/* 
#define DEBUG
*/

typedef struct Node
{
  struct Node *prev;
  struct Node *next;
  char *data;
} Node;

typedef struct BlockList
{
  size_t count;       /* Number of items in the list. */
  size_t block_size;  /* Each block size in quantity of items - not bytes! */
  size_t block_count; /* Number of blocks in the list */
  size_t item_size;   /* Size of each item in bytes. */
  Node *first;
  Node *last;
} BlockList;

char allocNewBlock(BlockList *l)
{
  char *block = malloc(l->block_size * l->item_size);
  Node *node = malloc(sizeof(Node));

  if (!node || !block)
  {
    fprintf(stderr, "CList: ERROR! Can not allocate memory block!\n");
    assert(node != NULL && block != NULL);
    return 0;
  }

  if (l->count)
  {
    l->last->next = node;
    node->prev = l->last;
    node->data = block;
    l->last = node;
    node->next = NULL;
  }  
  else
  {
    node->prev = NULL;
    node->next = NULL;
    node->data = block;
    l->first = node;
    l->last = node;
  }

  l->block_count++;
  return 1;        
}

void *CList_exec(CList list, void *obj, int *num, enum CListMode mode)
{
  if (num != NULL && *num < 0)
  {
    fprintf(stderr, "CList: ERROR! num = %i value canot be less than 0.\n", *num);
    assert(*num >= 0);
    return NULL;
  }

  switch (mode)
  {
    case CList_Init:
    {
      if (num == NULL || *num == 0)
      {
        fprintf(stderr, "CList: ERROR! Item size \'num\' is not set.\n");
        assert(num != NULL && *num != 0);
        break;
      }

      BlockList *lst = malloc(sizeof(BlockList));
      if (!lst)
      {
        fprintf(stderr, "CList: ERROR! Can not allocate CList!\n");
        assert(lst != NULL);
        return NULL;
      }
      lst->count = 0;
      lst->block_size = 64;
      lst->block_count = 0;
      lst->item_size = *num;
      lst->first = NULL;
      lst->last = NULL;
      return lst;
    }

    case CList_Add:
    {
      BlockList *lst = (BlockList*) list;      
      size_t pos_inblk = lst->count % lst->block_size;
       
      if (pos_inblk == 0)
        allocNewBlock(lst);
     
      char *data = lst->last->data + pos_inblk * lst->item_size;
      memcpy(data, obj, lst->item_size);
      //printf("pos=%lu    %c    data= %p    it_size= %i \n", pos,  *(char*)obj, data, lst->item_size);
      lst->count++;
      break;
    }

    case CList_Get:
    {
      if (num == NULL)
      {
        fprintf(stderr, "CList: ERROR! Index \'num\' is NULL.\n");
        assert(num != NULL);
        break;
      }

      size_t pos = *num;
      BlockList *lst = (BlockList*) list;
      if (pos >= lst->count)
      {
        fprintf(stderr, "CList: ERROR! Get position outside range - %d; pos - %d.\n", 
                          lst->count, pos);
        assert(pos < lst->count);
        break;
      }
      
      size_t block_N = pos / lst->block_size;
      Node *cur = lst->first;
      size_t i = 0; 
      for (; i < block_N; i++)
        cur = cur->next;

      pos = pos - block_N * lst->block_size; /* Position in block */
      char *data = cur->data + pos * lst->item_size;
      //printf("\nblock_N= %i   pos= %i   ", block_N, pos);
      return data;
    }

    case CList_Count:
    {
      if (num == NULL)
      {
        fprintf(stderr, "CList: ERROR! Index \'num\' is NULL.\n");
        assert(num != NULL);
        break;
      }

      BlockList *lst = (BlockList*) list;
      *num = lst->count;
      break;
    }

    case CList_SetBlockSize:
    {
      if (num == NULL || *num == 0)
      {
        fprintf(stderr, "CList: ERROR! Block size \'num\' is not set.\n");
        assert(num != NULL && *num != 0);
        break;
      }
      BlockList *lst = (BlockList*) list;
      if (lst->count)
      {
        fprintf(stderr, "CList: ERROR! List contains %i items.\n", lst->count);
        assert(lst->count == 0);
        break;
      }  
      lst->block_size = *num;
      break;
    }

    case CList_BlockSize:
    {
      if (num == NULL)
      {
        fprintf(stderr, "CList: ERROR! Index \'num\' is NULL.\n");
        assert(num != NULL);
        break;
      }

      BlockList *lst = (BlockList*) list;
      *num = lst->block_size;
      break;   
    }

    case CList_Insert:
    {
      if (num == NULL)
      {
        fprintf(stderr, "CList: ERROR! Index \'num\' is NULL.\n");
        assert(num != NULL);
        break;
      }

      BlockList *lst = (BlockList*) list;
      size_t pos = *num;
      //printf("_  pos= %i   count=%i    \n", pos, lst->count);
      if (pos > lst->count)
      {
        fprintf(stderr, "CList: ERROR! Insert position outside range - %d; pos - %d.\n", 
                          lst->count, pos);
        assert(pos <= lst->count);
        break;
      }

      int newblk = lst->count % lst->block_size;
      if (newblk == 0)
        allocNewBlock(lst);

      Node *node = lst->last;
      size_t pos_inblk = pos % lst->block_size;
      size_t start = (lst->block_count - 1) * lst->block_size;
      size_t end = newblk ? lst->count : start;
      int length = end - start;
      char *data = NULL;
      char stop = 0;
      //printf("A  start= %i   end=%i   pos=%i \n", start, end, pos);
      size_t i = 0;
      for (; i < lst->block_count; i++)
      {
        data = node->data;
        if (pos >= start)
        {
          length = (pos_inblk != lst->block_size - 1) ? length - pos_inblk : 0;
          if (length > 0)
            data = node->data + pos_inblk * lst->item_size;
          stop = 1;
        }

        if (length > 0)
          memmove(data + lst->item_size, data, length * lst->item_size);
        if (stop || node->prev == NULL) break;

        data = node->prev->data + (lst->block_size - 1) * lst->item_size;
        memcpy(node->data, data, lst->item_size);

        node=node->prev;
        start -= lst->block_size;
        length = lst->block_size - 1;
      }

      data = node->data + pos_inblk * lst->item_size;
      memcpy(data, obj, lst->item_size);
      lst->count++;
      break;
    }

    case CList_Replace:
    {
      if (num == NULL)
      {
        fprintf(stderr, "CList: ERROR! Index \'num\' is NULL.\n");
        assert(num != NULL);
        break;
      }

      BlockList *lst = (BlockList*) list;
      size_t pos = *num;
      if (pos >= lst->count)
      {
        fprintf(stderr, "CList: ERROR! Replace position outside range - %d; pos - %d.\n", 
                          lst->count, pos);
        assert(pos < lst->count);
        break;
      }

      size_t block_N = pos / lst->block_size;
      Node *cur = lst->first;
      size_t i = 0; 
      for (; i < block_N; i++)
        cur = cur->next;

      pos = pos - block_N * lst->block_size; /* Position in block */
      char *data = cur->data + pos * lst->item_size;
      memcpy(data, obj, lst->item_size);    
      break;
    }

    case CList_FirstIndex:
    {
      if (num == NULL)
      {
        fprintf(stderr, "CList: ERROR! Index \'num\' is NULL.\n");
        assert(num != NULL);
        break;
      }

      BlockList *lst = (BlockList*) list;
      size_t step = lst->item_size;
      size_t i = 0;
      size_t end;
      size_t index = 0;
      *num = -1;
      Node *node = lst->first;
      char *data = NULL;
      while (node != NULL)
      {  
        data = node->data;
        end = (node->next != NULL) ? lst->block_size : lst->count % lst->block_size;
        for (; i < end; i++, index++)
        {          
          if (strncmp(data + i * step, obj, step) == 0)
          {
            *num = index;
            return NULL;
          }
        }
        i = 0;
        node = node->next;
      }
      break;
    }

    case CList_LastIndex:
    {
      if (num == NULL)
      {
        fprintf(stderr, "CList: ERROR! Index \'num\' is NULL.\n");
        assert(num != NULL);
        break;
      }

      BlockList *lst = (BlockList*) list;
      size_t step = lst->item_size;
      int i = (lst->count - 1) % lst->block_size;
      size_t index = lst->count - 1;
      *num = -1;
      Node *node = lst->last;
      char *data = NULL;
      while (node != NULL)
      {  
        data = node->data;
        for (; i >=0 ; i--, index--)
        {          
          if (strncmp(data + i * step, obj, step) == 0)
          {
            *num = index;
            return NULL;
          }
        }
        i = lst->block_size - 1;
        node = node->prev;
      }
      break;
    }
  
    case CList_Remove:
    {
      if (num == NULL)
      {
        fprintf(stderr, "CList: ERROR! Index \'num\' is NULL.\n");
        assert(num != NULL);
        break;
      }

      BlockList *lst = (BlockList*) list;
      size_t pos = *num;
      if (pos >= lst->count)
      {
        fprintf(stderr, "CList: ERROR! Remove position outside range - %d; pos - %d.\n",
                          lst->count - 1, pos);
        assert(pos < lst->count);
        break;
      }

      size_t block_N = pos / lst->block_size;
      Node *node = lst->first;
      size_t i = 0; 
      for (; i < block_N; i++)
        node = node->next;

      size_t start = pos;
      size_t end = (node->next != NULL) ? 
        (block_N + 1) * lst->block_size - 1: lst->count - 1;
      int length = end - start;
      char *data = node->data + (pos % lst->block_size) * lst->item_size;

      for (i = 0; i < lst->block_count - block_N; i++)
      {
        if (length > 0)
          memmove(data, data + lst->item_size, length * lst->item_size);
        if (node->next == NULL) break;

        data = node->data + (lst->block_size - 1) * lst->item_size;
        memcpy(data, node->next->data, lst->item_size);

        start = (block_N + i + 1) * lst->block_size;
        node=node->next;
        if (start >= lst->count - 1)
          break;

        length = (node->next != NULL) ? lst->block_size - 1 : (lst->count - 1) - start;
        data = node->data;
      }

      lst->count--;
      if (lst->count % lst->block_size == 0)
      {  
        if (node->prev != NULL)
        {  
          node->prev->next = NULL;
          lst->last = node->prev;
        }
        else
        {
          lst->first = NULL;
          lst->last = NULL;
        }  
        free(node->data);
        free(node);
        lst->block_count--;
      }
      break;
    }

    case CList_Clear:
    {
      BlockList *lst = (BlockList*) list;
      Node *node = lst->last;
      if (node == NULL)
        break;
      Node *tmp;
      while (node->prev != NULL)
      { 
        tmp = node->prev;  
        free(node->data);       
        free(node);
        node = tmp;
      }
      free(node->data);
      free(node);  
      lst->count = 0;
      lst->item_size = 0;
      lst->block_count = 0;
      lst->first = NULL;
      lst->last = NULL;
      break;
    }

    default:
    {
      fprintf(stderr, "CList: ERROR! Wrong enum \'CListMode\' value used.\n");
      assert(mode >= CList_Init && mode <= CList_Clear);
      break;
    }
  }

  return NULL;
}

void CList_print(CList list)
{
  BlockList *lst = (BlockList*) list;
  printf("CListPrint:   count = %li   block_count = %li   block_size = %li   item_size = %li   "
       "first = %p    last = %p \n", lst->count, lst->block_count, lst->block_size,
       lst->item_size, lst->first, lst->last);
}
