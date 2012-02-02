/*
 * hme_tree_expert.cpp
 *
 *  Created on: Feb 2, 2012
 *      Author: dragoon
 */

#include "hme_tree_expert.h"

namespace hme_model {

Hme_tree_expert::Hme_tree_expert(fstream &load_stream, size_t parameters_count) :
		Hme_tree_node(parameters_count) {
	W_ = new double[parameters_count];
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
	double rez = 0;
	for (size_t i = 0; i != parameters_count_; i++) {
		rez += W_[i] * params[i];
	}
	return rez;
}

} /* namespace hme_model */
