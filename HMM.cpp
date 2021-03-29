#include "matrix.cpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

class HMM
{
public:

Matrix * initial_matrix;
Matrix * transition_matrix;
Matrix * confusion_matrix;

int num_hidden_states;
int num_observable_states;

unordered_map <string, int> hidden_states;
unordered_map <string, int> observable_states;

vector <string> hidden_states_reverse;

HMM (int num_hidden_states, int num_observable_states)
{
	this->num_hidden_states = num_hidden_states;
	this->num_observable_states = num_observable_states;
	
	initial_matrix = new Matrix(1, num_hidden_states);
	transition_matrix = new Matrix(num_hidden_states, num_hidden_states);
	confusion_matrix = new Matrix(num_hidden_states, num_observable_states);
	
	cout << endl << "Enter the names of the hidden states." << endl;
	
	hidden_states_reverse.resize(num_hidden_states);
	
	for(int i=0; i<num_hidden_states; ++i)
	{
		string s;
		cin >> s;
		hidden_states[s] = i;
		hidden_states_reverse[i] = s;
	}
	
	cout << endl << "Enter the names of the observable states." << endl;
	
	for(int i=0; i<num_observable_states; ++i)
	{
		string s;
		cin >> s;
		observable_states[s] = i;
	}
	
	cout << endl << "Enter probabilities for initial matrix." << endl;

    initial_matrix->take_input();

	cout << endl << "Enter probabilities for transition matrix." << endl;

	transition_matrix->take_input();
	
	cout << endl << "Enter probabilities for confusion matrix." << endl;
	
	confusion_matrix->take_input();
}

double forward()
{
	int T;
	
	cout << endl << "Enter the number of observations: ";
	cin >> T;
	cout << endl << "Enter the observations: ";
	
	vector <int> observations(T);
	
	for (int i=0; i<T; ++i)
	{
		string s;
		cin >> s;
		
		unordered_map <string, int> :: iterator itr;
		
		if((itr=observable_states.find(s))==observable_states.end())
		{
			cout << "Invalid state entered!" << endl;
			return 0;
		}
		
		observations[i] = itr->second;
	}
	
	return forward_proc (observations, -1);
}

double forward_proc (vector <int> & observations, int initial)
{
	
	int T = observations.size();
	
	vector <vector <double>> a (2, vector<double>(num_hidden_states, 0));
		
	if(initial!=-1)
	{
		a[0][initial] = confusion_matrix->mat[initial][observations[0]];
	}
	
	else
	{
		for (int j=0; j<num_hidden_states; ++j)
		{
			a[0][j] = initial_matrix->mat[0][j] * confusion_matrix->mat[j][observations[0]];
		}
	}
		
	for(int t=1; t<T; ++t)
	{
		fill(a[1].begin(), a[1].end(), 0);
		for (int i=0; i<num_hidden_states; ++i)
		{
			for(int j=0; j<num_hidden_states; ++j)
			{
				a[1][j] += a[0][i] * transition_matrix->mat[i][j] * confusion_matrix->mat[j][observations[t]];
			}
		}
		a[0] = a[1];
	}
	
	double prob = 0;
	
	for(int j=0; j<num_hidden_states; ++j)
		prob += a[0][j];
		
	return prob;
}

vector <string> Viterbi ()
{
	int T;
	
	cout << endl << "Enter the number of observations: ";
	cin >> T;
	cout << endl << "Enter the observations: ";
	
	vector <int> observations(T);
	
	vector <string> most_probable_states;
	
	for (int i=0; i<T; ++i)
	{
		string s;
		cin >> s;
		
		unordered_map <string, int> :: iterator itr;
		
		if((itr=observable_states.find(s))==observable_states.end())
		{
			cout << "Invalid state entered!" << endl;
			return most_probable_states;
		}
		
		observations[i] = itr->second;
	}
	
	most_probable_states.resize(T);
	
	vector <vector<int>> states (T, vector<int>(num_hidden_states));
	
	vector <vector <double>> a (2, vector<double>(num_hidden_states));
	
	for(int j=0; j<num_hidden_states; ++j)
	{
		a[0][j] = initial_matrix->mat[0][j] * confusion_matrix->mat[j][observations[0]];
	}
	
	for(int t=1; t<T; ++t)
	{
		fill(a[1].begin(), a[1].end(), 0);
		
		for (int j=0; j<num_hidden_states; ++j)
		{
			for(int i=0; i<num_hidden_states; ++i)
			{
				double x = a[0][i] * transition_matrix->mat[i][j] * confusion_matrix->mat[j][observations[t]];
				if(x > a[1][j])
				{
					a[1][j] = x;
					states[t][j] = i;
				}
			}
		}
		
		a[0] = a[1];
	}
	
	double x = 0;
	int choose;
	
	for(int j=0; j<num_hidden_states; ++j)
	{
		if(a[0][j] > x)
		{
			x = a[0][j];
			most_probable_states[T-1] = hidden_states_reverse[j];
			choose = j;
		}
	}
	
	for(int t=T-1; t>0; --t)
	{
		choose = states[t][choose];
		most_probable_states[t-1] = hidden_states_reverse[choose];
	}
	
	return most_probable_states;
}

vector <vector<double>> forward_helper (int T, vector <int> & observations)
{
	
	vector <vector <double>> a (T, vector<double>(num_hidden_states));
		
	for (int j=0; j<num_hidden_states; ++j)
	{
		a[0][j] = initial_matrix->mat[0][j] * confusion_matrix->mat[j][observations[0]];
	}
		
	for(int t=1; t<T; ++t)
	{
		fill(a[t].begin(), a[t].end(), 0);
		
		for (int i=0; i<num_hidden_states; ++i)
		{
			for(int j=0; j<num_hidden_states; ++j)
			{
				a[t][j] += a[t-1][i] * transition_matrix->mat[i][j] * confusion_matrix->mat[j][observations[t]];
			}
		}
	}
			
	return a;
}

vector <vector<double>> backward_helper (int T, vector <int> & observations)
{
	vector <vector <double>> b (T, vector<double>(num_hidden_states));
	
	fill(b[T-1].begin(), b[T-1].end(), 1);
	
	for(int t=T-2; t>=0; --t)
	{
		fill(b[t].begin(), b[t].end(), 0);
		
		for(int i=0; i<num_hidden_states; ++i)
		{
			for(int j=0; j<num_hidden_states; ++j)
			{
				b[t][i] +=  transition_matrix->mat[i][j] * confusion_matrix->mat[j][observations[t+1]] * b[t+1][j];
			}
		}
	}
	
	return b;
}

void backward()
{
	int T;
	
	cout << endl << "Enter the number of observations: ";
	cin >> T;
	cout << endl << "Enter the observations: ";
	
	vector <int> observations(T);
	
	for (int i=0; i<T; ++i)
	{
		string s;
		cin >> s;
		
		unordered_map <string, int> :: iterator itr;
		
		if((itr=observable_states.find(s))==observable_states.end())
		{
			cout << "Invalid state entered!" << endl;
			return;
		}
		
		observations[i] = itr->second;
	}
	
	vector <vector <double>> gamma(T, vector<double>(num_hidden_states));
	vector <vector<vector<double>>> xi (T-1, vector<vector<double>> (num_hidden_states, vector<double> (num_hidden_states, 0)));
	
	vector <double> prob_given_param (T, 0);
	
	for(int num_iterations=1; num_iterations<=10000; ++num_iterations)
	{
		vector <vector<double>> a = forward_helper(T, observations);
		vector <vector<double>> b = backward_helper(T, observations);
		
		for(int t=0; t<T; ++t)
		{
			for(int j=0; j<num_hidden_states; ++j)
			{
				prob_given_param[t] += a[t][j] * b[t][j];
			}
		}
		
		for(int t=0; t<T; ++t)
		{
			for(int i=0; i<num_hidden_states; ++i)
			{
				gamma[t][i] = a[t][i] * b[t][i] / prob_given_param[t];
			}
		}
		
		for(int t=0; t<T-1; ++t)
		{
			for(int i=0; i<num_hidden_states; ++i)
			{
				for(int j=0; j<num_hidden_states; ++j)
				{
					xi[t][i][j] = a[t][i] * transition_matrix->mat[i][j] * confusion_matrix->mat[j][observations[t+1]] * b[t+1][j] / prob_given_param[t];
				}
			}
		}
		
		for(int i=0; i<num_hidden_states; ++i)
		{
			for(int j=0; j<num_hidden_states; ++j)
			{
				double sum1 = 0, sum2 = 0;
				
				for(int t=0; t<T-1; ++t)
				{
					sum1 += xi[t][i][j];
					sum2 += gamma[t][i];
				}
				
				transition_matrix->mat[i][j] = sum1/sum2;
			}
		}
		
		for(int j=0; j<num_hidden_states; ++j)
		{
			double sum2 = 0;
			
			for(int t=0; t<T; ++t)
			{
				sum2 += gamma[t][j];
			}
			
			for(int k=0; k<num_observable_states; ++k)
			{
				double sum1 = 0;
				
				for(int t=0; t<T; ++t)
				{
					if(k==observations[t])
						sum1 += gamma[t][j];
				}
				
				confusion_matrix->mat[j][k] = sum1/sum2;
			}
		}
		
		double sum = 0;
		for(int j=0; j<num_hidden_states; ++j)
		{
			sum += (initial_matrix->mat[0][j] = forward_proc(observations, j));
		}
		
		for(int j=0; j<num_hidden_states; ++j)
			initial_matrix->mat[0][j]/=sum;
		
	}
	
	cout << endl << "Updated initial matrix: " << endl;
	initial_matrix->print_matrix();
	
	cout << "Updated transition matrix: " << endl;
	transition_matrix->print_matrix();
	
	cout << "Updated confusion_matrix: " << endl;
	confusion_matrix->print_matrix();
}

};

int main()
{
	HMM moo (2, 2);
	
	while(1)
	{
		moo.backward();
	}
}


