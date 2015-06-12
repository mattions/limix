import unittest
import numpy as np
from limix.core.covar import SQExpCov
from limix.utils.check_grad import mcheck_grad
import scipy as sp

class TestSQExp(unittest.TestCase):
    def setUp(self):
        np.random.seed(1)
        self._X = np.random.randn(10, 5)
        self._cov = SQExpCov(self._X)

    def test_setX_retE(self):
        X1 = self._X
        np.random.seed(2)
        X2 = np.random.randn(10, 5)

        E1 = sp.spatial.distance.pdist(X1,'euclidean')**2
        E1 = sp.spatial.distance.squareform(E1)

        E2 = sp.spatial.distance.pdist(X2,'euclidean')**2
        E2 = sp.spatial.distance.squareform(E2)

        np.testing.assert_almost_equal(E1, self._cov.E())

        self._cov.X = X2
        np.testing.assert_almost_equal(E2, self._cov.E())

    def test_Kgrad(self):

        def func(x, i):
            self._cov.scale = x[i]
            return self._cov.K()

        def grad(x, i):
            self._cov.scale = x[i]
            return self._cov.K_grad_i(0)

        x0 = np.array([self._cov.scale])
        err = mcheck_grad(func, grad, x0)

        np.testing.assert_almost_equal(err, 0.)

        def func(x, i):
            self._cov.length = x[i]
            return self._cov.K()

        def grad(x, i):
            self._cov.scale = x[i]
            return self._cov.K_grad_i(1)

        x0 = np.array([self._cov.scale])
        err = mcheck_grad(func, grad, x0)


if __name__ == '__main__':
    unittest.main()