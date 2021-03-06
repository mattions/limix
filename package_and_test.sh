#!/bin/bash

python setup.py bdist_wheel
virtualenv --python=python venv-for-wheel
. venv-for-wheel/bin/activate
pushd dist
pip install --upgrade numpy
pip install --upgrade h5py
pip install --upgrade scikit-learn
pip install --upgrade wheel
pip install --pre --upgrade --find-links . limix
popd
python -c "import limix; limix.test()"
python -c "import test_limix; test_limix.test()"
