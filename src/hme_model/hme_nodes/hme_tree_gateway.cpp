/*
 * hme_tree_gateway.cpp
 *
 *  Created on: Feb 2, 2012
 *      Author: dragoon
 */

#include "hme_tree_gateway.h"
#include "hme_tree_expert.h"

#include <assert.h>

namespace hme_model {

Hme_tree_gateway::Hme_tree_gateway(fstream &load_stream, size_t parameters_count,
		double leaves_error_multiplier) :
		Hme_tree_node(parameters_count) {
	init(load_stream, parameters_count, leaves_error_multiplier);
}

Hme_tree_gateway::~Hme_tree_gateway() {
	delete (perceptrone);
	delete (left_child_);
	delete (right_child_);
}

void Hme_tree_gateway::init(fstream &load_stream, size_t parameters_count,
		double leaves_error_multiplier) {
	perceptrone = new BackpropagationPerceptron(load_stream, 1.0, 0);

	bool child_is_leaf = false;
	load_stream.read((char*) &child_is_leaf, sizeof(child_is_leaf));
	if (child_is_leaf) {
		left_child_ = new Hme_tree_expert(load_stream, parameters_count, leaves_error_multiplier);
	} else {
		left_child_ = new Hme_tree_gateway(load_stream, parameters_count, leaves_error_multiplier);
	}

	load_stream.read((char*) &child_is_leaf, sizeof(child_is_leaf));
	if (child_is_leaf) {
		right_child_ = new Hme_tree_expert(load_stream, parameters_count, leaves_error_multiplier);
	} else {
		right_child_ = new Hme_tree_gateway(load_stream, parameters_count, leaves_error_multiplier);
	}
}

void Hme_tree_gateway::save_model(fstream &save_stream) {
	bool is_leaf = false;
	save_stream.write((char*) &is_leaf, sizeof(is_leaf));
	perceptrone->save(save_stream);
	left_child_->save_model(save_stream);
	right_child_->save_model(save_stream);
}

double Hme_tree_gateway::evaluate_row(double* params) {
	double left_child_evaluation = left_child_->evaluate_row(params);
	double right_child_evaluation = right_child_->evaluate_row(params);

	double* val = perceptrone->evaluate(params);

	left_priori_probability_ = val[0];
	right_priori_probability_ = 1.0 - val[0];

	delete[] val;

	double ret_value = left_child_evaluation * left_priori_probability_
			+ right_child_evaluation * right_priori_probability_;
	return ret_value;
}

double Hme_tree_gateway::posteriori_probability_calc(double expected_value) {
	double z_left = left_child_->posteriori_probability_calc(expected_value);
	double z_right = right_child_->posteriori_probability_calc(expected_value);

	double tmp = left_priori_probability_ * z_left + right_priori_probability_ * z_right;
	left_posteriori_probability_ = left_priori_probability_ * z_left / tmp;
	assert(left_posteriori_probability_ == left_posteriori_probability_);
	right_posteriori_probability_ = right_priori_probability_ * z_right / tmp;
	assert(right_posteriori_probability_ == right_posteriori_probability_);
	return tmp;
}

void Hme_tree_gateway::adoption(double learn_speed) {
	double left_probability_error = (left_posteriori_probability_ - left_priori_probability_)
			* learn_speed;
	assert(left_probability_error == left_probability_error);

	perceptrone->teach_by_errors_row(&left_probability_error);

	left_child_->adoption(left_posteriori_probability_ * learn_speed);
	right_child_->adoption(right_posteriori_probability_ * learn_speed);
}

void Hme_tree_gateway::status_remember() {
	perceptrone->status_remember();
	left_child_->status_remember();
	right_child_->status_remember();
}
void Hme_tree_gateway::status_recover() {
	perceptrone->status_recover();
	left_child_->status_recover();
	right_child_->status_recover();
}

} /* namespace hme_model */
