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
  CList_Init,
  CList_ReAlloc,
  CList_Add,
  CList_Insert,
  CList_Remove,
  CList_Get,
  CList_FirstIndex,
  CList_LastIndex,
  CList_Clear
} CListMode;

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
