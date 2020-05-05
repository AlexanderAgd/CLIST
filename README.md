# CLIST
C List, universal list solution for C language.
Supports all kind of data in a list: char, short, int, long, void pointers, long double, structs.
One enum, one struct and one function - very easy to use.
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
Enum sets supported operations:  
<pre>
enum CListMode
{
  CList_Init,        /* List allocating - size may be manual or automated */
  CList_ReAlloc,     /* List size reallocating - size manual or automated */
  CList_Add,         /* Add object to the end of a list */
  CList_Insert,      /* Insert object at position 'N' */
  CList_Replace,     /* Replace object at position 'N' */
  CList_Remove,      /* Remove object at position 'N' */
  CList_Get,         /* Get object at position 'N' */
  CList_FirstIndex,  /* Get first index of the object */
  CList_LastIndex,   /* Get last index of the object */
  CList_Clear        /* Clear list */
}; 
</pre> 

## Build
Just make command:
<pre>
make clean && make	
</pre>
Run test executable for list examples and perfomance.

## Perfomance

<pre> 
CPU: Intel(R) Core(TM) i7-4770K CPU @ 3.50GHz

Initialization of 10000 int array takes  -  3 microseconds.
Add of 10000 int to array takes  -  122 microseconds.
Remove from position '0' of 10000 int of array takes  -  3234 microseconds.
Insert to position '0' of 10000 int to array takes  -  3514 microseconds.
Replace of 10000 int in array takes  -  49 microseconds.
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

