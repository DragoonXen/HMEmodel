/*
 * hmetreenode.h
 *
 *  Created on: Feb 2, 2012
 *      Author: dragoon
 */

#ifndef HMETREENODE_H_
#define HMETREENODE_H_

#include <fstream>

namespace hme_model {

using std::fstream;

class Hme_tree_node {
public:
	Hme_tree_node(size_t parameters_count);
	virtual ~Hme_tree_node();

	void virtual save_model(fstream &save_stream) = 0;
	double virtual evaluate_row(double* params) = 0;
	double virtual posteriori_probability_calc(double expected_value) = 0;
	void virtual adoption(double learn_speed) = 0;
	void virtual status_remember() = 0;
	void virtual status_recover() = 0;

protected:
	double mul(double* arr1, double* arr2);
	size_t parameters_count_;
};

} /* namespace hme_model */
#endif /* HMETREENODE_H_ */
