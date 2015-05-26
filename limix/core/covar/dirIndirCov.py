import numpy as np
import scipy as sp
import scipy.linalg as LA
from covar_base import Covariance
from freeform import FreeFormCov 
from limix.core.type.cached import cached
from limix.core.utils.normalization import covar_rescaling_factor
from limix.core.utils.normalization import covar_rescale
import pdb

import logging as LG

class DirIndirCov(Covariance):
    """
    freeform covariance function
    """
    def __init__(self, kinship, design, jitter = 1e-4):
        ff_dim = 2
        self.covff = FreeFormCov(ff_dim, jitter = 1e-4)
        self._K = covar_rescale(kinship)
        self._ZK = sp.dot(design, kinship)
        self._KZ = sp.dot(kinship, design.T)
        self._ZKZ = sp.dot(self._ZK, design.T)
        sf_ZKZ = covar_rescaling_factor(self._ZKZ)
        self._ZK  *= sp.sqrt(sf_ZKZ)
        self._KZ  *= sp.sqrt(sf_ZKZ)
        self._ZKZ *= sf_ZKZ
        Covariance.__init__(self, kinship.shape[0])

    def dirIndirCov_K(self):
        return self.covff.K()

    #####################
    # Properties
    #####################
    @property
    def variance(self):
        return self.covff.variance

    @property
    def correlation(self):
        return self.covff.correlation

    @property
    def variance_ste(self):
        return self.covff.variance_ste

    @property
    def correlation_ste(self):
        return self.covff.correlation_ste

    #####################
    # Params handling
    #####################
    def getParams(self):
        return self.covff.getParams()

    def setParams(self,params):
        self.covff.setParams(params)
        self.clear_all()
        self._notify()

    def _calcNumberParams(self):
        self.n_params = self.covff.getNumberParams() 

    def setCovariance(self,cov):
        """ set hyperparameters from given covariance """
        return self.covff.setCovariance(cov)

    #####################
    # Cached
    #####################
    @cached
    def K(self):
        C = self.covff.K()
        RV  = C[0,0] * self._K
        RV += C[0,1] * (self._KZ + self._ZK)
        RV += C[1,1] * self._ZKZ
        return RV

    @cached
    def K_grad_i(self,i):
        Cgrad = self.covff.K_grad_i(i)
        RV  = Cgrad[0,0] * self._K
        RV += Cgrad[0,1] * (self._KZ + self._ZK)
        RV += Cgrad[1,1] * self._ZKZ
        return RV

    ####################
    # Interpretable Params
    ####################
    def getInterParams(self):
        return self.covff.getInterParams()

    def K_grad_interParam_i(self,i):
        Cgrad = self.covff.K_grad_interParam_i(i)
        RV  = Cgrad[0,0] * self._K
        RV += Cgrad[0,1] * (self._KZ + self._ZK)
        RV += Cgrad[1,1] * self._ZKZ
        return RV

    def setFIinv(self, value):
        self.covff.setFIinv(value)

    def getFIinv(self):
        return self.covff.getFIinv()

if __name__ == '__main__':
    # generate data
    n = 100
    f = 10
    X  = 1.*(sp.rand(n,f)<0.2)
    X -= X.mean(0); X /= X.std(0)
    kinship  = sp.dot(X,X.T)
    kinship /= kinship.diagonal().mean()
    design = sp.zeros((n,n))
    for i in range(n/2):
        design[2*i,2*i+1] = 1
        design[2*i+1,2*i] = 1

    # test covariance
    cov = DirIndirCov(kinship,design)
    cov.setRandomParams()
    print cov.K()
    print cov.K_grad_i(0)
