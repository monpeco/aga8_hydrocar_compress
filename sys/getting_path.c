#include <stdio.h>      /* printf */
#include <stdlib.h>     /* getenv */

int main ()
{
    char* pPath;
    pPath = getenv ("PATH");
    if (pPath!=NULL)
        printf ("The current path is: %s\n",pPath);  
    
    pPath = getenv ("USER");
    if (pPath!=NULL)
        printf ("The current user is: %s\n",pPath);
    
    pPath = getenv ("APACHE_PID_FILE");
    if (pPath!=NULL)
        printf ("The current apache pid file is: %s\n",pPath);
        
    return 0;
}