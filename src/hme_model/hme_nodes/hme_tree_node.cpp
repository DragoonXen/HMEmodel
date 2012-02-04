/*
 * hmetreenode.cpp
 *
 *  Created on: Feb 2, 2012
 *      Author: dragoon
 */

#include "hme_tree_node.h"

namespace hme_model {

Hme_tree_node::Hme_tree_node(size_t parameters_count) {
	parameters_count_ = parameters_count;
}

Hme_tree_node::~Hme_tree_node() {
}

double Hme_tree_node::mul(double* arr1, double* arr2) {
	double rez = 0;
	for (size_t i = 0; i != parameters_count_; i++) {
		rez += arr1[i] * arr2[i];
	}
	return rez;
}

} /* namespace hme_model */
