int sum(int n)
{ 
   if(n==0) return n; 
   else return n+sum(n-1); /*call to itself */ 
}

main()
{ 
  int num,add; 
  printf("Enter a positive integer:\n");
  scanf("%d",&num); 
  
  add=sum(num); 
  printf("sum=%d\n",add); 
}

