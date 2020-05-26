# CLIST
BlockList is another version of CLIST its differs from previous list how it allocates and 
deallocates memory blocks. When you add item BlockList allocates block for 64 items by default
(you can specify block size), when all slots filled - another 64 block will be added. 
In the same logic it deallocates blocks. When you remove all elements - all blocks is destroyed. 
Also BlockList has "private" CList data.
<pre>
typedef struct CList *CList;
</pre>
You can set block size by "CList_SetBlockSize" after initialization of a list. 
Also supports all kind of data in a list: char, short, int, long, void pointers, structs.
For usage check test.c and perfomace run "test".
