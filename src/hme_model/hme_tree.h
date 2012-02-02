/*
 * hme_tree.h
 *
 *  Created on: Feb 2, 2012
 *      Author: dragoon
 */

#ifndef HME_TREE_H_
#define HME_TREE_H_

#include "hme_nodes/hme_tree_node.h"

#include <fstream>

namespace hme_model {

using std::fstream;

class Hme_tree {
public:
	Hme_tree(fstream &load_stream);
	Hme_tree(fstream &load_stream, double learn_speed);
	virtual ~Hme_tree();

	size_t parameters_count();

	void save_model(fstream &save_stream);
	double evaluate_row(double* params);
	void learn(double** params_matrix, double* d_vector, size_t rows_count);

private:
	void init(fstream &load_stream);

	Hme_tree_node *root_node_;
	size_t parameters_count_;
	double learn_speed_;
};

} /* namespace hme_model */
#endif /* HME_TREE_H_ */
