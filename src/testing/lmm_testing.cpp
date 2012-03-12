#if 1
//============================================================================
// Name        : GPmix.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "gpmix/types.h"
#include "gpmix/covar/covariance.h"
#include "gpmix/covar/linear.h"
#include "gpmix/utils/matrix_helper.h"
#include "gpmix/LMM/lmm.h"
#include "gpmix/LMM/lmm_old.h"
using namespace gpmix;

using namespace std;
using namespace gpmix;

int main() {

	int n = 100;
	int p = 3;
	int s = 10;
	int ncov = 1;
	MatrixXd snps = (MatrixXd)randn((muint_t)n,(muint_t)s);
	MatrixXd pheno = (MatrixXd)randn((muint_t)n,(muint_t)p);
	MatrixXd covs = MatrixXd::Ones(n,ncov);

	MatrixXd K = 1.0/snps.cols() * (snps*snps.transpose());

	//Default settings:
	int num_intervals0 = 100;
	int num_intervalsAlt = 0;
	double ldeltamin0 = -5;
	double ldeltamax0 = 5;
	double ldeltaminAlt = -1.0;
	double ldeltamaxAlt =1.0;
	MatrixXd pvals = MatrixXd(p, s);

	if (1){ //LMM testing using old code
		lmm_old::train_associations(&pvals, snps, pheno,	K, covs, num_intervalsAlt,ldeltaminAlt, ldeltamaxAlt, num_intervals0, ldeltamin0, ldeltamax0);
		cout << "pv_old:\n"<<scientific<<pvals<<endl;
	}



	if (1){ //LMM testing using new code
		CLMM lmm;


		VectorXd v = VectorXd::Ones(3);
		MatrixXd M = MatrixXd::Ones(3,2);

		lmm.setK(K);
		lmm.setSNPs(covs);
		lmm.setPheno(pheno);
		lmm.setCovs(covs);
		lmm.setNumIntervals0(10);
		lmm.setTestStatistics(lmm.TEST_F);

		lmm.process();
		std::cout << lmm.getLdelta0() << "\n\n\n\n";
		std::cout << lmm.getLdeltaAlt() << "\n";

		std::cout << lmm.getNLL0() << "\n\n\n\n";
		std::cout << lmm.getNLLAlt() << "\n";

		MatrixXd pv = lmm.getPv();
		cout <<"pv_new:\n"<< scientific <<pv<<endl;



		CInteractLMM ilmm;
		ilmm.setK(K);
		ilmm.setSNPs(snps);
		ilmm.setPheno(pheno);
		ilmm.setCovs(covs);
		ilmm.setInter(covs);
		ilmm.setInter0(MatrixXd::Ones(snps.rows(),1));

		ilmm.setTestStatistics(lmm.TEST_F);

		ilmm.process();
		MatrixXd ipv = ilmm.getPv();


		std::cout << pv-ipv << "\n";
	}


}
#endif
