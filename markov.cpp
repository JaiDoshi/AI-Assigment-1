#include <vector>
#include <iostream>
#include<math.h>
#include<unordered_map>
#ifndef MATRIX
#define MATRIX
#include "matrix.cpp"
#endif

using namespace std;

class State{
public:
  int m;
  Matrix s;
  Matrix transProb;

  unordered_map<string,int> mapping_sn;
  vector<string> mapping_ns;
  State(int m){
    this->m = m;
    s.m = 1;
    s.n = m;
    s.mat.resize(1,vector<double>(m));

	  mapping_ns.resize(m);

    cout<< "Enter the names of the states -" << endl;
  	string temp = "";
  	for(int i=0; i<m; i++){
  		cin >> temp;
  		mapping_sn[temp] = i;
  		mapping_ns[i] = temp;
  	}

    cout << "Enter the probabilities of initial matrix" ;
    s.take_input();
    transProb.m = m;
    transProb.n = m;
    transProb.mat.resize(m,vector<double>(m));
    cout << "Enter the values in transition probability matrix";
    transProb.take_input();
  }


  string nth(int n){
    State fin = *this;
    for(int i = 0 ; i<n ; i++)
      fin.s = fin.s.mult(fin.s,transProb);

    int most_probable_state = 0 ,  res = 0;
    for(int i = 0 ;i < m; i++){
      if(fin.s.mat[0][i]>res){
        res = fin.s.mat[0][i];
        most_probable_state = i;
      }
    }
    return mapping_ns[most_probable_state];
  }

  string equi(){
    State fin = *this;
    State prev = fin;
    Matrix dev(1,m) ;
    dev.mat[0][1] = 1;
    int i = 1;
    while(dev.mat[0][i-1] >= 0.0001 && i<=100){
      prev.s = fin.s;
      fin.s = fin.s.mult(fin.s,transProb);
      dev.mat[0][i] = calcDev(fin.s,prev.s);
      i++;
    }
    int most_probable_state = 0 ,  res = 0;
    for(int i = 0 ;i < m; i++){
      if(fin.s.mat[0][i]>res){
        res = fin.s.mat[0][i];
        most_probable_state = i;
      }
    }
    return mapping_ns[most_probable_state];
  }

  double calcDev(Matrix & s1, Matrix & s2 ){

    if(s1.m != 1 || s2.m != 1 || s1.n != s2.n){
      return 1.00;
    }
    Matrix dev(1,s1.n);
    double res = 0;
    for(int i = 0 ; i< s1.n ; i++){
      res += (s1.mat[0][i] - s2.mat[0][i])*(s1.mat[0][i] - s2.mat[0][i]);
    }
    return res;
  }

  double sequence(vector<string> v){
    double res = 1;
    int a = 0, b = 0;
    for(int i = 1; i <v.size(); i++){
      a = mapping_sn[v[i-1]];
      b = mapping_sn[v[i]];
      res *= transProb.mat[a][b];
    }
    return res;
  }

};
