# CLIST
Universal list solution for C language.
Supports all kind of data in a list: char, short, int, long, void, pointes, long double, structs.
One enum, one struct and one function - very EASY.
<pre>  
typedef struct CList  
{  
  size_t count;       /* Number of items in the list. */  
  size_t alloc_size;  /* Allocated size in quantity of items */  
  size_t item_size;   /* Size of each item in bytes. */  
  void *items;        /* Pointer to the list */  
} CList;  
</pre>  
One universal function:  
<pre>
void *CList_exec(CList *list, void *obj, int *num, enum CListMode mode);
</pre>
Does all necessary operation with a list depend on the CListMode mode, see clist.h header.
Manual and automated memory size allocation when use initialization of list and
automated reallocation of memory size when adding or inserting new elements.
May be used on x16 x32 x64 bit systems, any OS that supports C standart library:
Windows, Linux, Mac OS, any Unix, Android, iOS and others. Examples of usage can be find
in 'test.c' file.     
Add your remarks or question if any :) Thanks.
