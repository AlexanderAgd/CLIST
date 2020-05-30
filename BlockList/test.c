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
#include "blist.h"

#if defined(WIN32)
int gettimeofday(struct timeval *tp, void *tzp);
#endif

size_t diff_usec(struct timeval start, struct timeval end)
{ 
  return (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec);
}

int main(int argc, char **argv)
{
  void *obj = NULL;
  int i = 0;
  int n = 0;

  /******************************************************* CHAR LIST */

  n = sizeof(char); 
  BList *lst = BList_Init(n, 32);

  for (i = 33; i < 123; i++)
  { 
    obj = &i;  
    lst->add(lst, obj); /* Adding obj items to the end of array */
  }
  lst->print(lst, 150, "char");

  n = 32;
  char ch = 'A';
  obj = lst->at(lst, n); /* Get 32nd item of array */
  printf("Position %i contains symbol \'%c\'\n", n, *((char*)obj));

  ch = '!';
  obj = &ch;
  n = lst->firstIndex(lst, obj);  /* Find first index of '!' char */
  printf("Index of \'%c\' is %i\n", ch, n);

  ch = '+';
  n = lst->firstIndex(lst, obj);
  printf("Index of \'%c\' is %i\n", ch, n);

  n = 0;
  for (i = 15; i > 0; i--)
    lst->remove(lst, 0); /* Remove item at index 0 from array 15 times */

  lst->print(lst, 150, "char");
  lst->free(lst);

  /******************************************************* SHORT INT LIST */
 
  n = sizeof(short); 
  lst = BList_Init(n, 0); /* When block size set 0 default size 64 used instead */

  short sh = 1001;
  for (i = 0; i < 24; i++, sh += 1000)
  {
    obj = &sh;
    lst->add(lst, obj);
  }

  lst->print(lst, 100, "short");

  sh = 5001;
  lst->insert(lst, &sh, 20);   /* Insert value of 'sh' to position 20 */

  lst->insert(lst, &sh, 25);   /* Insert value of 'sh' to position 25 */ 
  lst->print(lst, lst->count(lst) - 1, "short");

  n = lst->lastIndex(lst, &sh); /* Find last index of '5001' short */  
  printf("Last index of \'%i\' is %i\n", sh, n);

  while (n != -1)
  {
    n = lst->firstIndex(lst, &sh); /* Find first index of '5001' short */

    if (n != -1)                   /* Remove it from the list */
      lst->remove(lst, n);
  }

  sh = 1111;
  lst->replace(lst, &sh, 0); /* Replace object at position '0' */
  lst->print(lst, 100, "short");

  lst->clear(lst); /* CLear list */
  lst->free(lst);
  
  /******************************************************* STRUCT LIST */

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

  n = sizeof(sample);
  lst = BList_Init(n, 8);

  printf("Size of struct 'sample' = %i bytes\n", sizeof(sample));

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
    lst->insert(lst, obj, n); /* Insert each object at index '0' of array */
  }

  lst->print(lst, 20, NULL);

  int j;
  int count = lst->count(lst);
  for (j = 0; j < 6; j++, i = 0)  /* Print out struct content */
  {  
    for (i = 0; i < count; i++)
    {
      struct sample *sam = lst->at(lst, i);

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

  lst->free(lst);

  /******************************************************* POINTER LIST */

  /* If we want create list of objects located in diffent places of memory, */
  /* let create pointers list or in our case "uintptr_t" address list       */

  printf("\nSize of 'uintptr_t' = %i bytes\n", sizeof(uintptr_t));

  n = sizeof(uintptr_t);
  lst = BList_Init(n, 64);

  struct sample sm1 = { 64, 6.4, 4, &n, 16, obj };
  struct sample sm2 = { 128, 12.8, 8, &j, { 1024 }, (void*)&sh }; /* Just some sample data */

  uintptr_t addr = (uintptr_t) &sm1;    /* Cast reference to address value */
  lst->add(lst, &addr);

  addr = (uintptr_t) &sm2;
  lst->add(lst, &addr);

  struct sample *sm3 = malloc(sizeof(sample));
  sm3->l = 256;
  sm3->d = 25.6;
  sm3->s = 16;
  sm3->v = &sm1;
  sm3->u.lli = 2048;
  sm3->dat = (void*) &sm2;

  addr = (uintptr_t) sm3;
  lst->add(lst, &addr);

  lst->print(lst, 20, "uintptr_t");

  count = lst->count(lst);
  for (j = 0; j < 6; j++, i = 0) /* Print out struct content */
  {  
    for (i = 0; i < count; i++)
    {
      uintptr_t *ad = lst->at(lst, i);
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
  lst->free(lst);

  /******************************************************* PERFOMANCE TEST */

  n = sizeof(int);
  lst = BList_Init(n, 0);

  size_t time;
  int pos = 0;
  i = 0;
  n = 10000;
  struct timeval start;
  struct timeval end;

  printf("PERFOMANCE TEST - 1 second contains 1000000 microseconds\n\n");
  
  gettimeofday(&start, NULL);
  for(i=0; i < n; i++)
    lst->add(lst, &i);
  gettimeofday(&end, NULL);
  time = diff_usec(start, end);
  printf("Add of %i int takes  -  %zu microseconds\n", n, time);

  gettimeofday(&start, NULL);
  for(i=0; i < n; i++)
    lst->remove(lst, 0);
  gettimeofday(&end, NULL);
  time = diff_usec(start, end);
  printf("Remove from position '0' of %i int takes  -  %zu microseconds\n", n, time);

  gettimeofday(&start, NULL);
  for(i=0; i < n; i++)
    lst->insert(lst, &i, 0);
  gettimeofday(&end, NULL);
  time = diff_usec(start, end);
  printf("Insert to position '0' of %i int takes  -  %zu microseconds\n", n, time);

  gettimeofday(&start, NULL);
  for(i=0; i < n; i++)
    lst->remove(lst, lst->count(lst) - 1);
  gettimeofday(&end, NULL);
  time = diff_usec(start, end);
  printf("Remove from last position of %i int takes  -  %zu microseconds\n", n, time);

  gettimeofday(&start, NULL);
  for(i=0; i < n; i++)
    lst->insert(lst, &i, lst->count(lst));
  gettimeofday(&end, NULL);
  time = diff_usec(start, end);
  printf("Insert to last position of %i int takes  -  %zu microseconds\n", n, time);

  gettimeofday(&start, NULL);
  for(i=0; i < n; i++)
    lst->replace(lst, &n, i);
  gettimeofday(&end, NULL);
  time = diff_usec(start, end);
  printf("Replace of %i int takes  -  %zu microseconds\n", n, time);

  printf("\n");
  lst->free(lst);

  return 0;

} /**** MAIN ****/

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
