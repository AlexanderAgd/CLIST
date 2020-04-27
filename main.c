/******************************************/
/*                                        */
/*        Alexander Agdgomlishvili        */
/*                                        */
/*         cdevelopment@mail.com          */
/*                                        */
/******************************************/

#include <stdio.h>
#include <stdlib.h>
#include "clist.h"

enum DataType
{                    /* bytes below true at least for x64 bit system */
  type_char,         /* 1 byte */
  type_short,        /* 2 byte */
  type_int,          /* 4 byte */
  type_void,         /* 8 byte */
  type_long_double   /* 16 byte */
};

void CListInfo(CList *list, enum DataType byte);

int main(int argc, char **argv)
{
  printf("\n\n");

  void *obj = NULL;
  int i = 0;
  int n = 0;
  CList list = { .count = 0,
                 .alloc_size = 0,
                 .item_size = sizeof(char), /* 1 byte */
                 .items = NULL
               };

  CList_exec(&list, NULL, NULL, CList_Init); /* 128 byte allocated for char array */

  CListInfo(&list, type_char);
  
  for (i = 33; i < 118; i++)
  { 
    obj = &i;  
    CList_exec(&list, obj, NULL, CList_Add); /* Adding obj items to the end of array */
  }

  CListInfo(&list, type_char);

  n = 32;
  char ch = 'A';
  CList_exec(&list, obj, &n, CList_Get); /* Get 32nd item of array */
  ch = *((char*) obj);
  printf("Position %i contains symbol \'%c\'\n", n, ch);

  ch = '!';
  obj = &ch;
  CList_exec(&list, obj, &n, CList_FirstIndex);  /* Find first index of '!' char */
  printf("Index of \'%c\' is %i\n", ch, n);

  ch = '+';
  CList_exec(&list, obj, &n, CList_FirstIndex);
  printf("Index of \'%c\' is %i\n\n", ch, n);

  n = 0;
  for (i = 15; i > 0; i--)
    CList_exec(&list, obj, &n, CList_Remove); /* Remove item at index 0 from array 15 times */

  CListInfo(&list, type_char);

  CList_exec(&list, NULL, NULL, CList_Clear); /* Clear struct list and free its array "items" */
                                              /* Only item_size is unchanged. */

  /*******************************************************/

  list.item_size = sizeof(short); /* 2 byte */
  
  short sh = 1001;
  for (i = 0; i < 24; i++, sh += 1000)
  {
    obj = &sh;
    CList_exec(&list, obj, NULL, CList_Add); /* No CList_Init used  after clear, automated allocation */ 
  }

  CListInfo(&list, type_short);

  sh = 5001;
  n = 20;
  CList_exec(&list, obj, &n, CList_Insert);   /* Insert value of 'sh' to position 20 */
  n = 25;
  CList_exec(&list, obj, &n, CList_Insert);   /* Insert value of 'sh' to position 25 */
  
  CListInfo(&list, type_short);

  n = 0;
  CList_exec(&list, obj, &n, CList_LastIndex); /* Find last index of '5001' short */
  printf("Last index of \'%i\' is %i\n", sh, n);

  
  /*******************************************************/
  CList *lst = malloc(sizeof(CList));
  lst->item_size = 2;
  int size = 3;

  //fprintf(stdout, "Item_size %i \nK =  %i ...\n", lst->item_size, k);
  //*(lst) = { 0, 11, 8, ptr };

  CList_exec(lst, NULL, &size, CList_Init);

  short block = 2;
  short *ptr = &block;
  int p = 1;
  int *pos = &p;

  short dig = 21;
  CList_exec(lst, &dig, NULL, CList_Add);
  dig = 28;
  CList_exec(lst, &dig, NULL, CList_Add);
  dig = 23;
  CList_exec(lst, &dig, pos, CList_Insert);
  p = 2;
  dig = 11;
  CList_exec(lst, &dig, pos, CList_Insert);

  return 0;
}

void CListInfo(CList *list, enum DataType byte)
{
  printf("CList:   count = %lu   alloc_size = %lu   item_size = %lu    -----------------------\n", 
    list->count, list->alloc_size, list->item_size);

  size_t i = 0;
  switch(byte)
  {
    case type_char:
    {
      char *data = (char*) list->items;
      for (; i < list->count; i++)
        printf("%c ", data[i]);
      break;
    }  
    case type_short:
    {
      short *data = (short*) list->items;
      for (; i < list->count; i++)
        printf("%hi ", data[i]);
      break;
    }  
    case type_int:
    {
      int *data = (int*) list->items;
      for (; i < list->count; i++)
        printf("%i ", data[i]);
      break;
    }  
    case type_void:
    {
      long int *data = list->items;
      for (; i < list->count; i++)
        printf("%p ", data[i]);
      break;
    }  
    case type_long_double:
    {
      long double *data = (long double*) list->items;
      for (; i < list->count; i++)
        printf("%llf ", data[i]);
      break;
    }  
    default:
    {
      printf("CList: CListInfo - wrong DataType value!\n");
      break;
    }
  }   

  printf("\n\n");                    
}
