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
  void * (* add)         (struct CList *l, void *o);            /* Add object to the end of a list */
  void * (* insert)      (struct CList *l, void *o, int n);     /* Insert object at position 'n' */
  void * (* replace)     (struct CList *l, void *o, int n);     /* Replace object at position 'n' */
  void   (* remove)      (struct CList *l, int n);              /* Remove object at position 'n' */
  void * (* at)          (struct CList *l, int n);              /* Get object at position 'n' */
  int    (* realloc)     (struct CList *l, int n);              /* Reallocate list to 'size' items */
  int    (* count)       (struct CList *l);                     /* Get list size in items */
  void * (* firstMatch)  (struct CList *l, const void *o, size_t shift, size_t size, int string);
                                                                /* Returns object with first match of string or byte compare */
  void * (* lastMatch)   (struct CList *l, const void *o, size_t shift, size_t size, int string);
                                                                /* Returns object with last match of string or byte compare */
  int    (* index)       (struct CList *l);                     /* Get index of previos search match */
  int    (* swap)        (struct CList *l, int a, int b);       /* Swap, replace two items with index a b */
  int    (* allocSize)   (struct CList *l);                     /* Get allocated size in items */
  size_t (* itemSize)    (struct CList *l);                     /* Get item size in bytes */
  void   (* print)       (struct CList *l, size_t shift, int n, const char *type);   /* Print list data */
  void   (* clear)       (struct CList *l);                     /* Clear list */
  void   (* free)        (struct CList *l);                     /* Destroy struct CList and all data */
  void  *priv;           /* NOT FOR USE, private data */
} CList;

CList *CList_init(size_t objSize); /* Set list object size in bytes */

/*  void *add(struct CList *l, void *o);
        Returns pointer to added object; Returns NULL if failed.

    void *insert(struct CList *l, void *o, int n);
        Returns pointer to inserted object; Returns NULL if failed.

    void *replace(struct CList *l, void *o, int n);
        Returns pointer to replaced object; Returns NULL if failed.

    void *at(struct CList *l, int n);
        Returns pointer to object at index n;

    int realloc(struct CList *l, int n);
        Return 1 when success. Returns 0 if failed.

    void *firstMatch(struct CList *l, const void *o, size_t shift, size_t size, int string);
        Returns pointer to list item when first match found. Straight scanning, from 0 to list end.
        Returns NULL if search failed.  

    void *lastMatch(struct CList *l, const void *o, size_t shift, size_t size, int string);
        Returns pointer to list item when first match found. Reverse scanning, from list end to 0.
        Returns NULL if search failed.

    int index(struct CList *l);
        Returns index of last search firstMatch or lastMatch. Returns -1 if search failed.
    
    void print(struct CList *l, size_t shift, int n, const char *type);
        Prints data of "int n" list items with offset "size_t shift" and type "const char *type".
        Supported types: char, short, int, long, uintptr_t, size_t, double, string.
        If type is NULL just pointers data will be printed. 
*/

#ifdef __cplusplus
}
#endif

#endif /* CLIST_H */
