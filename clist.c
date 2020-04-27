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

void CList_exec(CList *list, void *obj, int *num, enum CListMode mode)
{
  if (num != NULL && *num < 0)
  {
    fprintf(stderr, "CList: ERROR! num = %i value canot be less than 0.\n", *num);
    assert(*num >= 0);
    return;
  }  

  switch (mode)
  {
    case CList_Init:
    {
      if (sizeof(void*) != 8 && sizeof(void*) != 4)
        fprintf(stderr, "CList: WARNING! Unknown device, not 32 or 64 bit system!");

      if (list->item_size > 16)
        fprintf(stderr, "CList: WARNING! Object size more than 16. Try use pointers.");

      size_t size = *num;
      if (size <= 0 || num == NULL)
        size = 128;

      list->items = malloc(list->item_size * size);
      list->alloc_size = size;
      list->count = 0;
      break;
    }

    case CList_ReAlloc:
    {
      size_t size = 0;
      if (num != NULL) size = *num;

      if (size == 0)
      {
        size_t length = list->alloc_size;

        if (length < 32) size = 128;
        else if (length < 129) size = 512;
        else if (length < 513) size = 2048;
        else if (length < 2049) size = 8192;
        else if (length < 8193) size = 16384;
        else if (length < 16385) size = 32768;
        else if (length < 32769) size = 65536;
        else if (length < 65537) size = 131072;
        else if (length < 131073) size = 262144;
        else if (length < 262145) size = 524288;
        else if (length < 524289) size = 1048576;
        else
        {
          fprintf(stderr, "CList: You need a lot of memory!\nEdit this code yourself ...\n");
          assert(size <= 1048576);
          break;
        }
      }

      list->items = realloc(list->items, list->item_size * size);

      if (list->items == NULL)
      {
        fprintf(stderr, "CList: ERROR! can not reallocate memory!\n");
        assert(list->items != NULL);
      }

      #ifdef DEBUG
      fprintf(stdout, "CList: Reallocated %d bytes or %d items.\n", 
               list->item_size * size, size);
      #endif

      list->alloc_size = size;
      break;
    }

    case CList_Add:
    {
      if (list->count == list->alloc_size)
        CList_exec(list, NULL, NULL, CList_ReAlloc);
    
      char *data = (char*) list->items;
      //char *item = (char*) obj;
      data = data + list->count * list->item_size;
      memcpy(data, obj, list->item_size);
      list->count++;

      break;
    }

    case CList_Insert:
    {
      size_t pos = *num;
      if (pos > list->count)
      {
        fprintf(stderr, "CList: ERROR! Position outside range - %d; pos - %d.\n", 
                          list->count - 1, pos);
        assert(pos <= list->count);
        break;
      }

      if (list->count == list->alloc_size)
        CList_exec(list, NULL, NULL, CList_ReAlloc);

      size_t step = list->item_size;
      size_t i = list->count * step;
      char *data = (char*) list->items;
      //char *item = (char*) obj;

      for (; i > pos * step; i -= step)
        memcpy(data + i, data + i - step, step);
        
      memcpy(data + i, obj, step);
      list->count++;
      break;
    }

    case CList_Get:
    {
      size_t pos = *num;
      char *data = (char*) list->items;
      //char *item = (char*) obj;
      data = data + pos * list->item_size;
      memcpy(obj, data, list->item_size);
      break;
    }

    case CList_FirstIndex:
    {
      size_t pos = *num;
      char *data = (char*) list->items;
      //char *item = (char*) obj;
      size_t step = list->item_size;
      size_t i = 0;
      *num = -1;
      for (; i < list->count * step; i += step)
      {
        if (strncmp(data + i, obj, step) == 0)
        {
          *num = i / step;
          break;
        }
      }

      break;
    }

    case CList_LastIndex:
    {
      size_t pos = *num;
      char *data = (char*) list->items;
      //char *item = (char*) obj;
      size_t step = list->item_size;
      long int i = list->count * step - step;
      *num = -1;
      for (; i >= 0 ; i -= step)
      {
        if (strncmp(data + i, obj, step) == 0)
        {
          *num = i / step;
          break;
        }
      }
        
      break;
    }
  
    case CList_Remove:
    {
      size_t pos = *num;
	
      if (num == NULL)
      {
        fprintf(stderr, "CList: ERROR! Index \'num\' is NULL.\n");
        assert(num != NULL);
        break;
      }
      else if (pos >= list->count)
      {
        fprintf(stderr, "CList: ERROR! Outside range - %d; pos - %d.\n", list->count - 1, pos);
        assert(pos < list->count);
        break;
      }

      size_t step = list->item_size;
      size_t i = pos * step;
      char *data = (char*) list->items;

      for (; i < list->count * step; i += step)
       memcpy(data + i, data + i + step, step);       

      list->count--;
      break;
    }

    case CList_Clear:
    {
      free(list->items);
      list->items = NULL;
      list->alloc_size = 0;
      list->count = 0;
      break;
    }

    default:
    {
      fprintf(stderr, "CList: WARNING! Wrong enum \'CListMode\' value used.\n");
      break;
    }
  }
}
