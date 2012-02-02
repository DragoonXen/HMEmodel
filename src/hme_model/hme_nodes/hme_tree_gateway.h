/*
 * hme_tree_gateway.h
 *
 *  Created on: Feb 2, 2012
 *      Author: dragoon
 */

#ifndef HME_TREE_GATEWAY_H_
#define HME_TREE_GATEWAY_H_

#include "hme_tree_node.h"

namespace hme_model {

class Hme_tree_gateway: public Hme_tree_node {
public:
	Hme_tree_gateway(fstream &load_stream, size_t parameters_count);
	virtual ~Hme_tree_gateway();

	void save_model(fstream &save_stream);
	double evaluate_row(double* params);
	double posteriori_probability_calc(double expected_value);
	void adoption(double* params, double learn_speed);

private:
	void init(fstream &load_stream, size_t parameters_count);

	double** A_;
	Hme_tree_node *left_child_;
	Hme_tree_node *right_child_;
	double left_priori_probability_;
	double right_priori_probability_;
	double left_posteriori_probability_;
	double right_posteriori_probability_;
};

} /* namespace hme_model */
#endif /* HME_TREE_GATEWAY_H_ */
