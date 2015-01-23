import numpy as np
import scipy.linalg as la

class LmmKronecker(object):
    pass

    def __init__(self, gp=None):
        '''
        Input:
        forcefullrank   : if True, then the code always computes K and runs cubically
					        (False)
        '''
        self.gp = gp
        self.index_snpterm = self.gp.mean.n_terms
        self._LL_0 = gp.LML()
        
    def LL_snps(self, snps, Asnps=None):
        """
        test snps for association
        """
        #TODO: find a way to evaluate multiple SNPs at once
        nLL_snps = np.ones(snps.shape[1])
        for i_snp in xrange(snps.shape[1]):

            self.gp.mean.addFixedEffect(F=snps[:,i_snp:i_snp+1], A=Asnps,index=self.index_snpterm)
            nLL_snps[i_snp] = self.gp.LML()

        return nLL_snps

def compute_D(S_C, S_R, delta=1.0):
	return 1.0 / (delta + np.outer(S_C, S_R))

def ldet_Kron(S_C, S_R, D=None, delta=1.0):
	"""
	compute the log determinant
	"""
	if D is None:
		D = compute_D(S_C=S_C, S_R=S_R, delta=delta)
	ldet = R * np.log(S_R).sum() + C * np.log(S_C).sum() - np.log(D).sum()
	return ldet


def compute_Kronecker_beta(Y, D, X, A):

    cov_beta = mean.compute_XKX()

    inv_cov_beta =la.inv(cov_beta)
    betas = np.dot(inv_cov_beta,XYA.flatten())
    #chol_beta = la.cho_factor(a=cov_beta, lower=False, overwrite_A=True, check_finite=True)
    #betas = la.cho_solve(chol_beta,b=XYA.flatten(), overwrite_b=False, check_finite=True)

    total_sos = (Y * DY).sum()
    var_expl = (betas * XYA.flatten()).sum()

    sigma2 = (res - var_expl)/(R*C) #change according to REML
    ldet = ldet_Kron(S_C, S_R, D=D, delta=1.0)

    nLL = 0.5 * ( R * C * ( np.log(2.0*np.pi) + log(sigma2) + 1.0) + ldet)

    W=[]
    cum_sum = 0
    for term in xrange(n_terms):
        current_size = X[term].shape[1] * A[term].shape[0]
        W_term = np.reshape(betas[cum_sum:cum_sum+current_size], (X[term].shape[1],A[term].shape[0]), order='C')
        cum_sum += current_size