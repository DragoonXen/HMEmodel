/*
 * hme_tree_expert.h
 *
 *  Created on: Feb 2, 2012
 *      Author: dragoon
 */

#ifndef HME_TREE_EXPERT_H_
#define HME_TREE_EXPERT_H_

#include "hme_tree_node.h"

#include "../perceptron/backpropagation_perceptron.h"

namespace hme_model {

using perceptron::BackpropagationPerceptron;

class Hme_tree_expert: public Hme_tree_node {

public:
	Hme_tree_expert(fstream &load_stream, size_t parameters_count, double leaves_error_multiplier);
	virtual ~Hme_tree_expert();

	void save_model(fstream &save_stream);
	double evaluate_row(double* params);
	double posteriori_probability_calc(double expected_value);
	void adoption(double learn_speed);
	void status_remember();
	void status_recover();

private:
	void init(fstream &load_stream, size_t parameters_count);

	BackpropagationPerceptron *perceptrone;
	double last_y_;
	double error_;
	double leaves_error_multiplier_;
};

} /* namespace hme_model */
#endif /* HME_TREE_EXPERT_H_ */
