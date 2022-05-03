#include <iostream>
#include <cmath>
#include <string>

using namespace std;

double calc(int n, double val, double *coeff);							// Function that calculates a given function with given value

void secant(int n, double x0, double x1, double tol, double *coeff);	// Function for secant algorithm

void bisec(int n, double x0, double x1, double tol, double *coeff);		// Function for bisect algorithm

void hybrid(int n, double x0, double x1, double tol, double *coeff);	// Function for hybrid method

int main(int argc, char *argv[]){
	
	
	int n = argc-1;
	double *coeff = new double [n-3];
	
	for(int i=n-3; i>0; i--)					// An array for coefficients of the function
	{
		coeff[i-1] = stod(argv[n-2-i]);
	}
	
	
	double x0 = stod(argv[n-2]);				// x0 value
	double x1 = stod(argv[n-1]);				// x1 value
	double tol = stod(argv[n]);					// tolerance value
	
	bisec(n, x0, x1, tol, coeff);				// Bisection algorithm
	
	secant(n, x0, x1, tol, coeff);				// Secant algorithm
	
	hybrid(n, x0, x1, tol, coeff);				// Hybrid method
		
}


double calc(int n, double val, double *coeff) {
	
	double fun = 0.0;
	
	for (int i=0; i<n-3; i++){				//
		fun += coeff[i]*pow(val,i);			// Recalculating for next iteration
	}
	
	return fun;
}


void secant(int n, double x0, double x1, double tol, double *coeff) {
	
	int iter = 0;		// iteration number
	
	double xn = 0;		// initializing xn
	
	while(true){
		
	double num	= calc(n, x1, coeff) * (x1-x0);					// numerator of the next step calculation
	
	double denum = calc(n, x1, coeff) - calc(n, x0, coeff);		// numerator of the next step calculation
	
	xn = x1 - num/denum;	// next x
	
	iter++;					// increasing iteration
	
	if(abs(xn-x1) < tol){	// if toleracne is lower than the limit break
		break;
		
	}
	
	x0 = x1;				//
	x1 = xn;				// assignin values for next step
		
	}
	
	cout<<iter<<" number of iterations and root with secant method is: "<<xn<<endl;
	
}


void bisec(int n, double x0, double x1, double tol, double *coeff) {
	
	int iter = 0;			// iteration number
	
	double c = 0;			// initializing mid point
	
	while((x1-x0) > tol){	// while tolerance is greater than
		
		c = (x1+x0)/2;		// calcualting mid point
		
		iter++;				// increasing iteration
		
		double Fx0 = calc(n, x0, coeff);	// calculating the given func with calue of x0
		double Fx1 = calc(n, x1, coeff);	// calculating the given func with calue of x1
		double Fc = calc(n, c, coeff);		// calculating the given func with calue of c
		
		if((Fc < 0 && Fx0 > 0) || (Fc > 0 && Fx0 < 0) ){
			x1 = c;		// assigning new values accordingly
		} 
		else {
			x0 = c;		// assigning new values accordingly
		}
		
	}
	
	cout<<iter<<" number of iterations and root with bisection method is: "<<c<<endl;
}


void hybrid(int n, double x0, double x1, double tol, double *coeff) {
	
	int iter = 0;
	
	double c = 0;
	
	while((x1-x0) > tol){	// bisection method
		
		c = (x1+x0)/2;
		
		iter++;
		
		double Fx0 = calc(n, x0, coeff);
		double Fx1 = calc(n, x1, coeff);
		double Fc = calc(n, c, coeff);
		
		if((Fc < 0 && Fx0 > 0) || (Fc > 0 && Fx0 < 0) ){
			x1 = c;
		} 
		else {
			x0 = c;
		}
		
		if(iter == 2) {		// when the iteration number is 2 break
			break;
		}
		
	}
	
	while(true){			// rest is continued using secant method
			
		double num	= calc(n, x1, coeff) * (x1-x0);
		
		double denum = calc(n, x1, coeff) - calc(n, x0, coeff);
		
		c = x1 - num/denum;
		
		iter++;
		
		if(abs(c-x1) < tol){
			break;
			
		}
		
		x0 = x1;
		x1 = c;
			
	}
	
	cout<<iter<<" number of iterations and root with hybrid method is: "<<c<<endl;
	
}


