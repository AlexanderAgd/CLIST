/******************************************/
/*                                        */
/*        Alexander Agdgomlishvili        */
/*                                        */
/*         cdevelopment@mail.com          */
/*                                        */
/******************************************/

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#include <unistd.h>
#endif

#if defined(_POSIX_VERSION)
#include <sys/time.h>
#elif defined(WIN32)
#include <Winsock2.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "clist.h"

enum DataType
{                    /* bytes below true for x64 bit system */
  type_char,         /* 1 byte */
  type_short,        /* 2 byte */
  type_int,          /* 4 byte */
  type_void_ptr,     /* 8 byte */
};

void CListInfo(CList *list, enum DataType byte);

#if defined(WIN32)
int gettimeofday(struct timeval *tp, void *tzp);
#endif

size_t diff_usec(struct timeval start, struct timeval end)
{ 
  return (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec);
}

int main(int argc, char **argv)
{
  /******************************************************* CHAR ARRAY */

  void *obj = NULL;
  int i = 0;
  int n = 0;
  CList list = { .count = 0,
                 .alloc_size = 0,
                 .item_size = sizeof(char), /* 1 byte */
                 .items = NULL
               };

  CList_exec(&list, NULL, NULL, CList_Init); /* 128 byte allocated  automatically for char array */

  printf("\n");
  CListInfo(&list, type_char);
  
  for (i = 33; i < 123; i++)
  { 
    obj = &i;  
    CList_exec(&list, obj, NULL, CList_Add); /* Adding obj items to the end of array */
  }

  CListInfo(&list, type_char);

  n = 32;
  char ch = 'A';
  obj = CList_exec(&list, NULL, &n, CList_Get); /* Get 32nd item of array */
  printf("Position %i contains symbol \'%c\'\n", n, *((char*)obj));

  ch = '!';
  obj = &ch;
  CList_exec(&list, obj, &n, CList_FirstIndex);  /* Find first index of '!' char */
  printf("Index of \'%c\' is %i\n", ch, n);

  ch = '+';
  CList_exec(&list, obj, &n, CList_FirstIndex);
  printf("Index of \'%c\' is %i\n\n", ch, n);

  n = 0;
  for (i = 15; i > 0; i--)
    CList_exec(&list, obj, &n, CList_Remove); /* Remove item at index 0 from array 15 times */

  CListInfo(&list, type_char);

  CList_exec(&list, NULL, NULL, CList_Clear); /* Set struct size_t params to '0' and free array "items" */
                                              /* Only item_size is unchanged. */

  /******************************************************* SHORT INT ARRAY */

  list.item_size = sizeof(short); /* 2 byte */
  
  short sh = 1001;
  for (i = 0; i < 24; i++, sh += 1000)
  {
    obj = &sh;
    CList_exec(&list, obj, NULL, CList_Add); /* No CList_Init used  after clear, automated allocation */ 
  }

  CListInfo(&list, type_short);

  sh = 5001;
  n = 20;
  CList_exec(&list, obj, &n, CList_Insert);   /* Insert value of 'sh' to position 20 */
  n = 25;
  CList_exec(&list, obj, &n, CList_Insert);   /* Insert value of 'sh' to position 25 */
  
  CListInfo(&list, type_short);

  n = 0;
  CList_exec(&list, obj, &n, CList_LastIndex); /* Find last index of '5001' short */
  printf("Last index of \'%i\' is %i\n\n", sh, n);

  while (n != -1)
  {
    CList_exec(&list, obj, &n, CList_FirstIndex); /* Find first index of '5001' short */

    if (n != -1)
      CList_exec(&list, NULL, &n, CList_Remove); /* Remove it from the list */
  }

  n = 0;
  sh = 1111;
  CList_exec(&list, &sh, &n, CList_Replace); /* Replace object at position '0' */

  CListInfo(&list, type_short);
  CList_exec(&list, NULL, NULL, CList_Clear); /* CLear list */
  
  /******************************************************* STRUCT ARRAY */
  struct sample
  {
    long int l;
    double d;
    int s;
    void *v;
    union u
    {
      long long lli;
      char c;
    } u;
    union u *dat;
  } sample;

  CList *newlist = malloc(sizeof(CList));
  newlist->item_size = sizeof(sample);     /* Always set item_size before allocation */
  n = 16;

  CList_exec(newlist, NULL, &n, CList_Init); /* 'newlist->items' allocation */

  printf("Size of struct 'sample' = %i bytes\n\n", sizeof(sample));

  n = 0;
  for (i = 0; i < 8; i++) /* Let create list of a struct */
  { 
    struct sample sam = 
    {
      .l = 10 * i,
      .d = 5.010101 * i,
      .s = i,
      .v = &sam,
      .u = { .lli = 11 * i },
      .dat = &(sam.u)
    };

    obj = &sam;
    CList_exec(newlist, obj, &n, CList_Insert); /* Insert each object at index '0' of array */
  }

  CListInfo(newlist, type_void_ptr);

  int j;
  for (j = 0; j < 6; j++, i = 0)  /* Print out struct content */
  {  
    for (i = 0, n = 0; i < 8; i++, n++)
    {
      struct sample *sam = CList_exec(newlist, NULL, &n, CList_Get);

      switch (j)
      {
        case 0: printf("%15li ", sam->l); break;
        case 1: printf("%15lf ", sam->d); break;
        case 2: printf("%15i ", sam->s); break;
        case 3: printf("%15p ", sam->v); break;
        case 4: printf("%15lli ", sam->u.lli); break;
        case 5: printf("%15p ", sam->dat); break;
        default: break;
      }
    }
    printf("\n");
  }

  CList_exec(newlist, NULL, NULL, CList_Clear);

  /******************************************************* POINTER ARRAY */

  /* If we want create list of objects located in diffent places of memory, */
  /* let create pointers list or in our case "uintptr_t" address list       */

  newlist->item_size = sizeof(uintptr_t);
  printf("\nSize of 'uintptr_t' = %i bytes\n\n", sizeof(uintptr_t));

  n = 4;
  CList_exec(newlist, NULL, &n, CList_Init);

  struct sample sm1 = { 64, 6.4, 4, &n, 16, obj };
  struct sample sm2 = { 128, 12.8, 8, &j, { 1024 }, (void*)&sh }; /* Just some sample data */

  uintptr_t addr = (uintptr_t) &sm1;    /* Cast reference to address value */
  CList_exec(newlist, &addr, NULL, CList_Add);

  addr = (uintptr_t) &sm2;
  CList_exec(newlist, &addr, NULL, CList_Add);

  struct sample *sm3 = malloc(sizeof(sample));
  sm3->l = 256;
  sm3->d = 25.6;
  sm3->s = 16;
  sm3->v = &sm1;
  sm3->u.lli = 2048;
  sm3->dat = (void*) &sm2;

  addr = (uintptr_t) sm3;
  CList_exec(newlist, &addr, NULL, CList_Add);

  CListInfo(newlist, type_void_ptr);

  for (j = 0; j < 6; j++, i = 0) /* Print out struct content */
  {  
    for (i = 0, n = 0; i < 3; i++, n++)
    {
      uintptr_t *ad = CList_exec(newlist, NULL, &n, CList_Get);
      struct sample *sam = (struct sample*) *ad; /* Cast address value to struct pointer */
      switch (j)
      {
        case 0: printf("%15li ", sam->l); break;
        case 1: printf("%15lf ", sam->d); break;
        case 2: printf("%15i ", sam->s); break;
        case 3: printf("%15p ", sam->v); break;
        case 4: printf("%15lli ", sam->u.lli); break;
        case 5: printf("%15p ", sam->dat); break;
        default: break;
      }
    }
    printf("\n");
  }

  printf("\n");
  free(sm3);
  CList_exec(newlist, NULL, NULL, CList_Clear);
  free(newlist);

  /******************************************************* PERFOMANCE TEST */

  size_t time;
  int pos = 0;
  i = 0;
  n = 10000;
  list.item_size = sizeof(int);
  struct timeval start;
  struct timeval end;

  printf("PERFOMANCE TEST - 1 second contains 1000000 microseconds\n\n");
  
  gettimeofday(&start, NULL);
  CList_exec(&list, NULL, &n, CList_Init); /*   */
  gettimeofday(&end, NULL);

  time = diff_usec(start, end);
  printf("Initialization of %i int array takes  -  %lu microseconds\n", n, time);

  gettimeofday(&start, NULL);
  for(i=0; i < n; i++)
    CList_exec(&list, &i, NULL, CList_Add);
  gettimeofday(&end, NULL);

  time = diff_usec(start, end);
  printf("Add of %i int to array takes  -  %lu microseconds\n", n, time);

  gettimeofday(&start, NULL);
  for(i=0; i < n; i++)
    CList_exec(&list, NULL, &pos, CList_Remove);
  gettimeofday(&end, NULL);

  time = diff_usec(start, end);
  printf("Remove from position '0' of %i int of array takes  -  %lu microseconds\n", n, time);

  pos = 0;
  gettimeofday(&start, NULL);
  for(i=0; i < n; i++)
    CList_exec(&list, &i, &pos, CList_Insert);
  gettimeofday(&end, NULL);

  time = diff_usec(start, end);
  printf("Insert to position '0' of %i int to array takes  -  %lu microseconds\n", n, time);

  pos = 0;
  gettimeofday(&start, NULL);
  for(i=0; i < n; i++)
    CList_exec(&list, &n, &i, CList_Replace);
  gettimeofday(&end, NULL);

  time = diff_usec(start, end);
  printf("Replace of %i int in array takes  -  %lu microseconds\n", n, time);

  printf("\n");
  CList_exec(&list, NULL, NULL, CList_Clear);
  return 0;

} /**** MAIN ****/

void CListInfo(CList *list, enum DataType byte)
{
  printf("CList:   count = %lu   alloc_size = %lu   item_size = %lu \n", 
    list->count, list->alloc_size, list->item_size);

  size_t i = 0;
  switch(byte)
  {
    case type_char:
    {
      char *data = (char*) list->items;
      for (; i < list->count; i++)
        printf("%c ", data[i]);
      break;
    }
    case type_short:
    {
      short *data = (short*) list->items;
      for (; i < list->count; i++)
        printf("%hi ", data[i]);
      break;
    }
    case type_int:
    {
      int *data = (int*) list->items;
      for (; i < list->count; i++)
        printf("%i ", data[i]);
      break;
    }
    case type_void_ptr:
    {
      void *data = list->items;
      for (; i < list->count; i++, data += list->item_size)
        printf("%p  ", data);
      break;
    } 
    default:
    {
      printf("CList: CListInfo - wrong DataType value!\n");
      break;
    }
  }

  printf("\n\n");                    
}

#if defined(WIN32)
int gettimeofday(struct timeval *tp, void *tzp)
{
  LARGE_INTEGER ticksPerSecond;
  LARGE_INTEGER tick;
  LARGE_INTEGER time;

  if (QueryPerformanceFrequency(&ticksPerSecond) == 0) 
    return -1;
  QueryPerformanceCounter(&tick);  /* what time is it? */

  time.QuadPart = tick.QuadPart / ticksPerSecond.QuadPart; /* convert the tick number into the number */ 
                                                           /* of seconds since the system was started...  */
  tp->tv_sec = time.QuadPart; /* seconds */
  tp->tv_usec = (tick.QuadPart - (time.QuadPart * ticksPerSecond.QuadPart)) * 
                     1000000.0 / ticksPerSecond.QuadPart;
  return 0;
}
#endif
