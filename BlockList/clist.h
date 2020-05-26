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
  CList_Init,          /* List allocating */
  CList_Add,           /* Add object to the end of a list */
  CList_Insert,        /* Insert object at position 'N' */
  CList_Replace,       /* Replace object at position 'N' */
  CList_Remove,        /* Remove object at position 'N' */
  CList_Get,           /* Get object at position 'N' */
  CList_FirstIndex,    /* Get first index of the object */
  CList_LastIndex,     /* Get last index of the object */
  CList_Clear,         /* Clear list */
  CList_Count,         /* Get list size */
  CList_SetBlockSize,  /* Set "block" size */
  CList_BlockSize      /* Get block size */
};

typedef struct CList *CList; /* CList is private, Init and use it */ 

void *CList_exec(CList list, void *obj, int *num, enum CListMode mode);

void CList_print(CList list);

#ifdef __cplusplus
}
#endif

#endif /* CLIST_H */
