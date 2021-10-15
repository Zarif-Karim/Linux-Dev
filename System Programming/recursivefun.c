#include <stdio.h>

void RecursiveFun() 
   { 
       printf("This is a recursive function\n");	
       RecursiveFun(); //Function calls itself
   } 

    main() 
   { 
       RecursiveFun(); 
   } 

