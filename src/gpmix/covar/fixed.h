/*
 * fixed.h
 *
 *  Created on: Nov 11, 2011
 *      Author: stegle
 */

#ifndef FIXED_H_
#define FIXED_H_

#include <gpmix/covar/covariance.h>

namespace gpmix {

class CFixedCF : public ACovarianceFunction {
protected:
	MatrixXd K0;
	MatrixXd K0cross;
public:
	CFixedCF(const MatrixXd& K0);
 	CFixedCF() : ACovarianceFunction(1)
	{};
	~CFixedCF();


	//overloaded pure virtual functions:
	virtual void aKcross(MatrixXd* out, const CovarInput& Xstar ) const throw(CGPMixException);
	virtual void aKgrad_param(MatrixXd* out,const muint_t i) const throw(CGPMixException);
	virtual void aKcross_grad_X(MatrixXd* out,const CovarInput& Xstar, const muint_t d) const throw(CGPMixException);
	virtual void aKdiag_grad_X(VectorXd* out,const muint_t d) const throw(CGPMixException);
	//other overloads
	virtual void aK(MatrixXd* out) const;
	//setter and getters
	void setK0(const MatrixXd& K0);
	void setK0cross(const MatrixXd& Kcross);
	void getK0(MatrixXd* out) const;
	void getK0cross(MatrixXd* out) const;

	//class information
	inline string getName() const{ return "FixedCF";}


#ifndef SWIG
	MatrixXd getK0() const;
	MatrixXd getK0cross() const;
#endif

};

} /* namespace gpmix */
#endif /* FIXED_H_ */
