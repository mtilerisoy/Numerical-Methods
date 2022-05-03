#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

void print_matrix(int n, double **arr); 							// Function that prints the given matrix
void print_vector(int n, double **arr); 							// Function that prints the given vector
int row_number(ifstream& file_name); 								// Function that returns the number of rows from the given txt file (Used to find the rows of A)
void get_matrix(ifstream& file_name, int n, double **arr);			// Function that creates a 2D array from a given text file

void mult(int n, double **A, double **x, double** next_x);			// Function that multiplies a matrix and a vector
double abs_max(int n, double **vec);								// Function that finds the maximum of absolute values of elements of given vector (ie infinity norm)
double max_index(int n, double **vec);								// Function that finds the index of the max abs element
void abs(int n, double** vec, double** res);						// Funciton that assigns the given vector as its elements absolute valued to some vector (res)
void sub(int n, double** vec1, double** vec2, double** res);		// Function that subtracts given two vectors and assigns the result to some vector (res)
void sub_matrix(int n, double** A, double** temp3, double** B);		// Function that subtracts given two matirices and assigns the result to some other matrix (B)
void write(int n, double* x, double l1, double l2, string name);	// Function that writes the output file

int main(int argc, char *argv[]){
	
	
	if(argc != 4)	// If the passed arguments are not 3, (name of the file, A matrix, b vector), print out an error message and terminate
	{
		cout<<"Error! Invalid number of arguments."<<endl;
		return 0;
	}
	
	
	double tol = stod(argv[2]);//1e-6; 
	
	string File_A = argv[1];
	
	//string File_A = "";	 					 		// Filename of A matrix
	//cout<<"Enter name of the matrix A: ";
	//cin>>File_A;
	
	File_A+=".txt"; 								// Adding .txt extention to the files					
	ifstream A_text (File_A);						// Opening the files
	
	int n = row_number(A_text);						// Getting the dimension of A from its row number
	
	double **A;										// Creating the A matrix dynamically
	A = new double *[n];
	for(int i=0; i<n; i++)
	{
		A[i] = new double [n];
	}
	
	get_matrix(A_text, n, A);						// Generating the A matrix from the input file
	
	double **temp3;									// Creating a temporary matrix dynamically
	temp3 = new double *[n];
	for(int i=0; i<n; i++)
	{
		temp3[i] = new double [n];
	}
	
	double **B;										// Creating a temporary matrix dynamically
	B = new double *[n];
	for(int i=0; i<n; i++)
	{
		B[i] = new double [n];
	}
	
	double **x;										// Creating the x vector dynamically
	x = new double *[n];
	for(int i=0; i<n; i++)
	{
		x[i] = new double [1];
	}
	
	for(int i=0; i<n; i++){
		x[i][0] = 1;
	}
	
	double **next_x;								// Creating the next_x vector dynamically
	next_x = new double *[n];
	for(int i=0; i<n; i++)
	{
		next_x[i] = new double [1];
	}
	
	for(int i=0; i<n; i++){
		next_x[i][0] = 0;
	}

	double **temp1;									// Creating a temporary vector dynamically
	temp1 = new double *[n];
	for(int i=0; i<n; i++)
	{
		temp1[i] = new double [1];
	}
	for(int i=0; i<n; i++){
		temp1[i][0] = 0;
	}
	
	double **temp2;									// Creating a temporary vector dynamically
	temp2 = new double *[n];
	for(int i=0; i<n; i++)
	{
		temp2[i] = new double [1];
	}
	for(int i=0; i<n; i++){
		temp2[i][0] = 0;
	}
	
	double *eigenvec;									// Creating a temporary vector dynamically
	eigenvec = new double [n];
	for(int i=0; i<n; i++)
	{
		eigenvec[i] = 0;
	}
	
	//Normalized Power Iteration
	double dx = 1.0;
	while (dx > tol){
		
		mult(n, A, x, next_x);							// Finding next x vector

		double norm_inf = abs_max(n, next_x);			// Finding the norm of next x vector

		for(int i=0; i<n; i++){
			next_x[i][0] = next_x[i][0]/norm_inf;		// Normalizing the new x vector
		}
		
		abs(n, x, temp1);								// Abslute value of x
		abs(n, next_x, temp2);							// Absolute value of next_x
	
		sub(n, temp2, temp1, temp2);					// Subtracting x from next_x and returning as temp2
	
		dx = abs_max(n, temp2);							// Calculating delx
		
		for(int i=0; i<n; i++){							// Assigning teh values of next_x to the previous x
			x[i][0] = next_x[i][0];
		}
		/*
		for(int i=0; i<n; i++){
			next_x[i][0] = 0;
		}
		*/
	}
	
	mult(n, A, x, next_x);

	int k = max_index(n, x);							// index of max absolute valued element
	double lambda1 = next_x[k][0]/x[k][0];
	cout<<"lambda1: "<<lambda1<<endl;
	
	cout<<"eigenvector 1 is: "<<endl;
	print_vector(n, x);
	
	for(int i=0; i<n; i++){
		eigenvec[i] = x[i][0];
	}
	
	
	//Deflation
	
	for(int row=0; row<n; row++){
		for(int col=0; col<n; col++){
			temp3[row][col] = x[row][0] * A[k][col];
		}
	}
	
	sub_matrix(n, A, temp3, B);
	
	dx=1;
	while(dx>tol){
		
		mult(n, B, x, next_x);							// Finding next x vector
		
		double norm_inf = abs_max(n, next_x);			// Finding the norm of next x vector

		for(int i=0; i<n; i++){
			next_x[i][0] = next_x[i][0]/norm_inf;		// Normalizing the new x vector
		}
		
		abs(n, x, temp1);								// Abslute value of x
		abs(n, next_x, temp2);							// Absolute value of next_x
	
		sub(n, temp2, temp1, temp2);					// Subtracting x from next_x and returning as temp2
	
		dx = abs_max(n, temp2);							// Calculating delx
		
		for(int i=0; i<n; i++){							// Assigning teh values of next_x to the previous x
			x[i][0] = next_x[i][0];
		}
		
		/*
		for(int i=0; i<n; i++){							// Clearing the next_x vector
			next_x[i][0] = 0;
		}
		*/
		
	}
	
	mult(n, B, x, next_x);
	
	k = max_index(n, x);								// index of max absolute valued element
	double lambda2 = next_x[k][0]/x[k][0];
	cout<<"lambda2: "<<lambda2<<endl;
	
	write(n, eigenvec, lambda1, lambda2, argv[3]);
	
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

void mult(int n, double **A, double **x, double** res){	// Function that multiplies given matrix and vector A*x
	
	for(int i=0; i<n; i++){
		res[i][0] = 0;
	}
	
	for(int row=0; row<n; row++){
		for(int col=0; col<n; col++){
			res[row][0] += A[row][col]*x[col][0];
		}
	}
}

double abs_max(int n, double **vec)				// Function that finds and returns the value of the row with maximum absolute value for given vector
{
	double max = 0.0;
	
	for(int i=0; i<n; i++){
		if(abs(vec[i][0]) > max){
			max = abs(vec[i][0]);
		}
	}
	
	return max;									// Returns the max
}

double max_index(int n, double **vec)			// Function that finds and returns the number of the row with maximum absolute value for given column
{
	double max = 0.0;
	int k = 0;
	
	for(int i=0; i<n; i++){
		if(abs(vec[i][0]) > max){
			max = abs(vec[i][0]);
			k=i;
		}
	}
	
	return k;									// Returns the index of maxium 
}

void abs(int n, double** vec, double** res){	// Function that gives the absolute values of given matrix
	
	for(int i=0; i<n; i++){
		res[i][0] = abs(vec[i][0]);
	}
}

void sub(int n, double** vec1, double** vec2, double** res){	// Function that subtracts given two vector and assigns the result to a different empty vector
	
	double temp[n][1];
	
	for(int i=0; i<n; i++){
		temp[i][0] = 0;
	}
	
	for(int i=0; i<n; i++){
		temp[i][0] = vec1[i][0] - vec2[i][0];
	}
	
	for(int i=0; i<n; i++){
		res[i][0] = temp[i][0];
	}
	
}

void sub_matrix(int n, double** A, double** temp3, double** B){	// Function that subtracts given two matricies and assigns the result to a different empty matrix
	
	for(int row=0; row<n; row++){
		for(int col=0; col<n; col++){
			B[row][col] = A[row][col] - temp3[row][col];
		}
	}
	
}


void write(int n, double* x, double l1, double l2, string name)		// Function that writes the output file
{
	name += ".txt";
	
	ofstream x_file;
	x_file.open (name);
	
	string str = "Eigenvalue #1 = ";
	
	str += to_string(l1) + '\n';
	
	for(int i=0; i<n; i++)
	{
		double number = x[i];
		str += to_string(number) + '\n';
	}
	
	str += "Eigenvalue #2 = ";
	str += to_string(l2) + '\n';
	
	
	
	x_file << str;				// Write the values
	
	x_file.close();				// Close the file
}

