#include <stdio.h>      /* printf */
#include <stdlib.h>     /* qsort, bsearch, NULL */
#include <string.h>     /* strcmp */

char strvalues[][20] = {"some","example","strings","here"};

int main ()
{
  char * pItem;
  char key1[20] = "example";
  char key2[20] = "test";

  /* sort elements in array: */
  qsort (strvalues, 4, 20, (int(*)(const void*,const void*)) strcmp);

  /* search for the key1: */
  pItem = (char*) bsearch (key1, strvalues, 4, 20, (int(*)(const void*,const void*)) strcmp);

  if (pItem!=NULL)
    printf ("%s is in the array.\n",pItem);
  else
    printf ("%s is not in the array.\n",key1);
    
  /* search for the key2: */
  pItem = (char*) bsearch (key2, strvalues, 4, 20, (int(*)(const void*,const void*)) strcmp);

  if (pItem!=NULL)
    printf ("%s is in the array.\n",pItem);
  else
    printf ("%s is not in the array.\n",key2);    
    
/*output
example is in the array.
test is not in the array.
*/
  return 0;
}