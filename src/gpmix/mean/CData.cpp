/*
 * CData.cpp
 *
 *  Created on: Jan 4, 2012
 *      Author: clippert
 */

#include "CData.h"

namespace gpmix {

CData::CData() : ADataTerm() {
}

CData::CData(MatrixXd& Y) : ADataTerm(Y) {
}

CData::~CData() {
}

} /* namespace gpmix */