# Copyright(c) 2014, The LIMIX developers (Christoph Lippert, Paolo Francesco Casale, Oliver Stegle)
# All rights reserved.
#
# LIMIX is provided under a 2-clause BSD license.
# See license.txt for the complete license.

import data_new as DATA
import scipy as SP

data_file = "./example_data/kruglyak.hdf5"

geno_reader  = DATA.geno_reader_hdf5(data_file)
pheno_reader = DATA.pheno_reader_hdf5(data_file)

data = DATA.QTLData(geno_reader=geno_reader,pheno_reader=pheno_reader)