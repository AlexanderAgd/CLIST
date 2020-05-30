/******************************************/
/*                                        */
/*        Alexander Agdgomlishvili        */
/*                                        */
/*         cdevelopment@mail.com          */
/*                                        */
/******************************************/

#ifndef LIST_H
#define LIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct List
{
  void  (* add)        (struct List *l, void *o);            /* Add object to the end of a list */
  void  (* insert)     (struct List *l, void *o, int n);     /* Insert object at position 'n' */
  void  (* replace)    (struct List *l, void *o, int n);     /* Replace object at position 'n' */
  void  (* remove)     (struct List *l, int n);              /* Remove object at position 'n' */
  void* (* at)         (struct List *l, int n);              /* Get object at position 'n' */
  int   (* firstIndex) (struct List *l, void *o);            /* Get first index of the object */
  int   (* lastIndex)  (struct List *l, void *o);            /* Get last index of the object */
  int   (* count)      (struct List *l);                     /* Get list size */
  void  (* clear)      (struct List *l);                     /* Clear list */
  void  (* free)       (struct List *l);                     /* Destroy struct List and all data */
  void  (* print)      (struct List *l, int n, const char *type);  /* Print list data */
  void *priv;          /* NOT FOR USE, private data */
} List;

List *List_Init(size_t objSize); /* Set list object size in bytes */

#ifdef __cplusplus
}
#endif

#endif /* LIST_H */
