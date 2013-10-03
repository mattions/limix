/*
 * trait.h
 *
 *  Created on: Jan 16, 2012
 *      Author: stegle
 */

#ifndef FREEFORM_H_
#define FREEFORM_H_

#include "covariance.h"

namespace limix {

    
#if (defined(SWIG) && !defined(SWIG_FILE_WITH_INIT))
//ignore C++ versions
%ignore CFreeFormCF::getIparamDiag;
%ignore CFreeFormCF::K0Covar2Params;
//rename argout versions for python; this overwrites the C++ convenience functions
%rename(getIparamDiag) CFreeFormCF::agetIparamDiag;
#endif

class CFreeFormCF: public ACovarianceFunction {
        
protected:
	muint_t numberGroups;
    //Calculate the number of parameter
    static muint_t calcNumberParams(muint_t numberGroups);
    //helper function to convert from matrix to hyperparams
    void aK0Covar2Params(VectorXd* out,const MatrixXd& K0);
public:
    
    CFreeFormCF(muint_t numberGroups);
    ~CFreeFormCF();
    
	//Block X functions: X is fixed and set in the constructor
	virtual void setX(const CovarInput& X) throw (CGPMixException) {};
	virtual void setXcol(const CovarInput& X, muint_t col) throw (CGPMixException) {};
	virtual void aKcross_diag(VectorXd* out, const CovarInput& Xstar) const throw(CGPMixException);
	virtual void aKcross_grad_X(MatrixXd* out,const CovarInput& Xstar, const muint_t d) const throw(CGPMixException) {};
	virtual void aKdiag_grad_X(VectorXd* out,const muint_t d) const throw(CGPMixException) {};

    virtual void agetScales(CovarParams* out);
    virtual void setParamsCovariance(const MatrixXd& K0) throw(CGPMixException);

    //Covariance pure functions
	//pure functions that need to be implemented
	virtual void aKcross(MatrixXd* out, const CovarInput& Xstar ) const throw(CGPMixException);
	virtual void aKgrad_param(MatrixXd* out,const muint_t i) const throw(CGPMixException);
    virtual void aKhess_param(MatrixXd* out,const muint_t i,const muint_t j) const throw(CGPMixException);
    virtual void agetParamMask0(CovarParams* out) const;
    
    //class information
    inline std::string getName() const {return "CFreeFormCF";};
    
    //FreeForm-specific functions
    virtual void setParamsVarCorr(const CovarParams& paramsVC) throw(CGPMixException);
    virtual void agetL0(MatrixXd* out) const;
    virtual void agetL0grad_param(MatrixXd* out,muint_t i) const throw(CGPMixException);
    //information on parameter settings
    void agetIparamDiag(VectorXi* out) const;
    VectorXi getIparamDiag() const
    {
        VectorXi rv;
        agetIparamDiag(&rv);
        return rv;
    }

};
typedef sptr<CFreeFormCF> PFreeFormCF;



class CRankOneCF: public ACovarianceFunction {

protected:
	muint_t numberGroups;
public:

    CRankOneCF(muint_t numberGroups);
    ~CRankOneCF();

	//Block X functions: X is fixed and set in the constructor
	virtual void setX(const CovarInput& X) throw (CGPMixException) {};
	virtual void setXcol(const CovarInput& X, muint_t col) throw (CGPMixException) {};
	virtual void aKcross_diag(VectorXd* out, const CovarInput& Xstar) const throw(CGPMixException);
	virtual void aKcross_grad_X(MatrixXd* out,const CovarInput& Xstar, const muint_t d) const throw(CGPMixException) {};
	virtual void aKdiag_grad_X(VectorXd* out,const muint_t d) const throw(CGPMixException) {};

    virtual void agetScales(CovarParams* out);
    virtual void setParamsCovariance(const MatrixXd& K0) throw(CGPMixException);

    //Covariance pure functions
	//pure functions that need to be implemented
	virtual void aKcross(MatrixXd* out, const CovarInput& Xstar ) const throw(CGPMixException);
	virtual void aKgrad_param(MatrixXd* out,const muint_t i) const throw(CGPMixException);
    virtual void aKhess_param(MatrixXd* out,const muint_t i,const muint_t j) const throw(CGPMixException);
    virtual void agetParamMask0(CovarParams* out) const;

    //class information
    inline std::string getName() const {return "CRankOneCF";};

};
typedef sptr<CRankOneCF> PRankOneCF;


//rename argout operators for swig interface
#if (defined(SWIG) && !defined(SWIG_FILE_WITH_INIT))
//rename functions and select the visible component for SWIG
%rename(getK0) CFixedCF::agetK0;
%rename(getK0cross) CFixedCF::agetK0cross;
%rename(getK0cross_diag) CFixedCF::agetK0cross_diag;
//%sptr(gpmix::CFixedCF)
#endif


class CFixedCF: public ACovarianceFunction {

protected:
	muint_t numberGroups;
    MatrixXd K0;
	MatrixXd K0cross;
	VectorXd K0cross_diag;
public:

    CFixedCF(const MatrixXd& K0);
    ~CFixedCF();

    virtual void agetScales(CovarParams* out);
    virtual void setParamsCovariance(const MatrixXd& K0) throw(CGPMixException);

	//overloaded pure virtual functions:
	virtual void aKcross(MatrixXd* out, const CovarInput& Xstar ) const throw(CGPMixException);
	virtual void aKcross_diag(VectorXd* out, const CovarInput& Xstar) const throw(CGPMixException);
	virtual void aKgrad_param(MatrixXd* out,const muint_t i) const throw(CGPMixException);
    virtual void aKhess_param(MatrixXd* out, const muint_t i, const muint_t j) const throw(CGPMixException);
	virtual void aKcross_grad_X(MatrixXd* out,const CovarInput& Xstar, const muint_t d) const throw(CGPMixException);
	virtual void aKdiag_grad_X(VectorXd* out,const muint_t d) const throw(CGPMixException);
	//other overloads
	virtual void aK(MatrixXd* out) const throw (CGPMixException);

    virtual void agetParamMask0(CovarParams* out) const;

	//setter and getters
	void setK0(const MatrixXd& K0);
	void setK0cross(const MatrixXd& Kcross);
	void agetK0(MatrixXd* out) const;
	void agetK0cross(MatrixXd* out) const;
	void setK0cross_diag(const VectorXd& Kcross_diag);
	void agetK0cross_diag(VectorXd* out) const;

    //class information
    inline std::string getName() const {return "CFixedCF";};

};
typedef sptr<CFixedCF> PFixedCF;
    
    
class CDiagonalCF: public ACovarianceFunction {

protected:
	muint_t numberGroups;
public:

    CDiagonalCF(muint_t numberGroups);
    ~CDiagonalCF();

	//Block X functions: X is fixed and set in the constructor
	virtual void setX(const CovarInput& X) throw (CGPMixException) {};
	virtual void setXcol(const CovarInput& X, muint_t col) throw (CGPMixException) {};
	virtual void aKcross_diag(VectorXd* out, const CovarInput& Xstar) const throw(CGPMixException);
	virtual void aKcross_grad_X(MatrixXd* out,const CovarInput& Xstar, const muint_t d) const throw(CGPMixException) {};
	virtual void aKdiag_grad_X(VectorXd* out,const muint_t d) const throw(CGPMixException) {};

    virtual void agetScales(CovarParams* out);
    virtual void setParamsCovariance(const MatrixXd& K0) throw(CGPMixException);

    //Covariance pure functions
	//pure functions that need to be implemented
	virtual void aKcross(MatrixXd* out, const CovarInput& Xstar ) const throw(CGPMixException);
	virtual void aKgrad_param(MatrixXd* out,const muint_t i) const throw(CGPMixException);
    virtual void aKhess_param(MatrixXd* out,const muint_t i,const muint_t j) const throw(CGPMixException);
    virtual void agetParamMask0(CovarParams* out) const;

    //class information
    inline std::string getName() const {return "CDiagonalCF";};

};
typedef sptr<CDiagonalCF> PDiagonalCF;
    

#if (defined(SWIG) && !defined(SWIG_FILE_WITH_INIT))
%rename(getRank1)    CRank1diagCF::agetRank1;
%rename(getDiag) CRank1diagCF::agetDiag;
#endif


class CRank1diagCF: public ACovarianceFunction {

protected:
	muint_t numberGroups;
public:

    CRank1diagCF(muint_t numberGroups);
    ~CRank1diagCF();

	//Block X functions: X is fixed and set in the constructor
	virtual void setX(const CovarInput& X) throw (CGPMixException) {};
	virtual void setXcol(const CovarInput& X, muint_t col) throw (CGPMixException) {};
	virtual void aKcross_diag(VectorXd* out, const CovarInput& Xstar) const throw(CGPMixException);
	virtual void aKcross_grad_X(MatrixXd* out,const CovarInput& Xstar, const muint_t d) const throw(CGPMixException) {};
	virtual void aKdiag_grad_X(VectorXd* out,const muint_t d) const throw(CGPMixException) {};

    virtual void agetScales(CovarParams* out);
    virtual void setParamsCovariance(const MatrixXd& K0) throw(CGPMixException);

    virtual void agetRank1(MatrixXd* out) const throw(CGPMixException);
    virtual void agetDiag(MatrixXd* out) const throw(CGPMixException);
    
    //Covariance pure functions
	//pure functions that need to be implemented
	virtual void aKcross(MatrixXd* out, const CovarInput& Xstar ) const throw(CGPMixException);
	virtual void aKgrad_param(MatrixXd* out,const muint_t i) const throw(CGPMixException);
    virtual void aKhess_param(MatrixXd* out,const muint_t i,const muint_t j) const throw(CGPMixException);
    virtual void agetParamMask0(CovarParams* out) const;

    //class information
    inline std::string getName() const {return "CRank1diagCF";};

};
typedef sptr<CRank1diagCF> PRank1diagCF;


} /* namespace limix */
#endif /* FREEFORM_H_ */
