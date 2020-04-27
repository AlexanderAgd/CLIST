# CLIST
Universal list solution for C language.
Supports all kind of data: char, short int, int, long int, void, long double and other.
  
typedef struct CList  
{  
  size_t count;       /* Number of items in the list. */  
  size_t alloc_size;  /* Allocated size in quantity of items - not bytes! */  
  size_t item_size;   /* Size of each item in bytes. */  
  void *items;        /* Pointer to the array list */  
} CList;  
  
One universal function:  
void CList_exec(CList *list, void *obj, int *num, enum CListMode mode);  
Does all necessary operation with a list depend on the CListMode mode, see clist.h header.
Manual and automated memory size allocation when use initialization of array and
automated reallocation of memory size when adding or inserting new elements.
Add your remarks or question if any :) THanks.
