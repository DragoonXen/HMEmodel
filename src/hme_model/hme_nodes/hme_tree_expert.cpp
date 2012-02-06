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

Hme_tree_expert::Hme_tree_expert(fstream &load_stream, size_t parameters_count,
		double leaves_error_multiplier) :
		Hme_tree_node(parameters_count) {
	leaves_error_multiplier_ = leaves_error_multiplier;
	init(load_stream, parameters_count);
}

Hme_tree_expert::~Hme_tree_expert() {
	delete (perceptrone);
}

void Hme_tree_expert::init(fstream &load_stream, size_t parameters_count) {
	perceptrone = new BackpropagationPerceptron(load_stream, 1.0, 0.5);
}

void Hme_tree_expert::save_model(fstream &save_stream) {
	bool is_leaf = true;
	save_stream.write((char*) &is_leaf, sizeof(is_leaf));
	perceptrone->save(save_stream);
}

double Hme_tree_expert::evaluate_row(double* params) {
	double* rez = perceptrone->evaluate(params);
	last_y_ = rez[0];
	delete[] rez;
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

void Hme_tree_expert::adoption(double learn_speed) {
	double tmp = learn_speed * error_;
	perceptrone->teach_by_errors_row(&tmp);
}

void Hme_tree_expert::status_remember() {
	perceptrone->status_remember();
}
void Hme_tree_expert::status_recover() {
	perceptrone->status_recover();
}

} /* namespace hme_model */
