/*
 * hme_tree_gateway.h
 *
 *  Created on: Feb 2, 2012
 *      Author: dragoon
 */

#ifndef HME_TREE_GATEWAY_H_
#define HME_TREE_GATEWAY_H_

#include "hme_tree_node.h"

#include "../perceptron/backpropagation_perceptron.h"

namespace hme_model {

using perceptron::BackpropagationPerceptron;

class Hme_tree_gateway: public Hme_tree_node {
public:
	Hme_tree_gateway(fstream &load_stream, size_t parameters_count, double leaves_error_multiplier);
	virtual ~Hme_tree_gateway();

	void save_model(fstream &save_stream);
	double evaluate_row(double* params);
	double posteriori_probability_calc(double expected_value);
	void adoption(double learn_speed);
	void status_remember();
	void status_recover();

private:
	void init(fstream &load_stream, size_t parameters_count, double leaves_error_multiplier);

	BackpropagationPerceptron *perceptrone;
	Hme_tree_node *left_child_;
	Hme_tree_node *right_child_;
	double left_priori_probability_;
	double right_priori_probability_;
	double left_posteriori_probability_;
	double right_posteriori_probability_;

};

} /* namespace hme_model */
#endif /* HME_TREE_GATEWAY_H_ */
