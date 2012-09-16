/*
 * CGPopt.h
 *
 *  Created on: Jan 1, 2012
 *      Author: stegle
 */

#ifndef CGPOPT_H_
#define CGPOPT_H_

#include "gp_base.h"
#include "nlopt/api/nlopt.h"
#include <vector>



namespace limix {
//#define solver NLOPT_LD_SLSQP
#define solver NLOPT_LD_LBFGS
#define DEFAULT_TOL 1E-4

#if (defined(SWIG) && !defined(SWIG_FILE_WITH_INIT))
//list of CGPHyperParams
//%template(HyperParmasVec) std::vector<gpmix::CGPHyperParams>;
#endif

class CGPopt {
protected:
	//gp object which is optimized:
	PGPbase gp;
	//starting points for optimization, if any
	std::vector<CGPHyperParams> optStartParams;
	CGPHyperParams optParams;
	mfloat_t optLML;
	CGPHyperParams optBoundLower;
	CGPHyperParams optBoundUpper;
	CGPHyperParams optParamMask;
	mfloat_t tolerance;
	muint_t numEvaluations;

	//objective without and with gradients
	mfloat_t objective(const VectorXd& paramArray);
	mfloat_t objective(const VectorXd& paramArray,VectorXd* gradParamArray);

	//optimization interface for nlopt:
	static double gpopt_nlopt_objective(unsigned n, const double *x, double *grad, void *my_func_data);
	//nlopt instance
	nlopt_opt optimizer;

	void completeConstraints(CGPHyperParams& constraints, const CGPHyperParams& params,mfloat_t fill_value);
public:
	CGPopt(PGPbase gp);
	virtual ~CGPopt();
	virtual bool gradCheck(mfloat_t relchange=1E-5,mfloat_t threshold=1E-2);
	virtual bool opt() throw (CGPMixException);

	CGPHyperParams getParamMask() const;
	void setParamMask(CGPHyperParams filter);
	double getTolerance() const;
	void setTolerance(double tol = 1E-4);

	muint_t getNumEvaluations() { return numEvaluations;}

	CGPHyperParams getOptBoundLower() const;
    void setOptBoundLower(CGPHyperParams optBoundLower);
    CGPHyperParams getOptBoundUpper() const;
    void setOptBoundUpper(CGPHyperParams optBoundUpper);
    std::vector<CGPHyperParams> getOptStartParams() const;
    void setOptStartParams(const std::vector<CGPHyperParams>& optStartParams);

    void addOptStartParams(const CGPHyperParams& params);
    void addOptStartParams(const VectorXd& paramArray);

    CGPHyperParams getOptParams()
	{ return optParams; }
};
typedef sptr<CGPopt> PGPopt;

} /* namespace limix */
#endif /* CGPOPT_H_ */