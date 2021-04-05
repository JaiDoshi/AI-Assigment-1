#include <iostream>
#include "markov.cpp"
#include "HMM.cpp"
#include "MDP.cpp"
#include "kalman.cpp"

using namespace std;

void printTranspose(vector<double> v){
	for(auto i : v)
	cout << i << endl;
}

void markov()
{
	int num_states;

	cout << endl << "Enter the total number of states: ";
	cin >> num_states;

	State model(num_states);

	int option;

	while(1)
	{
		cout << endl << "Enter 1 for calculating nth state, 2 for calculating equilibrium state, 3 for calculating probability of a sequence of states, or 0 to exit Markov Model." << endl;
		cout << endl << "Enter the desired option: ";
		cin >> option;

		switch(option)
		{
			case 0:
			return;

			case 1:{
				int n;
				cout << endl << " Enter the desired number of iterations: ";
				cin >> n;
				model.nth(n);
				break;
			}

			case 2:{
				model.equi();
				break;
			}

			case 3:{
				cout<< endl << "Enter the number of states in your sequence  ";
				int num_sequence;
				cin >> num_sequence;
				vector<string> v(num_sequence);
				cout << endl << "Enter the sequence of states  ";
				for(int i = 0 ; i<num_sequence ; i++)
				cin >> v[i];
				model.sequence(v);
				break;
			}
		}
	}
}


void hidden_markov()
{
	int num_hidden_states;
	int num_observable_states;

	cout << endl << "Enter the number of hidden states: ";
	cin >> num_hidden_states;
	cout << "Enter the number of observable states: ";
	cin >> num_observable_states;

	HMM model(num_hidden_states, num_observable_states);

	int option;

	while(1)
	{
		cout << endl << "Enter 1 for problem 1, 2 for problem 2, 3 for problem 3, or 0 to exit Hidden Markov Model." << endl;
		cout << endl << "Enter the desired option: ";
		cin >> option;

		switch(option)
		{
			case 0:
			return;

			case 1:
			{
				double probability = model.forward();
				cout << endl << "The probability of the given observation sequence is: " << probability << endl;
				break;
			}

			case 2:
			{
				vector <string> most_probable_states = model.Viterbi();
				cout << endl << "The most probable sequence of hidden states is: ";
				for(string & s: most_probable_states)
				{
					cout << s << " ";
				}
				cout << endl;
				break;
			}

			case 3:
			{
				cout << endl << "Enter the desired number of iterations (recommended=100): " << endl;
				int num_iterations;
				cin >> num_iterations;
				model.backward(num_iterations);
				break;
			}
		}
	}
}

void markov_decision()
{
	int num_states;
	int num_actions;
	double discount_factor;

	cout << endl << "Enter the number of states: ";
	cin >> num_states;
	cout << "Enter the number of actions: ";
	cin >> num_actions;
	cout << "Enter the discount factor: ";
	cin >> discount_factor;

	MDP model(num_states, num_actions, discount_factor);

	int num_iterations;

	cout << endl << "Enter the number of iterations (recommended=100): ";
	cin >> num_iterations;

	model.getPolicy(num_iterations);
}

void kalman(){
	int n , m;
	double dt , t =0;
	cout << endl << "Enter the number of states(n)";
	cin >> n;
	cout << endl << "Enter the number of outputs(m)";
	cin >> m;
	cout << endl << "Enter the timestep";
	cin >> dt;
	
	vector<double> x0(n);
	cout << " Enter the initial states " ;
	for(auto i : x0)
	cin >> i;

	Matrix A(n,n) , C(m,n) , Q(n,n) , R(m,m) , P(n,n);
	cout << endl << " Enter the values for matrices A(sytem dynamics : nXn ), C(output : mXn)";
	cout << endl << " Q(Process noise covariance : nXn), R(Measurement noise covariance : mXm), P(Estimate error covariance nXn) (in this order)" << endl;
	A.take_input(); C.take_input() ; Q.take_input() ; R.take_input() ; P.take_input();

	KalmanFilter kf(dt, A, C, Q, R, P);
	int x;
	cout << " Enter the number of (noisy) measurements";
	cin >> x;

	vector<double> measurements(x);
	cout << " Enter the List of noisy position measurements (y) " << endl;
	for(auto i : measurements)
	cin >> i;

	kf.init(t,x0);
	vector<double> y(m);

	cout << "t = " << t << ", " << "x_hat[0]: ";
	printTranspose(kf.state());
	cout << endl;
	for(int i = 0; i < measurements.size(); i++) {
		t += dt;
		y[0] = measurements[i];
		kf.update(y);
		cout << "t = " << t << ", " << "y[" << i << "] = ";
		printTranspose(y);
		cout << ", x_hat[" << i << "] = ";
		printTranspose(kf.state());
		cout << endl;
	}

}

int main()
{

	int option;

	while(1)
	{
		cout << endl << "1 - Markov Model" << endl;
		cout << "2 - Hidden Markov Model" << endl;
		cout << "3 - Markov Decision Process" << endl;
		cout << "4 - Kalman Filter" << endl;
		cout << endl << "Enter the desired option. Enter 0 to exit: ";
		cin >> option;

		switch(option)
		{
			case 0:
			return 0;

			case 1:
			{
				markov();
				break;
			}

			case 2:
			{
				hidden_markov();
				break;
			}

			case 3:
			{
				markov_decision();
				break;
			}
			break;

			case 4:
			kalman();
			break;
		}
	}
}
