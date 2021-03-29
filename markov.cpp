#include <vector>
#include <iostream>
#include<math.h>
#include "matrix.cpp"
using namespace std;

class State{
  int m;
  Matrix s;
  Matrix transProb;
  State(int m){
    this->m = m;
    s.mat.resize(m,vector<double>(1));
    cout << "Enter the probabilities of state" ;
    s.take_input();
    transProb.mat.resize(m,vector<double>(m));
    cout << "Enter the values in transition probability matrix";
    transProb.take_input();
  }


  State nth(State initial /*,Matrix transProb*/){
    State fin = initial;
    for(int i = 0 ; i<initial.m ; i++){
      fin.s = fin.s.mult(fin.s,initial.transProb);
    }
    return fin;
  }

  State equi(State initial /*,Matrix transProb*/){
    State fin = initial;
    State prev = fin;
    Matrix dev(initial.m,1) ;
    dev.mat[0][1] = 1;
    int i = 1;
    while(dev.mat[i-1][1] >= 0.0001 && i<100){
      prev.s = fin.s;
      fin.s = fin.s.mult(fin.s,initial.transProb);
      dev.mat[i][1] = calcDev(fin.s,prev.s);
      i++;
    }
  }
  double calcDev(Matrix & s1, Matrix & s2 ){

    if(s1.n != 1 || s2.n != 1 || s1.m != s2.m){
      return 1.00;
    }
    Matrix dev(s1.m,1);
    double res = 0;
    for(int i = 0 ; i<s1.m ; i++){
      res += (s1.mat[i][1] - s2.mat[i][1])*(s1.mat[i][1] - s2.mat[i][1]);
    }
    return res;
  }

};


double mult_prob(vector<int> v, Matrix transProb){
  double res = 1;
  for(int i = 1; i <v.size(); i++){
    res *= transProb.mat[v[i-1]][v[i]];
  }
  return res;
}
