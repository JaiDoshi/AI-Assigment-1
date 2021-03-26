#include "matrix.cpp"
#include <iostream>
#include <string>
#include <unordered_map>

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

HMM (int num_hidden_states, int num_observable_states)
{
	this->num_hidden_states = num_hidden_states;
	this->num_observable_states = num_observable_states;
	
	initial_matrix = new Matrix(1, num_hidden_states);
	transition_matrix = new Matrix(num_hidden_states, num_hidden_states);
	confusion_matrix = new Matrix(num_hidden_states, num_observable_states);
	
	cout << endl << "Enter the names of the hidden states." << endl;
	
	for(int i=0; i<num_hidden_states; ++i)
	{
		string s;
		cin >> s;
		hidden_states[s] = i;
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

double forward ()
{
	int n;
	
	cout << endl << "Enter the number of observations: ";
	cin >> n;
	cout << endl << "Enter the observations: ";
	
	vector <int> observations(n);
	
	for (int i=0; i<n; ++i)
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
	
	
}

};
