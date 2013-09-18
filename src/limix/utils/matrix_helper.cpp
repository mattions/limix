/*
 * matrix_helper.cpp
 *
 *  Created on: Nov 10, 2011
 *      Author: stegle
 */


#include "limix/utils/matrix_helper.h"
#include "matrix_helper.h"
#include <stdlib.h>

namespace limix{


/*
template <typename Derived>
bool isnull(const Eigen::EigenBase<Derived>& m)
{
	return ((m.cols()==0) && (m.rows()==0));
}
*/


/*
bool isnull(const MatrixXd& m)
{
	return ((m.cols()==0) && (m.rows()==0));
}


bool isnull(const VectorXi& m)
{
	return (m.rows()==0);
}
*/


bool isnull(const Eigen::LLT<MatrixXd>& m)
{
	return ((m.cols()==0) && (m.rows()==0));
}

bool isnull(const Eigen::LDLT<MatrixXd>& m)
{
	return ((m.cols()==0) && (m.rows()==0));
}

//void arrayInverseInplace(MatrixXd& m)
//{
//}



mfloat_t logdet(Eigen::LLT<MatrixXd>& chol)
{
	//1. logdet
	VectorXd L = ((MatrixXd)(chol.matrixL())).diagonal();
	mfloat_t log_det = 0.0;
	for(muint_t i = 0;i < (muint_t)(L.rows());++i){
		log_det += limix::log((mfloat_t)(L(i))); //WARNING: mfloat_t cast
	}
	return 2*log_det;
}

mfloat_t logdet(Eigen::LDLT<MatrixXd>& chol)
{
	//1. logdet
	VectorXd L = chol.vectorD();
	mfloat_t log_det = 0.0;
	//iterate: here log(sqrt)
	for(muint_t i = 0;i < (muint_t)(L.rows());++i)
	{
		log_det += limix::log(L(i));
	}
	//note: factor of 2 missing because D = sqrt(L.diag) in LLT decomposition
	return log_det;
}



#ifndef PI
#define PI 3.14159265358979323846
#endif

mfloat_t randn(mfloat_t mu, mfloat_t sigma) {
	static bool deviateAvailable=false;	//	flag
	static mfloat_t storedDeviate;			//	deviate from previous calculation
	double dist, angle;

	//	If no deviate has been stored, the standard Box-Muller transformation is
	//	performed, producing two independent normally-distributed random
	//	deviates.  One is stored for the next round, and one is returned.
	if (!deviateAvailable) {

		//	choose a pair of uniformly distributed deviates, one for the
		//	distance and one for the angle, and perform transformations
		dist=sqrt( -2.0 * log(double(rand()) / double(RAND_MAX)) );
		angle= 2.0 * PI * (double(rand()) / double(RAND_MAX));

		//	calculate and store first deviate and set flag
		storedDeviate=dist*cos(angle);
		deviateAvailable=true;

		//	calcaulate return second deviate
		return (mfloat_t)(dist * sin(angle) * sigma + mu);
	}

	//	If a deviate is available from a previous call to this function, it is
	//	returned, and the flag is set to false.
	else {
		deviateAvailable=false;
		return storedDeviate*sigma + mu;
	}
}






MatrixXd randn(const muint_t n, const muint_t m)
/* create a randn matrix, i.e. matrix of Gaussian distributed random numbers*/
{
	MatrixXd rv(n,m);
	for (muint_t i=0; i<n; i++)
		for (muint_t j=0; j<m; j++) {
			double r = randn(0.0,1.0);
			rv(i,j) = r;
		}
	return rv;
}


MatrixXd Mrand(const muint_t n,const muint_t m)
{
	MatrixXd rv(n,m);
	for (muint_t i=0;i<n;i++)
		for(muint_t j=0;j<m;j++)
		{
			rv(i,j) = ((double)rand())/((double)RAND_MAX);
		}
	return rv;
}

double randbeta(mfloat_t a, mfloat_t b)
{
  double alpha, max, sample, density;

  alpha = (a-1.0)/(a+b-2.0);
  max = (a-1.0)*log(alpha) + (b-1.0)*log(1.0-alpha); max = exp(max);
  while(1)
  {
    sample = ((mfloat_t)rand())/((mfloat_t)RAND_MAX);
    density = (a-1.0)*log(sample) + (b-1.0)*log(1.0-sample); 
	density = exp(density);
    density = density/max;
    if(density > 1) { exit(1); }
    if(density >= ((mfloat_t)rand())/((mfloat_t)RAND_MAX)) return sample;
  }
}

MatrixXd randbeta(const muint_t n, const muint_t m, mfloat_t a, mfloat_t b)
{
	MatrixXd rv(n,m);
	for (muint_t i=0;i<n;i++)
		for(muint_t j=0;j<m;j++)
		{
			rv(i,j) = randbeta(a,b);
		}
	return rv;
}

MatrixXd BaldingNichols(muint_t N, muint_t M, mfloat_t mafmin=0.1, mfloat_t FST=0.005, bool standardize=true)
{
	MatrixXd res = MatrixXd(N,M);
	for (muint_t m = 0; m<M;++m)
	{
		mfloat_t maf = mafmin + (1.0-2.0*mafmin)*((mfloat_t)rand())/((mfloat_t)RAND_MAX);			
		mfloat_t a = maf*(1.0-FST)/FST; 
		mfloat_t b = (1.0-maf)*(1.0-FST)/FST;
		mfloat_t maf1 = randbeta(a,b);
		mfloat_t maf2 = randbeta(a,b);
		mfloat_t MAF = 0.5*maf1 + 0.5*maf2;
		for(muint_t n=0; n<N; ++n)
		{
			mfloat_t mafthis;
			if(n<N/2) mafthis = maf1;
			else mafthis = maf2;
			muint_t geno = 0;
			if(((mfloat_t)rand())/((mfloat_t)RAND_MAX) < mafthis) geno += 1;
			if(((mfloat_t)rand())/((mfloat_t)RAND_MAX) < mafthis) geno += 1;
			if(standardize){
				res(n,m)=(((mfloat_t)geno) - 2.0*MAF)/sqrt(2.0*MAF*(1.0-MAF));
			}
			else
			{
				res(n,m)=(muint_t)geno;
			}
		}
	}
	return res;
}

}
