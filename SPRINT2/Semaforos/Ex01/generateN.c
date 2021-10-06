#include <stdio.h>
#include <stdlib.h>

int main()
{
   FILE *fptr;
   fptr = fopen("Numbers.txt","w");

   if(fptr == NULL)
   {
      printf("Error!");
      exit(1);
   }


   int i = 0;

   for(i = 0; i < 2000; i++){
     fprintf(fptr,"%d\n",i);
   }


   fclose(fptr);

   return 0;
}
