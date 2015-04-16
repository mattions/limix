import sys
sys.path.insert(0,'./../../..')
from limix.core.utils.cached import *
import scipy as sp
from covariance import covariance
import pdb
import scipy.linalg as LA
import warnings

import logging as LG

class cov_reml(covariance):
    """
    abstract super class for all implementations of covariance functions
    """
    def __init__(self,gp):
        self.gp = gp 
        gp.register(self.clear_all)
        self.dim = gp.mean.b.shape[0]
        self._calcNumberParams()

    #####################
    # Params handling
    #####################
    def setParams(self,params):
        warnings.warn('Read-only covariance type')
        
    def getParams(self,params):
        return self.gp.covar.getParams()
        
    def _calcNumberParams(self):
        self.n_params = self.gp.covar.getNumberParams() 

    #####################
    # Cached
    #####################
    @cached
    def K(self):
        return self.gp.Areml_K() 

    @cached
    def K_grad_i(self,i):
        return self.gp.Areml_K_grad_i(i) 

