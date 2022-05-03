#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

void print_matrix(int n, double **arr); 							// Function that prints the given matrix

void print_vector(int n, double **arr); 							// Function that prints the given vector

int row_number(ifstream& file_name); 								// Function that returns the number of rows from the given txt file (Used to find the rows of A)

void get_matrix(ifstream& file_name, int n, double **arr);			// Function that creates a 2D array from a given text file

void get_vector(ifstream& file_name, int n, double **arr);			// Function that creates 1D array from a given text file

bool is_singular(int n, double **A);								// Function that decets the singularity of matrix A

void row_swap(int n, double **A, double **b, int row1, int row2);	// Function that interchanges the rows of a given matrix, row1 & row2 starts from 1, not 0 ie they are not array index

int find_max(int n, double **arr, int col);							// Iterates over rows to find the row number with maximum absolute value for given column

void upper_triangular(int n, double **A, double **b);				// Function that makes the given matrix an upper triangular

void back_subs(int n, double **A, double **b, double **x);			// Function that finds solution (x) with back substitution

void write_x(int n, double **x);									// Function that generates the x.txt file

double norm_1(int n, double **A);									// Function thah calculates the 1 norm of matrix

double norm_inf(int n, double **A);									// Function thah calculates the infinity norm of matrix

void cond(int n, double **A);										// Function that finds the condition numbers of 1 and infinity of given matrix A

int main(int argc, char *argv[]){
	
	if(argc != 3)	// If the passed arguments are not 3, (name of the file, A matrix, b vector), print out an error message and terminate
	{
		cout<<"Error! Invalid number of arguments."<<endl;
		return 0;
	}
	
	string File_A = argv[1], File_b = argv[2]; 		// Filenames of A & b matrcies
	
	File_A+=".txt"; 								// Adding .txt extention to the files					
	File_b+=".txt";									// Adding .txt extention to the files

	ifstream A_text (File_A);						// Opening the files
	ifstream b_text (File_b);  						// Opening the files
	
	int n = row_number(A_text);						// Getting the dimension of A from its row number
	
	double **matrix_A;								// Creating the A matrix dynamically
	matrix_A = new double *[n];
	for(int i=0; i<n; i++)
	{
		matrix_A[i] = new double [n];
	}
	
	double **matrix_b;								// Creating the b vector dynamically
	matrix_b = new double *[n];
	for(int i=0; i<n; i++)
	{
		matrix_b[i] = new double [1];
	}
	
	double **matrix_x;								// Creating the x vector dynamically
	matrix_x = new double *[n];
	for(int i=0; i<n; i++)
	{
		matrix_x[i] = new double [1];
	}

	get_vector(b_text, n, matrix_b);				// Generating the b vector from the input file
	//print_vector(n, matrix_b);

	get_matrix(A_text, n, matrix_A);				// Generating the A matrix from the input file
	//print_matrix(n, matrix_A);
	
	bool singular;
	
	singular = is_singular(n, matrix_A);			// Checking whether or not matrix A is singular
	
	if(singular == true)							// If it is singular print out an error message and terminate
	{
		cout<<"Error! Matrix is singular"<<endl;
		return 0;
	}
	
	if(n==2)										// If the dimension of given matrix A is 2x2, ie n is equal to 2
	{
		cond(n, matrix_A);							// Find the condition numbers of 1 and infinity of A
	}
	
	upper_triangular(n, matrix_A, matrix_b);		// Make A upper triangular with partial pivotting

	//print_matrix(n, matrix_A);
	//print_vector(n, matrix_b);
	
	back_subs(n, matrix_A, matrix_b, matrix_x);		// Back substuting to find the solution, ie x vector
	cout<<"vector x is: "<<endl;
	print_vector(n, matrix_x);
	
	write_x(n, matrix_x);							// Write the elements of x vector to x.txt
	
}

// Functions

void print_matrix(int n, double **arr) 	// Function that prints the given matrix
{
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){
			cout<<arr[i][j]<<" ";
			
			if(j==n-1){cout<<endl;}
		}	
	}
	
	cout<<endl;
}

void print_vector(int n, double **arr) 	// Function that prints the given vector
{
	for (int i=0; i<n; i++){
		cout<<arr[i][0]<<endl;
	}	
	cout<<endl;
}

int row_number(ifstream& file_name) 	// Function that returns the number of rows that given file has (in this case its n)
{
	string line;
	int n=0;	
	while (getline(file_name, line))	// While there is a non-empty line count
	{		
		if(line!="")
			n++;			
	}    
    return n; 							// Returns the number of rows that A has
}

void get_matrix(ifstream& file_name, int n, double **arr)
{
	file_name.clear();					//
	file_name.seekg(0);					// Resetting back to top of the file
	
	for (int row=0; row<n; row++)
	{
		for (int col=0; col<n; col++)
		{
			file_name >> arr[row][col];	// Reading matrix from txt file
		}	
	}
	
}

void get_vector(ifstream& file_name, int n, double **arr)
{
	file_name.clear();					//
	file_name.seekg(0);					// Resetting back to top of the file
	
	for (int row=0; row<n; row++)
	{
		file_name >> arr[row][0];		// Reading vector from txt file
		
	}
}

bool is_singular(int n, double **A)		// Function that detects wheter a matrix is singular or not
{
	int pivot_num = n;
	double lower_limit = 1e-15;			// Lower limit regarding machine precision

	for(int i=0; i<n; i++)
	{
		if(abs(A[i][i])<lower_limit)	// If the element on the pivot position is detected to be zero, ie. closer to zero than the lower limit of precision, or actually zero
		{
			pivot_num--;				// Counting the number of valid pivots
		}
	}

	if(pivot_num == n)
	{
		return false;					// Returns false if matrix has n pivots, ie. non singular
	}

	else
	{
		return true;					// Returns true if matrix has less than n pivots, ie. singular	
	}
}

void row_swap(int n, double **A, double **b, int row1, int row2)
{
	for(int i=0; i<n; i++)				// Swapping the row1 and row2 of matrix A, these numbers are not array index, ie they start from 1 not 0,
	{									// If row1=2 & row2=4 that means swap the second and fourth row of the matrix
		double temp_A=A[row1-1][i];
		A[row1-1][i]=A[row2-1][i];
		A[row2-1][i]=temp_A;
	}
	
	double temp_b=b[row1-1][0];			// Swapping the same rows of vector b
	b[row1-1][0]=b[row2-1][0];
	b[row2-1][0]=temp_b;
}

int find_max(int n, double **arr, int col)		// Function that finds and returns the number of the row with maximum absolute value for given column
{
	double max = 0;
	double max_row = 0;
	for(int row=col; row<n; row++)				// Iterates over the rows to find the row number with maximum absolute value for given column
	{
		if(abs(arr[row][col])>max)
		{
			max = abs(arr[row][col]);
			max_row = row;
		}
	}
	return max_row;								// Returns the row number
}

void upper_triangular(int n, double **A, double **b)		// Function that makes the given matrix an upper triangular
{	
	for(int pivot=0; pivot<n-1; pivot++)
	{
		int pivot_row = find_max(n, A, pivot);				// Finding the largest absolute valued row from the column
		row_swap(n, A, b, pivot+1, pivot_row+1);			// Swapping the current row with the max abs valued row which found above
		
		for(int row=pivot+1; row<n; row++)
		{
			double coeff = A[row][pivot]/A[pivot][pivot];	// Ratio of the every element under the pivot to the pivot
			
			for(int element=pivot; element<n; element++)
			{
				// Subtracting every element of the pivot row multiplied by the element under the pivot for every row under the pivot
				A[row][element] -= A[pivot][element]*coeff;	
			}
			
			b[row][0] -= b[pivot][0]*coeff;
		}
	}
}

void back_subs(int n, double **A, double **b, double **x)
{
	x[n-1][0] = b[n-1][0]/A[n-1][n-1];	// Finding the last component of x
	
	for(int row=n-2; row>=0; row--)		// Back substuting to find the other elements of x
	{
		double subtract = 0;
		for(int col=n-1; col>=row; col--)
		{
			subtract += A[row][col]*x[col][0];
		}
		
		x[row][0] = (b[row][0]-subtract)/A[row][row];
	}
}

void write_x(int n, double **x)		// Function that writes the values of x to a txt file
{
	ofstream x_file;
	x_file.open ("x.txt");
	
	string str = "";
	
	for(int i=0; i<n; i++)
	{
		double number = x[i][0];
		str += to_string(number) + '\n';
	}
	
	x_file << str;				// Write the values
	
	x_file.close();				// Close the file
}

double norm_1(int n, double **A)
{
	double sum1 = abs(A[0][0])+abs(A[1][0]);	// Sum of column 1
	double sum2 = abs(A[0][1])+abs(A[1][1]);	// Sum of column 2
	
	if(sum1>sum2) return sum1;
	
	else return sum2;							// Returns the 1 norm
}

double norm_inf(int n, double **A)
{
	double sum1 = abs(A[0][0])+abs(A[0][1]);	// Sum of row 1
	double sum2 = abs(A[1][0])+abs(A[1][1]);	// Sum of row 2
	
	if(sum1>sum2) return sum1;
	
	else return sum2;							// Returns the infinity norm
}

void cond(int n, double **A)
{
	// inverse of 2x2 matrix is found by intercahnging diagonal elements, 
	// adding a minus sign to off-diagonals and multiplying whole matrix with recipical of its determinant (1/det(A)).
	//
	//
	//cond_1(A) = nomr_1(A) * norm_1(A_inverse), similarly cond_inf(A) = norm_inf(A) * norm_inf(A_inverse)

	double **temp;					// Creating a temporary matrix in order not change the values of original matrix
	temp = new double *[n];
	
	for(int i=0; i<n; i++)
	{
		temp[i] = new double [n];
	}
	
	for(int i=0; i<n; i++)				// Copying values of A to temporary matrix
	{
		for(int j=0; j<n; j++)
		{
			temp[i][j] = A[i][j];
		}
	}
	
	double det = temp[0][0]*temp[1][1] - temp[0][1]*temp[1][0];	// Determinant of A
	
	temp[0][0] = A[1][1];		//
	temp[1][1] = A[0][0];		// Intercahnging diagonal elements
	
	temp[0][1] *= -1;			//
	temp[1][0] *= -1;			// Adding a minus sign to off-diagonals
	
	
	for(int i=0; i<n; i++)		// Muliplying with 1/det(A)
	{
		for(int j=0; j<n; j++)
		{
			temp[i][j] /= det;
		}
	}							// temp is the inverse of A now
	
	double cond_1 = norm_1(n, A)*norm_1(n, temp);			// Calculating cond_1(A)
	double cond_inf = norm_inf(n, A)*norm_inf(n, temp);		// Calculating cond_inf(A)
	
	cout<<"Condition number of 1 is: "<<cond_1<<endl;				//
	cout<<"Condition number of infinity is: "<<cond_inf<<endl;		// Printing out the condition numbers
}
