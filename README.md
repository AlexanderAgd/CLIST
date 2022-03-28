# CLIST
C List, universal list solution for C language. Pure C but compatible to compile with C++.    
Supports all kind of data in a list: char, short, int, long, pointers, structs ... 
One struct and one init function - very easy and comfort usage, example:
```C
  struct unit
  {
    long int size;
    char str[24];
  } unit;

  char info[24] = "Random_Information";

  CList *list = CList_init(sizeof(unit));     /* Initialization */
 
  struct unit U;
  long int l;
  for (l = 0; l < 10; l++)
  {
    U.size = l;
    sprintf(U.str, info + l);
    list->add(list, &U);             /* Adding data at the end */
    list->insert(list, &U, 0);       /* Insert at position 0 */
  }
                                     /* Get item at position '2' */
  struct unit *tmp = list->at(list, 2);
  printf("Unit at index 2: size is %li, string is \"%s\"\n", tmp->size, tmp->str);

  l = list->count(list);             /* Get number of items in the list */
  list->print(list, 0, l, "long");   /* Print out 'l' elements of list, first member of struct is printed */
  size_t step = offsetof(struct unit, str);
  list->print(list, step, l, "string");
                                     /* Print out 'l' elements of list, second member of struct is printed */
  tmp = list->lastMatch(list, "Information", step, 24, 1);
                                     /* Find last struct with second member "Information" */
  int position = list->index(list);  /* Get index of previous search */
  printf("Item with index %i has string \"%s\" \n\n", position, tmp->str);

  list->free(list);                  /* Destroy all data and list */
``` 

Code may be used as static library or just included in your own code as object.
Automated reallocation of memory block only at some points when adding, inserting or removing elements.
May be used on any OS that supports C standart library:
Windows, Linux, Mac OS, any Unix, Android, iOS and others.  
Here CList struct with implemented functions:  
```C
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
```
Improved search functions, now we have firstMatch and lastMatch to search data in a list, 
firstMatch starts search from 0 to last list item and lastMatch search from last to 0.
New functions allow compare whole list item or any member of it in case item is struct or class.
These functions use string compare or memory compare depend on last parameter "int string".
Second parameter "const void *o" pointer to data we are comparing.
Third parameter "size_t shift" is difference in bytes from struct pointer to struct member 
we want to compare, you can use "offsetof" function or just 0 if scanning whole list item.
Fourth paramerer "size_t size" is max length or size of member to be compared.
When we compare whole struct or other kind of list item shift is 0 and size can be set to 0, 
when shift and size is 0 - size will be set as "itemSize" automatically.
```C
firstMatch(struct CList *l, const void *o, size_t shift, size_t size, int string);
/* Check usage at test.c file */
```  
If you are working with a huge number of elements and think that your list holds 
a lot of memory or you do not plan add more items, you can use "realloc" and 
specify an exact number of items in the list:
```C
int num = list->count(list);
list->realloc(list, num);
```
Read test.c file for usage examples.

## Build
Clean and build:
<pre>
make clean && make	
</pre>
Run test executable for list examples and perfomance.

## Perfomance

<pre> 
CPU: Intel(R) Core(TM) i7-4770K CPU @ 3.50GHz

Add of 10000 int takes  -  51 microseconds
Remove from position '0' of 10000 int takes  -  3072 microseconds
Insert to position '0' of 10000 int takes  -  3017 microseconds
Remove from last position of 10000 int takes  -  88 microseconds
Insert to last position of 10000 int takes  -  105 microseconds
Replace of 10000 int takes  -  49 microseconds
</pre>

## License

BSD 2-Clause License

Copyright (c) 2020, Alexander Agdgomlishvili
cdevelopment@mail.com

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

