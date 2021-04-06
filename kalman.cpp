#include <iostream>
#include <stdexcept>
#ifndef MATRIX
#define MATRIX
#include "matrix.cpp"
#endif
using namespace std;
class KalmanFilter {
public:

  /*
     A - System dynamics matrix
     C - Output matrix
     Q - Process noise covariance
     R - Measurement noise covariance
     P - Estimate error covariance
     K - Kalman Gain
  */

  Matrix A,C,Q,R,P,K,P0;


  int m0,n0; // // System dimensions
  double t0, t, dt; // time
  bool initialized; //filter Initialized?
  Matrix I; //identity matrix
  vector<double> x_hat, x_hat_new;  // Estimated states

  KalmanFilter() {}

  KalmanFilter(double dt, const Matrix A, const Matrix C, const Matrix Q, const Matrix R, const Matrix P):
  m0(C.m), n0(A.m), dt(dt), initialized(false)
    {
	  this->A = A;
	  this->C = C;
	  this->Q = Q;
	  this->R = R;
	  this->P0 = P;
      I.m = n0; I.n = n0;
      I.mat.resize(n0,vector<double>(n0));
      x_hat.resize(n0);
      x_hat_new.resize(n0);
      I.set_identity();
    }

    void init(double t0, const vector<double> x0) {
      this->x_hat = x0;
      P.m = n0; P.n = n0;
      P.mat.resize(n0,vector<double>(n0));
      P = P0;
      this->t0 = t0;
      t = t0;
      initialized = true;
    }

    void init() {
      fill(this->x_hat.begin(), this->x_hat.end(), 0);
      P.m = n0; P.n = n0;
      P.mat.resize(n0,vector<double>(n0));
      P = P0;
      t0 = 0;
      t = 0;
      initialized = true;
    }

  void update(const vector<double> & y) {

      if(!initialized)
      throw std::runtime_error("Filter is not initialized!");

// *********PREDICT*************

    // 1). x_hat_new = A * x_hat
      double value = 0;
      for(int i = 0 ; i<n0; i++){
        value = 0;
        for(int j = 0; j<n0; j++){
          value +=  A.mat[i][j]*x_hat[j];
        }
        x_hat_new[i] = value;
      }

      // 2). P = A*P*A.transpose() + Q
      P = Matrix::mult(Matrix::mult(A,P),A.transpose()) + Q;

// *********UPDATE***************

    // 1). K = P*C.transpose() / (C*P*C.transpose() + R)
      K.m = n0 ; K.n = m0;
      K.mat.resize(n0,vector<double>(m0));
      K = Matrix::mult(Matrix::mult(P,C.transpose()),((Matrix::mult(Matrix::mult(C,P),C.transpose()) + R).inverse()));

    // 2).  x_hat_new =   x_hat_new + K * (y - C*x_hat_new)
    value = 0;
    vector<double> temp(m0,0);
    for(int i = 0 ; i<m0; i++){
      value = 0;
      for(int j = 0; j<n0; j++){
        value += C.mat[i][j]*x_hat_new[j];
      }
      temp[i] = value;
    }

    for(int i = 0 ; i<n0; i++){
      value = 0;
      for(int j = 0; j<m0; j++){
        value += (y[j]-temp[j])* K.mat[i][j];
      }
      x_hat_new[i] += value;
    }

    // 3).  P = (I - K*C)*P
      P = Matrix::mult((I - Matrix::mult(K,C)),P);

      x_hat = x_hat_new;
      t += dt;
    }

    void update(const vector<double> & y, double dt, const Matrix & A) {

      this->A = A;
      this->dt = dt;
      update(y);
    }

    vector<double> state() { return x_hat; }
    double time() { return t; }
  };
