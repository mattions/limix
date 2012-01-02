/*
 * CGPopt.h
 *
 *  Created on: Jan 1, 2012
 *      Author: stegle
 */

#ifndef CGPOPT_H_
#define CGPOPT_H_

#include "gp_base.h";
#include "nlopt/api/nlopt.h"


namespace gpmix {
#define solver NLOPT_LD_LBFGS
#define DEFAULT_TOL 1E-6

class CGPopt {
protected:

	CGPbase& gp;
	CGPHyperParams optParams;
	CGPHyperParams filter;
	mfloat_t tolerance;
	muint_t numEvaluations;

	//objective without and with gradients
	mfloat_t objective(const VectorXd& paramArray);
	mfloat_t objective(const VectorXd& paramArray,VectorXd* gradParamArray);
	//optimization interface for nlopt:
	static double gpopt_nlopt_objective(unsigned n, const double *x, double *grad, void *my_func_data);
public:
	CGPopt(CGPbase& gp);
	virtual ~CGPopt();
	virtual bool gradCheck(mfloat_t relchange=1E-5,mfloat_t threshold=1E-2);
	virtual void opt();

	CGPHyperParams getFilter() const;
	void setFilter(CGPHyperParams filter);
	double getTolerance() const;
	void setTolerance(double tol = 1E-4);

	CGPHyperParams getOptParams()
	{ return optParams; }

};




} /* namespace gpmix */
#endif /* CGPOPT_H_ */