#include <iostream>
#include "tools.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
	VectorXd rmse(4);
	rmse << 0,0,0,0;

	// check the validity of the following inputs:
	//  * the estimation vector size should not be zero
	//  * the estimation vector size should equal ground truth vector size
	if(estimations.size() != ground_truth.size()
			|| estimations.size() == 0){
		cout << "Invalid estimation or ground_truth data" << endl;
		return rmse;
	}

	//accumulate squared residuals
	for(unsigned int i=0; i < estimations.size(); ++i){

		VectorXd residual = estimations[i] - ground_truth[i];

		//coefficient-wise multiplication
		residual = residual.array()*residual.array();
		rmse += residual;
	}

	//calculate the mean
	rmse = rmse/estimations.size();

	//calculate the squared root
	rmse = rmse.array().sqrt();

	//return the result
	return rmse;
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {

  	MatrixXd Hj(3,4);
	//recover state parameters
	float px = x_state(0);
	float py = x_state(1);
	float vx = x_state(2);
	float vy = x_state(3);

	//check division by zero
	float divisor = px*px+py*py;
	float divisor_root = pow(divisor, 0.5);
	float divisor_3by2 = pow(divisor,1.5);
	
  //check division by zero

	if (fabs(divisor_3by2) < 0.0001){

	  std::cout << "CalculateJacobian () - Error - Division by Zero" << std::endl;

	  return Hj;

	}
	if (divisor != 0){
	    
	    Hj(0,0) = px/divisor_root;
    	Hj(0,1) = py/divisor_root;
    	Hj(0,2) = 0;
    	Hj(0,3) = 0;
    	Hj(1,0) = -(py/divisor);
    	Hj(1,1) = px/divisor;
    	Hj(1,2) = 0;
    	Hj(1,3) = 0;
    	Hj(2,0) = py*(vx*py-vy*px)/divisor_3by2;
    	Hj(2,1) = px*(vy*px-vx*py)/divisor_3by2;
    	Hj(2,2) = Hj(0,0);
    	Hj(2,3) = Hj(0,1);
	}
	return Hj;
}
