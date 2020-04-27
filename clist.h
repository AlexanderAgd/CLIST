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
  size_t count;
  size_t alloc_size;
  size_t item_size;
  void *items;
} CList;

void CList_exec(CList *list, void *obj, int *num, enum CListMode mode);

#ifdef __cplusplus
}
#endif

#endif /* CLIST_H */
