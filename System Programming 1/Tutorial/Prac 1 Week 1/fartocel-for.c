 # include <stdio.h>
 /*print Fahrenheit-Celsius table for fah=0,20..,220*/

 main()
 {
   int fah, cel;
   int lower, upper, step;
   lower=0;  /*lower limit of the temperature table */
   upper=220;  /* upper limit */
   step=20;  /*step size */
   for (fah=lower;fah<=upper; fah=fah+step)
   {
     cel=5*(fah-32)/9;
     printf("%d\t%d\n", fah, cel);
   }
 }
