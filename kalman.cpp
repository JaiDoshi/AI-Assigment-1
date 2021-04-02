#include <iostream>
#include <stdexcept>
#include "matrix.cpp"
using namespace std;
class KalmanFilter {
  /**
  *   A - System dynamics matrix
  *   C - Output matrix
  *   Q - Process noise covariance
  *   R - Measurement noise covariance
  *   P - Estimate error covariance
  */

public:
  Matrix A,C,Q,R,P,K,P0;

  // System dimensions
  int m0,n0;

  // Initial and current time
  double t0, t;

  // Discrete time step
  double dt;

  // Is the filter initialized?
  bool initialized;

  // n-size identity
  Matrix I;

  // Estimated states
  vector<double> x_hat, x_hat_new;

  KalmanFilter(double dt, const Matrix A, const Matrix C, const Matrix Q, const Matrix R, const Matrix P):
  A(A), C(C), Q(Q), R(R), P0(P),m0(C.m), n0(A.m), dt(dt), initialized(false),I(n0, n0), x_hat(n0), x_hat_new(n0)
    {
      I.set_identity();
    }

    KalmanFilter() {}

    void init(double t0, const vector<double> x0) {
      x_hat = x0;
      P = P0;
      this->t0 = t0;
      t = t0;
      initialized = true;
    }

    void init() {
      fill(x_hat.begin(), x_hat.end(), 0);
      P = P0;
      t0 = 0;
      t = t0;
      initialized = true;
    }

    void update(const vector<double> y) {

      if(!initialized)
      throw std::runtime_error("Filter is not initialized!");

    //  x_hat_new = A * x_hat; // matrix with vector

      P = Matrix::mult(Matrix::mult(A,P),A.transpose()) + Q;
      K = Matrix::mult(Matrix::mult(P,C.transpose()),((Matrix::mult(Matrix::mult(C,P),C.transpose()) + R).inverse()));


    //  x_hat_new += K * (y - C*x_hat_new); // matrix with vector

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
