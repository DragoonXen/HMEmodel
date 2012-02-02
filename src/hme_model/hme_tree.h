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
	virtual ~Hme_tree();

	void save_model(fstream &save_stream);

private:
	void init(fstream &load_stream);

	Hme_tree_node *root_node_;
	size_t parameters_count_;
};

} /* namespace hme_model */
#endif /* HME_TREE_H_ */
