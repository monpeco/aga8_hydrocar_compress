#include <stdio.h>      /* printf */
#include <stdlib.h>     /* qsort */

int values[] = { 40, 10, 100, 90, 20, 25 };

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

int main ()
{
  int n;
  
  printf ("Before qsort()\n");
  for (n=0; n<6; n++)
     printf ("%d ",values[n]);  
  printf ("\n");

  qsort (values, 6, sizeof(int), compare);

  printf ("After qsort()\n");
  for (n=0; n<6; n++)
     printf ("%d ",values[n]);
  printf ("\n");
     
  return 0;
}