#ifndef MATRIX
#define MATRIX
#include "matrix.cpp"
#endif
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

class MDP
{
public:

int num_states;
int num_actions;

double discount_factor;

unordered_map <int, Matrix *> transitions; 
Matrix * rewards;

MDP (int num_states, int num_actions, double discount_factor)
{
	this->num_states = num_states;
	this->num_actions = num_actions;
	this->discount_factor = discount_factor;
	
	rewards = new Matrix(num_states, num_actions);
	
	for(int i=0; i<num_actions; ++i)
	{
		transitions[i] = new Matrix(num_states, num_states);
		cout << endl << "Enter the transition matrix corresponding to action " << i+1 << ": " << endl;
		transitions[i]->take_input();
	}
	
	cout << endl << "Enter the rewards for the actions corresponding to each state (matrix is of the form (state, action_reward)): " << endl;
	
	rewards->take_input();
}

vector <double> policyReward (Matrix * policy)
{
	vector <double> reward (num_states, 0);
	
	for(int i=0; i<num_states; ++i)
	{
		for(int j=0; j<num_actions; ++j)
		{
			reward[i] += rewards->mat[i][j] * policy->mat[i][j];
		}
	}
	
	return reward;
}

Matrix * policyTransitions (Matrix * cur_policy)
{
	Matrix * trans_policy = new Matrix(num_states, num_states);
	
	for(int i=0; i<num_states; ++i)
	{
		for (std::unordered_map <int, Matrix *>::iterator it = transitions.begin(); it != transitions.end(); ++it) 
		{	
			for(int j=0; j<num_states; ++j)
			{
				trans_policy->mat[i][j] += it->second->mat[i][j] * cur_policy->mat[i][it->first];
			}
		}
	}
	
	return trans_policy;
}

//Compute the result of the Bellman equation
vector<double> bellmanEquation(Matrix * policyTrans, vector<double> & policyRew, vector<double> & valueFunc) 
{
	vector <double> result(num_states, 0);
	
	for(int i=0; i<num_states; ++i)
	{
		double sum = 0;
		for(int j=0; j<num_states; ++j)
		{
			sum += policyTrans->mat[i][j] * valueFunc[j];
		}
		
		result[i] += policyRew[i] + discount_factor * sum;
	}
	
	return result;
}

//Compute the value function associated with a given policy
vector<double> policyEvaluation(Matrix * pTransProb, vector<double> & pReward, double epsilon) {

	//Initialize value function to zero
	vector <double> valueFunction =  vector<double>(pReward.size(), 0);

	double delta = 10.0;
	
	while (delta > epsilon) {
			
		vector<double> previousValueFunction = valueFunction;

		//compute value function via the Bellman equation
		valueFunction = bellmanEquation(pTransProb, pReward, valueFunction);

		//Check for convergence
		vector <double> diffVect(valueFunction.size());
		
		for(int i=0; i<valueFunction.size(); ++i)
			diffVect[i] = valueFunction[i] - previousValueFunction[i];

		//get maximum element
		vector<double>::iterator result;

		//delta is most changed element (if it is small then convergence has occurred)
		delta = *std::max_element(diffVect.begin(), diffVect.end());
	}

	return valueFunction;
}

struct actionValue {
	int action;
	double value;
};

//Greedy policy improvement given the current policy's value function
Matrix * policyImprovement(vector<double> & valueFunction) {

	Matrix * greedyPolicy = new Matrix(num_states, num_actions);

	for (int i = 0; i < num_states; ++i) {

		actionValue greedyAction = { -1, 0.0 };

		for (std::unordered_map<int, Matrix *>::iterator it = transitions.begin(); it != transitions.end(); ++it) {

			//compute the value associated with this action at this state
			double value = 0;
			for(int j=0; j<num_states; ++j)
			{
				value += it->second->mat[i][j] * valueFunction[j];
			}
			
			value += rewards->mat[i][it->first];

			//select the greedy action in terms of value
			if (value > greedyAction.value) {
				greedyAction.action = it->first;
				greedyAction.value = value;
			}
		}

		greedyPolicy->mat[i][greedyAction.action] = 1.0;
	}

	return greedyPolicy;
}

void getPolicy(int num_iterations)
{
	Matrix * policy = new Matrix(num_states, num_actions);
	
	for(int i=0; i<num_states; ++i)
	{
		for(int j=0; j<num_actions; ++j)
		{
			if(i==j)
				policy->mat[i][j] = 1;
		}
	}
	
	Matrix  * oldPolicy = new Matrix(num_states, num_actions);
	
	vector <double> policyValue;
	
	for(int i=1; i<=num_iterations; ++i)
	{	
		delete(oldPolicy);
			
		oldPolicy = policy;
		
		Matrix * policyTrans = policyTransitions(policy);
		
		vector <double> policy_reward = policyReward(policy);
		
		policyValue = policyEvaluation(policyTrans, policy_reward, 0.001);
		
		policy = policyImprovement(policyValue);
		
		delete(policyTrans);
	}
	
	cout << endl << "The optimum policy is: ";
	policy->print_matrix();
	
	cout << "The corresponding policy values for the states are: " << endl;
	
	for(int i=0; i<policyValue.size(); ++i)
		cout << policyValue[i] << " ";
	cout << endl;
}

};
