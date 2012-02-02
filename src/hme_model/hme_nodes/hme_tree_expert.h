/*
 * hme_tree_expert.h
 *
 *  Created on: Feb 2, 2012
 *      Author: dragoon
 */

#ifndef HME_TREE_EXPERT_H_
#define HME_TREE_EXPERT_H_

#include "hme_tree_node.h"

namespace hme_model {

class Hme_tree_expert: public Hme_tree_node {
public:
	Hme_tree_expert(fstream &load_stream, size_t parameters_count);
	virtual ~Hme_tree_expert();

	void save_model(fstream &save_stream);
	double evaluate_row(double* params);

private:
	void init(fstream &load_stream, size_t parameters_count);

	double *W_;
};

} /* namespace hme_model */
#endif /* HME_TREE_EXPERT_H_ */
