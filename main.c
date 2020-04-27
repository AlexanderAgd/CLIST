#include <stdio.h>
#include <stdlib.h>
#include "clist.h"

void CListInfo(CList *list)
{
  printf("-----------\n\n");	
  printf("list - %p \n&list - %p \n*list - %p\n", list, &list, *list);
  printf("list->item_size - %i \n&(list->item_size) - %p \n", list->item_size, &(list->item_size));

  list->item_size = 111;
}

int main(int argc, char **argv)
{
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

  p = 0;

  CList_exec(lst, ptr, &p, CList_Get);

  fprintf(stdout, "\n\nHello LIST\n");
  fprintf(stdout, "Count %i \nAlloc_size %i \n", lst->count, lst->alloc_size);
  fprintf(stdout, "Item_size %i \nptr value %i \n", lst->item_size, *ptr);
  fprintf(stdout, "=================================================\n\n");

  block = 23;
  CList_exec(lst, ptr, &p, CList_LastIndex);
  printf("p = %d \n\n", p);

  ptr = (short*) lst->items; 
  size_t i = 0;
  for (i; i<lst->count; i++)
    printf("%i \n", ptr[i]);
  printf("-----------\n"); 

  p = 2;
  CList_exec(lst, NULL, &p, CList_Remove);

    ptr = (short*) lst->items;
  for (i=0; i<lst->count; i++)
    printf("%i \n", ptr[i]);

  //printf("lst - %p \n&lst - %p \n*lst - %p\n", lst, &lst, *lst);
  //printf("lst->item_size - %i \n&(lst->item_size) - %p \n", lst->item_size, &(lst->item_size));
  //testIt(lst);
  //printf("\nlst->item_size == %i\n", lst->item_size);
  return 0;
}