/*
 * hme_tree_gateway.cpp
 *
 *  Created on: Feb 2, 2012
 *      Author: dragoon
 */

#include "hme_tree_gateway.h"
#include "hme_tree_expert.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace hme_model {

Hme_tree_gateway::Hme_tree_gateway(fstream &load_stream, size_t parameters_count) :
		Hme_tree_node(parameters_count) {
	A_ = new double*[2];
	A_[0] = new double[parameters_count];
	A_[1] = new double[parameters_count];
	init(load_stream, parameters_count);
}

Hme_tree_gateway::~Hme_tree_gateway() {
	delete[] A_[0];
	delete[] A_[1];
	delete[] A_;
	delete (left_child_);
	delete (right_child_);
}

void Hme_tree_gateway::init(fstream &load_stream, size_t parameters_count) {
	for (size_t j = 0; j != 2; j++) {
		for (size_t i = 0; i != parameters_count; i++) {
			load_stream.read((char*) &A_[j][i], sizeof(A_[j][i]));
		}
	}
	bool child_is_leaf = false;
	load_stream.read((char*) &child_is_leaf, sizeof(child_is_leaf));
	if (child_is_leaf) {
		left_child_ = new Hme_tree_expert(load_stream, parameters_count);
	} else {
		left_child_ = new Hme_tree_gateway(load_stream, parameters_count);
	}

	load_stream.read((char*) &child_is_leaf, sizeof(child_is_leaf));
	if (child_is_leaf) {
		right_child_ = new Hme_tree_expert(load_stream, parameters_count);
	} else {
		right_child_ = new Hme_tree_gateway(load_stream, parameters_count);
	}
}

void Hme_tree_gateway::save_model(fstream &save_stream) {
	bool is_leaf = false;
	save_stream.write((char*) &is_leaf, sizeof(is_leaf));
	for (size_t j = 0; j != 2; j++) {
		for (size_t i = 0; i != parameters_count_; i++) {
			save_stream.write((char*) &A_[j][i], sizeof(A_[j][i]));
		}
	}
	left_child_->save_model(save_stream);
	right_child_->save_model(save_stream);
}

double Hme_tree_gateway::evaluate_row(double* params) {
	double left_child_evaluation = left_child_->evaluate_row(params);
	double right_child_evaluation = right_child_->evaluate_row(params);

	double left_child_plausibility = 0;
	double right_child_plausibility = 0;
	for (size_t i = 0; i != parameters_count_; i++) {
		left_child_plausibility += A_[0][i] * params[i];
		right_child_plausibility += A_[1][i] * params[i];
	}

	double left_decision_power = pow(M_E, left_child_plausibility);
	double right_decision_power = pow(M_E, right_child_plausibility);

	g_left_ = left_decision_power / (left_decision_power + right_decision_power);
	g_right_ = right_decision_power / (left_decision_power + right_decision_power);

	return left_child_evaluation * g_left_ + right_child_evaluation * g_right_;
}

} /* namespace hme_model */
