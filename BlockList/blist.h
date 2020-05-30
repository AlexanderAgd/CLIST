/******************************************/
/*                                        */
/*        Alexander Agdgomlishvili        */
/*                                        */
/*         cdevelopment@mail.com          */
/*                                        */
/******************************************/

#ifndef BLIST_H
#define BLIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BList
{
  void  (* add)        (struct BList *l, void *o);            /* Add object to the end of a list */
  void  (* insert)     (struct BList *l, void *o, int n);     /* Insert object at position 'n' */
  void  (* replace)    (struct BList *l, void *o, int n);     /* Replace object at position 'n' */
  void  (* remove)     (struct BList *l, int n);              /* Remove object at position 'n' */
  void* (* at)         (struct BList *l, int n);              /* Get object at position 'n' */
  int   (* firstIndex) (struct BList *l, void *o);            /* Get first index of the object */
  int   (* lastIndex)  (struct BList *l, void *o);            /* Get last index of the object */
  int   (* count)      (struct BList *l);                     /* Get list size */
  void  (* clear)      (struct BList *l);                     /* Clear list */
  void  (* free)       (struct BList *l);                     /* Destroy struct CBList and all data */
  void  (* print)      (struct BList *l, int n, const char *type);  /* Print list data */
  void *priv;          /* NOT FOR USE, private data */
} BList;

BList *BList_Init(size_t objSize, size_t blockSize); /* Set list object size in bytes and block size */

#ifdef __cplusplus
}
#endif

#endif /* BLIST_H */
