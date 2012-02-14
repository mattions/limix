import sys
sys.path.append('./..')
sys.path.append('./../../../pygp')


import gpmix
import pygp.covar.linear as lin
import pygp.likelihood as LIK
from pygp.gp import gp_base,gplvm,kronecker_gplvm,gplvm_ard
from pygp.covar import linear,se, noise, combinators, fixed
#import pygp.covar.gradcheck as GC
import pygp.covar.combinators as comb
import pygp.optimize.optimize_base as opt
import scipy as SP
import pdb
import time
import logging as LG


if __name__ == '__main__':
    LG.basicConfig(level=LG.INFO)
    #
    #1. simulate data from a linear PCA model
    #note, these data are truely independent across rows, so the whole gplvm with kronecker is a bit pointless....

    if 1:
        N = 100
        D = 20
        NS = 10

        SP.random.seed(1)
        ir = SP.random.permutation(NS)[0]

        S = SP.random.randn(N,NS)
        W = SP.random.randn(1,D)

        Y = SP.dot(S[:,ir:ir+1],W)
        Y += 0.1*SP.random.randn(N,D)


    if 1:
        covar_c = gpmix.CFixedCF(SP.eye(D))
        covar_r = gpmix.CFixedCF(SP.eye(N))
        Xr      = SP.zeros([N,0])
        Xc      = SP.zeros([D,0])

        gp = gpmix.CGPkronecker(covar_r,covar_c)
        gp.setX_r(Xr)
        gp.setX_c(Xc)
        gp.setY(Y)
        
        params = gpmix.CGPHyperParams()
        params["covar_r"] = SP.zeros([covar_r.getNumberParams()])
        params["covar_c"] = SP.zeros([covar_c.getNumberParams()])
        params["lik"] = SP.log([0.1])

        gp.setParams(params)
        opt=gpmix.CGPopt(gp)

        #opt.opt()
        #opt.gradCheck()

        opt_params = gpmix.CGPHyperParams()
        #opt_params["lik"] = params["lik"]
        gp.setParams(opt_params)
        

        #lmm object
        lmm = gpmix.CGPLMM(gp)
        lmm.setSNPs(S)
        lmm.setCovs(SP.ones([N,1]))
        lmm.setPheno(Y)
        A = SP.ones([2,D])
        A0 = SP.ones([1,D])
        lmm.setA(A)
        lmm.setA0(A0)
        lmm.process()
        pv= lmm.getPv()
