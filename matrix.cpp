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
	mat.resize(m, vector<double>(n));
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

static Matrix mult(Matrix & m1, Matrix & m2){
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
};
