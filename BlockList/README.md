# CLIST
BlockList is another version of CLIST its differs from previous list how it allocates and 
deallocates memory blocks. When you add item BlockList allocates block for 64 items by default
(you can specify block size), when all slots filled - another 64 block will be added. 
In the same logic it deallocates blocks. When you remove all elements - all blocks is destroyed. 
You can set block size in an initialization of a list.
<pre>
 BList *lst = BList_Init(sizeof(struct info), 32); /* 32 is a block size in items */	
</pre>	
Also supports all kind of data in a list: char, short, int, long, pointers, structs.
For usage check test.c and perfomace run "test".
