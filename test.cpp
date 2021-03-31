#include "matrix.cpp"
#include <iostream>

using namespace std;

int main()
{
	Matrix a(3, 3);
	a.take_input();
	Matrix::transpose(a);
	
	a.print_matrix();
	
	Matrix::inverse(a);
	
	a.print_matrix();
	
	Matrix b(2, 3);
	b.take_input();
	/*
	a.mat[0][0] = 1;
	a.mat[0][1] = 2;
	
	b.mat[0][0] = 7;
	b.mat[0][1] = -8;
	b.mat[0][2] = 3.2;
	b.mat[1][0] = 98;
	b.mat[1][1] = 12;
	b.mat[1][2] = 33;
	*/
	
	a.print_matrix();
	b.print_matrix();
	
	Matrix c = Matrix::mult(a, b);
	
	c.print_matrix();
}