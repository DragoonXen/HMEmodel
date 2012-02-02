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

protected:
	size_t parameters_count_;
};

} /* namespace hme_model */
#endif /* HMETREENODE_H_ */
