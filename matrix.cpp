#include <vector>
#include <iostream>

using namespace std;

class Matrix{

public:

	int m, n;
	vector<vector<double>> mat;

	Matrix(int m, int n)
	{
		this->m = m;
		this->n = n;
		mat.resize(m, vector<double>(n, 0));
	}

	Matrix()
	{
		this->m = 0;
		this->n = 0;
		mat.resize(0, vector<double>(0));
	}

	void take_input()
	{
		for(int i=0; i<m; ++i)
		{
			cout << "Enter row " << i << ": ";
			for(int j=0; j<n; ++j)
			{
				cin >> mat[i][j];
			}
		}

		cout << endl;
	}
	void set_identity(){
		for(int i = 0; i<this->m ; i++){
			for(int j =0 ; j<this->n; j++){
				if(i == j)
					this->mat[i][j] = 1;
			else
				 this->mat[i][j] = 0;
			 }
		}
	}

	static Matrix mult(Matrix m1, Matrix m2){
		if(m1.n != m2.m)
		{
			cout << "Invalid input!" << endl;
			return Matrix(0, 0);
		}

		Matrix prod (m1.m, m2.n);
		for(int i = 0 ; i<m1.m; i++){
			for(int j = 0 ; j<m2.n ; j++){
				for(int k = 0 ; k<m1.n ; k++){
					prod.mat[i][j] += m1.mat[i][k]*m2.mat[k][j];
				}
			}
		}
		return prod;
	}

	void print_matrix()
	{
		cout << endl;
		for(int i=0; i<m; ++i)
		{
			cout << "[ ";
			for(int j=0; j<n; ++j)
			{
				cout << mat[i][j] << " ";
			}

			cout << "]" << endl;
		}
		cout << endl;
	}

 Matrix transpose(){
		Matrix mt(this->n,this->m);
		for(int i = 0 ; i<mt.m ; i++){
			for(int j = 0 ; j<mt.n ; j++){
				mt.mat[j][i] = this->mat[i][j];
			}
		}
		return mt;
	}

	static void getCofactor(Matrix & m1, Matrix & temp, int p, int q, int s){
		int i = 0, j = 0;

		// Looping for each element of the matrix
		for (int row = 0; row < s; row++){

			for (int col = 0; col < s; col++){
				//  Copying into temporary matrix only those element
				//  which are not in the given row and column
				if (row != p && col != q){
					temp.mat[i][j++] = m1.mat[row][col];

					// Row is filled, so increase row index and
					// reset col index
					if (j == s - 1){
						j = 0;
						i++;
					}
				}
			}
		}
	}

	/* Recursive function for finding determinant of matrix.
	s is current dimension of m1 */
	static int dtm(Matrix & m1 ,  int s)
	{
		if(m1.m != m1.n){
			cout << " non-square matrix";
			return -1;
		}
		int D = 0; // Initialize result

		//  Base case : if matrix contains single element
		if (s == 1)
		return m1.mat[0][0];

		Matrix temp(m1.n , m1.n); // To store cofactors

		int sign = 1;  // To store sign multiplier

		// Iterate for each element of first row
		for (int f = 0; f < s; f++)
		{
			// Getting Cofactor of A[0][f]
			getCofactor(m1, temp, 0, f, s);
			D += sign * m1.mat[0][f] * dtm(temp, s - 1);

			// terms are to be added with alternate sign
			sign = -sign;
		}

		return D;
	}

	// Function to get adjoint of m1 in adj
	static void adjoint(Matrix & m1,Matrix & adj ){

		if (m1.n == 1){
			adj.mat[0][0] = 1;
			return;
		}

		// temp is used to store cofactors of m1
		int sign = 1;
		Matrix temp(m1.m, m1.n);
		for (int i=0; i<m1.m; i++)
		{
			for (int j=0; j<m1.n; j++)
			{
				// Get cofactor of m1.mat[i][j]
				getCofactor(m1, temp, i, j, m1.n);

				// sign of adj.mat[j][i] positive if sum of row
				// and column indexes is even.
				sign = ((i+j)%2==0)? 1: -1;

				// Interchanging rows and columns to get the
				// transpose of the cofactor matrix
				adj.mat[j][i] = (sign)*(dtm(temp, m1.n - 1));
			}
		}
	}


	Matrix inverse()
	{

		if(this->m != this->n){
			cout << "Non-square matrix!" << endl;
			Matrix err;
			return err;
		}

		int det = dtm(*this,this->n);
		if (det == 0){
			cout << "Singular matrix!" << endl;
			Matrix err;
			return err;
		}
		// Find adjoint
		Matrix adj(this->m , this->n);
		adjoint(*this,adj);
		Matrix inverse(this->n , this->m);

		for (int i=0; i<this->n ; i++)
			for (int j=0; j<this->n; j++)
				inverse.mat[i][j] = adj.mat[i][j]/double(det);

		return inverse;
	}

	Matrix operator + (Matrix a){
		Matrix res(a.m,a.n);
		if(a.m != this->m || a.n != this->n){
			cout << " Invalid operation !";
			return res;
		}
		for(int i = 0; i<a.m ; i++){
			for(int j = 0; j<a.n ; j++){
				res.mat[i][j] = this->mat[i][j] + a.mat[i][j];
			}
		}
		return res;
	}
	Matrix operator - (Matrix a){
		Matrix res(a.m,a.n);
		if(a.m != this->m || a.n != this->n){
			cout << " Invalid operation !";
			return res;
		}
		for(int i = 0; i<a.m ; i++){
			for(int j = 0; j<a.n ; j++){
				res.mat[i][j] = this->mat[i][j] - a.mat[i][j];
			}
		}
		return res;
	}
};
