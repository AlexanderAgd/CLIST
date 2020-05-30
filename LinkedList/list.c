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
#include <stdint.h>
#include <assert.h>
#include "list.h"

typedef struct List_Node_
{
  struct List_Node_ *next;
  void *data;
} List_Node_;

typedef struct
{
  int count;          /* Number of items in the list. */
  size_t item_size;   /* Size of each item in bytes. */
  List_Node_ *first;
  List_Node_ *last;
} List_priv_;

void  List_Add_(List *l, void *o);
void  List_Insert_(List *l, void *o, int n);
void  List_Replace_(List *l, void *o, int n);
void  List_Remove_(List *l, int n);
void* List_At_(List *l, int n);
int   List_FirstIndex_(List *l, void *o);
int   List_LastIndex_(List *l, void *o);
int   List_Count_(List *l);
void  List_Clear_(List *l);
void  List_Free_(List *l);
void  List_print_(List *l, int n, char const *type);

List *List_Init(size_t objSize)
{
  List *lst = malloc(sizeof(List));
  List_priv_ *p = malloc(sizeof(List_priv_));
  if (!lst || !p)
  {
    fprintf(stderr, "List: ERROR! Can not allocate List!\n");
    return NULL;
  }
  p->count = 0;
  p->item_size = objSize;
  p->first = NULL;
  p->last = NULL;
  lst->add = &List_Add_;
  lst->insert = &List_Insert_;
  lst->replace = &List_Replace_;
  lst->remove = &List_Remove_;
  lst->at = &List_At_;
  lst->firstIndex = &List_FirstIndex_;
  lst->lastIndex = &List_LastIndex_;
  lst->count = &List_Count_;
  lst->clear = &List_Clear_;
  lst->free = &List_Free_;
  lst->print = &List_print_;
  lst->priv = p;
  return lst;
}

void List_Add_(List *l, void *o)              /* Add object to the end of a list */
{
  List_priv_ *p = (List_priv_*) l->priv;
  
  List_Node_ *node = malloc(sizeof(List_Node_));
  void *data = malloc(p->item_size);
  if (!node || !data)
  {
    fprintf(stderr, "List: ERROR! Can not allocate new node!\n");
    assert(node && data);
    return;
  }

  if (p->count)
  {
    p->last->next = node;
    node->data = data;
    p->last = node;
    node->next = NULL;
  }  
  else
  {
    node->next = NULL;
    node->data = data;
    p->first = node;
    p->last = node;
  }
  memcpy(data, o, p->item_size);
  p->count++;
}

void List_Insert_(List *l, void *o, int n)           /* Insert object at position 'n' */
{
  List_priv_ *p = (List_priv_*) l->priv;
  if (n > p->count || n < 0)
  {
    fprintf(stderr, "List: ERROR! Insert position outside range - %d; n - %d.\n", 
                        p->count, n);
    assert(n >= 0 && n <= p->count);
    return;
  }
  else if (n == p->count)
  {  
    List_Add_(l, o);
    return; 
  }

  List_Node_ *node = malloc(sizeof(List_Node_));
  void *data = malloc(p->item_size);
  if (!node || !data)
  {
    fprintf(stderr, "List: ERROR! Can not allocate new node!\n");
    assert(node && data);
    return;
  }

  List_Node_ *node_n = p->first;
  int i = 0;
  for (; i < n; i++)
    node_n = node_n->next;

  if (n)
  {
    node->data = data;
    node->next = node_n->next;
    node_n->next = node;
  }
  else
  {
    node->data = data;    
    node->next = node_n;
    p->first = node;
  }
  memcpy(data, o, p->item_size);
  p->count++;
}

void List_Replace_(List *l, void *o, int n)   /* Replace object at position 'n' */
{
  List_priv_ *p = (List_priv_*) l->priv;
  if (n >= p->count || n < 0)
  {
    fprintf(stderr, "List: ERROR! Replace position outside range - %d; n - %d.\n", 
                          p->count, n);
    assert(n >= 0 && n < p->count);
    return;
  }
  List_Node_ *node = p->first;
  int i = 0;
  for (; i < n; i++)
    node = node->next;

  memcpy(node->data, o, p->item_size);
}

void List_Remove_(List *l, int n)             /* Remove object at position 'n' */
{
  List_priv_ *p = (List_priv_*) l->priv;
  if (n >= p->count || n < 0)
  {
    fprintf(stderr, "List: ERROR! Remove position outside range - %d; n - %d.\n", 
                          p->count, n);
    assert(n >= 0 && n < p->count);
    return;
  }

  List_Node_ *tmp;
  List_Node_ *node = p->first;
  int i = 0;
  for (; i < n - 1; i++)
    node = node->next;

  if (n == 0)
  {
    tmp = node;
    if (node->next != NULL) 
      p->first = node->next;
    else
    {
      p->first = NULL;
      p->last = NULL;
    }
  }
  else
  {
    tmp = node->next;
    node->next = node->next->next;
    if (node->next == NULL)
      p->last = node;
  }  

  free(tmp->data);
  free(tmp);
  p->count--;
}

void* List_At_(List *l, int n)                /* Get object at position 'n' */
{
  List_priv_ *p = (List_priv_*) l->priv;
  if (n >= p->count || n < 0)
  {
    fprintf(stderr, "List: ERROR! Replace position outside range - %d; n - %d.\n", 
                          p->count, n);
    assert(n >= 0 && n < p->count);
    return NULL;
  }
  List_Node_ *node = p->first;
  int i = 0;
  for (; i < n; i++)
    node = node->next;

  return node->data;    
}

int List_FirstIndex_(List *l, void *o)       /* Get first index of the object */
{
  List_priv_ *p = (List_priv_*) l->priv;
  List_Node_ *node = p->first;
  int i = 0;
  for (; i < p->count; i++)
  {  
    if (strncmp(node->data, o, p->item_size) == 0)
      return i;
    node = node->next;
  }    
  return -1;
}

int List_LastIndex_(List *l, void *o)        /* Get last index of the object */
{
  List_priv_ *p = (List_priv_*) l->priv;
  List_Node_ *node = p->first;
  int index = -1;
  int i = 0;
  for (; i < p->count; i++)
  {  
    if (strncmp(node->data, o, p->item_size) == 0)
      index = i;
    node = node->next;
  }    
  return index;  
}

int List_Count_(List *l)                     /* Get list size */
{
  List_priv_ *p = (List_priv_*) l->priv;
  return p->count;
}

void List_Clear_(List *l)                     /* Clear list */
{
  List_priv_ *p = (List_priv_*) l->priv;
  List_Node_ *node = p->first;
  List_Node_ *tmp;
  int i = 0;
  for (; i < p->count; i++)
  { 
    tmp = node->next;
    free(node->data);
    free(node);
    node = tmp;
  }
  p->count = 0;
  p->first = NULL;
  p->last = NULL;
}

void List_Free_(List *l)
{
  List_Clear_(l);
  List_priv_ *p = (List_priv_*) l->priv;
  free(p);
  free(l);
}

void List_print_(List *l, int n, char const *type)
{
  List_priv_ *p = (List_priv_*) l->priv;
  printf("\nList:  count = %i  item_size = %zu   "
       "first = %p   last = %p \n", p->count, p->item_size, p->first, p->last);

  if (n > 0)
  {
    List_Node_ *node = p->first;
    int i = 0;
    for (; i < n && node != NULL; i++)
    {
      if (type == NULL) printf("%p  ", node->data);  /* Print out pointers */
      else if (strcmp(type, "char") == 0) printf("%c ", *(char*) node->data);
      else if (strcmp(type, "short") == 0) printf("%hi  ", *(short*) node->data);
      else if (strcmp(type, "int") == 0) printf("%i  ", *(int*) node->data);
      else if (strcmp(type, "long") == 0) printf("%li  ", *(long*) node->data);
      else if (strcmp(type, "uintptr_t") == 0) printf("%zx  ", (uintptr_t*) node->data);
      else if (strcmp(type, "size_t") == 0) printf("%zu  ", *(size_t*) node->data);
      else if (strcmp(type, "double") == 0) printf("%f  ", *(double*) node->data);
      else if (strcmp(type, "string") == 0) printf("%s\n", node->data);
      else { printf("Unknown type."); break; }
      node = node->next;    
    }
    printf("\n\n");
  }
}
