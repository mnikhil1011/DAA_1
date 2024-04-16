the Kirkpatrick-Seidel-Algorithm has a time complexity of O(n log m)  
the Jarvis March Algorithm has a time complexity of O(n m)  

where n is number of points and m is the number of points in the convex hull  

@tableofcontents

@section dataset-size
the Kirkpatrick-Seidel-Algorithm performs better for large datasets (as its timecomplexity is lesser), it is a very efficient algorithm
but it is very a complex algorithm to implement  

but for smaller datasets  
Kirkpatrick-Seidel-Algorithm might perform worse than the Jarvis March Algorithm because of its high value of constants  
the Kirkpatrick-Seidel-Algorithm has a very high value of constants, because we calculate the median using median of medians which in itself has a very large constant  
we seperately calculate the upper hull and lower hull 

we find the median of the points and then the median of the solpes and then partition the points into small large and equal and then repeat this process 

hence even though asymptotically it has a lesser time complexity than the Jarvis March Algorithm, the Jarvis March Algorithm performs better for smaller datasets  

@section number-of-points-on-the-hull

if the number of points on the hull is very less compared to the total number of points (n>>m)  
then the Jarvis March Algorithm might perform better in some cases because of its low value of constants  

@section Jarvis-March-Algorithm
it is also known as the Gift Wrapping Algorithm  
it is easy to impement  
very intuative algorithm  
performs good for smaller datasets  
can not parallelly solve the problem using this algorithm  
even though its time complexity is O(n*m) , on an average it is more efficient because of the point deletion optimisation step  

@section Kirkpatrick-Seidel-Algorithm
it is also known as the ultimate convex hull algorithm because of its efficiency  
it is a very efficient algorithm with a complexity of O(n*log m)  
it is tough to implement  
we can parallelly compute the subproblems, allowing us the solve the problem even quicker  



