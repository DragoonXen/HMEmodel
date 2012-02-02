/*
 * hme_tree.cpp
 *
 *  Created on: Feb 2, 2012
 *      Author: dragoon
 */

#include "hme_tree.h"

#include "hme_nodes/hme_tree_expert.h"
#include "hme_nodes/hme_tree_gateway.h"

#include <algorithm>
#include <assert.h>
#include <limits>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

namespace hme_model {

using std::random_shuffle;

Hme_tree::Hme_tree(fstream &load_stream, double learn_speed) {
	learn_speed_ = learn_speed;
	init(load_stream);
}

Hme_tree::Hme_tree(fstream &load_stream) {
	learn_speed_ = 0.01;
	init(load_stream);
}

Hme_tree::~Hme_tree() {
	delete (root_node_);
}

size_t Hme_tree::parameters_count() {
	return parameters_count_;
}

void Hme_tree::init(fstream &load_stream) {
	load_stream.read((char*) &parameters_count_, sizeof(parameters_count_));
	bool root_is_leaf = false;
	load_stream.read((char*) &root_is_leaf, sizeof(root_is_leaf));
	if (root_is_leaf) {
		root_node_ = new Hme_tree_expert(load_stream, parameters_count_);
	} else {
		root_node_ = new Hme_tree_gateway(load_stream, parameters_count_);
	}
}

void Hme_tree::save_model(fstream &save_stream) {
	save_stream.write((char*) &parameters_count_, sizeof(parameters_count_));
	root_node_->save_model(save_stream);
}

double Hme_tree::evaluate_row(double* params) {
	return root_node_->evaluate_row(params);
}

void Hme_tree::learn(double** params_matrix, double* d_vector, size_t rows_count) {
	size_t row_numbers[rows_count];
	for (size_t i = 0; i != rows_count; i++) {
		row_numbers[i] = i;
	}

	srand(time(NULL));
	random_shuffle(row_numbers, row_numbers + rows_count);

	size_t valid_rows_count = rows_count / 10;
	size_t train_rows_count = rows_count - valid_rows_count;

	//copy train params matrix && result vector
	double** train_params_matrix = new double*[train_rows_count];
	double train_d_vector[train_rows_count];
	for (size_t i = 0; i != train_rows_count; i++) {
		train_params_matrix[i] = params_matrix[row_numbers[i]];
		train_d_vector[i] = d_vector[row_numbers[i]];
	}

	//copy validation params matrix && result vector
	double** valid_params_matrix = new double*[valid_rows_count];
	double valid_d_vector[valid_rows_count];
	for (size_t i = 0; i != valid_rows_count; i++) {
		valid_params_matrix[i] = params_matrix[row_numbers[i + train_rows_count]];
		valid_d_vector[i] = d_vector[row_numbers[i + train_rows_count]];
	}

	double best_sum_sqr_difference = std::numeric_limits<double>::max();

	size_t iteration = 0;
	size_t bad_iterations = 0;
	while (true) {
		++iteration;
		for (size_t i = 0; i != train_rows_count; i++) {
			row_numbers[i] = i;
		}
		random_shuffle(row_numbers, row_numbers + train_rows_count);
		for (size_t i = 0; i != train_rows_count; i++) {
			double eval = evaluate_row(train_params_matrix[row_numbers[i]]);
			root_node_->posteriori_probability_calc(train_d_vector[row_numbers[i]]);
			root_node_->adoption(train_params_matrix[row_numbers[i]], learn_speed_);
//#define test_hme_tree
#ifdef test_hme_tree
			eval = fabs(eval - train_d_vector[row_numbers[i]]);
			double post_eval = fabs(
					evaluate_row(train_params_matrix[row_numbers[i]])
							- train_d_vector[row_numbers[i]]);
			std::cout << "eval: " << eval << ", post eval: " << post_eval << std::endl;
			assert(eval>=post_eval);
#endif
		}

		double sum_sqr_difference = .0;
		for (size_t i = 0; i != train_rows_count; i++) {
			double tmp = evaluate_row(train_params_matrix[i]) - train_d_vector[i];
			sum_sqr_difference += tmp * tmp;
		}
		std::cout << "iteration #" << iteration << ", train sqr sum difference (per one): "
				<< sum_sqr_difference / train_rows_count;

		sum_sqr_difference = .0;
		for (size_t i = 0; i != valid_rows_count; i++) {
			double tmp = evaluate_row(valid_params_matrix[i]) - valid_d_vector[i];
			sum_sqr_difference += tmp * tmp;
		}
		std::cout << ", valid sqr sum difference (per one): "
				<< sum_sqr_difference / valid_rows_count << std::endl;

		if (best_sum_sqr_difference > sum_sqr_difference) {
			best_sum_sqr_difference = sum_sqr_difference;
			bad_iterations = 0;
		} else {
			bad_iterations++;
			if (bad_iterations > 100) {
				break;
			}
		}
	}
	std::cout << "best valid sqr sum difference (per one): "
			<< best_sum_sqr_difference / valid_rows_count << std::endl;

	delete[] train_params_matrix;
	delete[] valid_params_matrix;
}

} /* namespace hme_model */
