/******************************************/
/*                                        */
/*        Alexander Agdgomlishvili        */
/*                                        */
/*         cdevelopment@mail.com          */
/*                                        */
/******************************************/

#ifndef CLIST_H
#define CLIST_H

#ifdef __cplusplus
extern "C" {
#endif

enum CListMode
{
  CList_Init,        /* List allocating - size may be manual or automated */
  CList_ReAlloc,     /* List size reallocating - size manual or automated */
  CList_Add,         /* Add object to the end of a list */
  CList_Insert,      /* Insert object at position 'N' */
  CList_Remove,      /* Remove object at position 'N' */
  CList_Get,         /* Get object at position 'N' */
  CList_FirstIndex,  /* Get first index of the object */
  CList_LastIndex,   /* Get last index of the object */
  CList_Clear        /* Clear list */
};

typedef struct CList
{
  size_t count;      /* Number of items in the list. */
  size_t alloc_size; /* Allocated size in quantity of items - not bytes! */
  size_t item_size;  /* Size of each item in bytes. */
  void *items;       /* Pointer to the array list */
} CList;

void CList_exec(CList *list, void *obj, int *num, enum CListMode mode);

#ifdef __cplusplus
}
#endif

#endif /* CLIST_H */
