# CLIST
C List, universal list solution for C language. Pure C but compatible to compile with C++.    
Supports all kind of data in a list: char, short, int, long, pointers, structs.
One struct and one init function - very easy and comfort usage, example:
<pre>
  struct unit
  {
    long int size;
    void *ptr;
  } unit;

  CList *list = CList_Init(sizeof(unit));     /* Initialization */
 
  long int i; 
  for (i = 0; i < 10; i++)
  {
    struct unit U = { i, &i };
    list->add(list, &U);            /* Adding data at the end */
    list->insert(list, &U, 0);      /* Insert at position 0 */    
  }
                                    /* Get item at position '1' */
  struct unit *tmp = (struct unit*) list->at(list, 1);  
  printf("Unit size is %li, pointer is %p \n", tmp->size, tmp->ptr);

  i = list->count(list);            /* Get number of items in the list */
  list->print(list, i, "long");     /* Print out 'i' elements of list, first element of struct is shown */

  list->free(list);                 /* Destroy all data and list */ 
</pre>  

Code may be used as static library or just included in your own code as object.
Automated reallocation of memory block only at some points when adding, inserting or removing elements.
May be used on any OS that supports C standart library:
Windows, Linux, Mac OS, any Unix, Android, iOS and others.  
Here CList struct with implemented functions:  
<pre>
typedef struct CList
{
  void  (* add)        (struct CList *l, void *o);            /* Add object to the end of a list */
  void  (* insert)     (struct CList *l, void *o, int n);     /* Insert object at position 'n' */
  void  (* replace)    (struct CList *l, void *o, int n);     /* Replace object at position 'n' */
  void  (* remove)     (struct CList *l, int n);              /* Remove object at position 'n' */
  void* (* at)         (struct CList *l, int n);              /* Get object at position 'n' */
  int   (* realloc)    (struct CList *l, int n);              /* Reallocate list to 'n' items */
  int   (* firstIndex) (struct CList *l, void *o);            /* Get first index of the object */
  int   (* lastIndex)  (struct CList *l, void *o);            /* Get last index of the object */
  int   (* count)      (struct CList *l);                     /* Get list size */
  void  (* clear)      (struct CList *l);                     /* Clear list */
  void  (* free)       (struct CList *l);                     /* Destroy struct CList and all data */
  void  (* print)      (struct CList *l, int n, const char *type);  /* Print list data */
  void* priv;          /* NOT FOR USE, private data */
} CList;
</pre>
If you are working with a huge number of elements and think that your list holds 
a lot of memory or you do not plan add more items, you can use "realloc" and 
specify an exact number of items in the list:
<pre>
int num = list->count(list);
list->realloc(list, num);
</pre>
You can check also BlockList version of CList, it has more computations 
and a little slow to compare to current list, but uses less memory at some points. 
Block list allocates and deallocates memory automatically by memory blocks. It's a mix 
of linked list and dynamic list.   
Check also standard C linked list at folder LinkedList. It's classic C list with nodes.
Run tests and check how fast is these lists.
The fastest is currect "CList", then block list "BList", and linked list "List" is last.

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

