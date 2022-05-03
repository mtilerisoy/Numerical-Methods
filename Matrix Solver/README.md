This program solves the equation of the form Ax=b where A is nxn matrix and b is nx1 vector. A and b are given 
as text files, name of these files must be past through comand-line respectively. These files must be in the 
same location as the program is. Name of the files must be written EXCLUDING the 'txt' extension. 
Program prints out the solution x and also writes the solution vector as another text file named x.txt
to the same location. In addition if A is 2x2 matrix, program calculates and prints out the condition numbers 
of 1 and infinity of matrix A.

EXAMPLE IN THE PDF

A=|1.000, 1.000|
  |1.000, 1.001|

cond_1 = 4004
cond_inf = 4004

for b1=[2.000,2.000] : x=[2,0]

for b2=[2.000,2.001] : x=[1,1]

Condition numbers are very large for this A matrix so it is an ill-conditioned matrix. Thus a very small 
change in matrix b has relatively large impact on solution vector. 

