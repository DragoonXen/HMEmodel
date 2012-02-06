/*
 * hme_tree_expert.cpp
 *
 *  Created on: Feb 2, 2012
 *      Author: dragoon
 */

#include "hme_tree_expert.h"

#include <assert.h>
#include <iostream>
#include <math.h>

namespace hme_model {

Hme_tree_expert::Hme_tree_expert(fstream &load_stream, size_t parameters_count, double leaves_error_multiplier) :
		Hme_tree_node(parameters_count) {
	W_ = new double[parameters_count];
	leaves_error_multiplier_ = leaves_error_multiplier;
	init(load_stream, parameters_count);
}

Hme_tree_expert::~Hme_tree_expert() {
	delete[] W_;
}

void Hme_tree_expert::init(fstream &load_stream, size_t parameters_count) {
	for (size_t i = 0; i != parameters_count; i++) {
		load_stream.read((char*) &W_[i], sizeof(W_[i]));
	}
}

void Hme_tree_expert::save_model(fstream &save_stream) {
	bool is_leaf = true;
	save_stream.write((char*) &is_leaf, sizeof(is_leaf));
	for (size_t i = 0; i != parameters_count_; i++) {
		save_stream.write((char*) &W_[i], sizeof(W_[i]));
	}
}

double Hme_tree_expert::evaluate_row(double* params) {
	last_y_ = 0;
	for (size_t i = 0; i != parameters_count_; i++) {
		last_y_ += W_[i] * params[i];
	}
	return last_y_;
}

double Hme_tree_expert::posteriori_probability_calc(double expected_value) {
	error_ = (expected_value - last_y_) * leaves_error_multiplier_;
	double ret = pow(M_E, -error_ * error_ / 2.0);
	if (ret < 1e-16) {
		return 1e-16;
	}
	return ret;
}

void Hme_tree_expert::adoption(double* params, double learn_speed) {
	double tmp = learn_speed * error_;
	for (size_t i = 0; i != parameters_count_; i++) {
		W_[i] += params[i] * tmp;
	}
}

} /* namespace hme_model */
