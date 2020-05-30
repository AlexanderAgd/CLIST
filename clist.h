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

typedef struct CList
{
  void  (* add)        (struct CList *l, void *o);            /* Add object to the end of a list */
  void  (* insert)     (struct CList *l, void *o, int n);     /* Insert object at position 'n' */
  void  (* replace)    (struct CList *l, void *o, int n);     /* Replace object at position 'n' */
  void  (* remove)     (struct CList *l, int n);              /* Remove object at position 'n' */
  void* (* at)         (struct CList *l, int n);              /* Get object at position 'n' */
  int   (* realloc)    (struct CList *l, int n);              /* Reallocate list to 'size' items */
  int   (* firstIndex) (struct CList *l, void *o);            /* Get first index of the object */
  int   (* lastIndex)  (struct CList *l, void *o);            /* Get last index of the object */
  int   (* count)      (struct CList *l);                     /* Get list size */
  void  (* clear)      (struct CList *l);                     /* Clear list */
  void  (* free)       (struct CList *l);                     /* Destroy struct CList and all data */
  void  (* print)      (struct CList *l, int n, const char *type);  /* Print list data */
  void *priv;          /* NOT FOR USE, private data */
} CList;

CList *CList_Init(size_t objSize); /* Set list object size in bytes */

#ifdef __cplusplus
}
#endif

#endif /* CLIST_H */
