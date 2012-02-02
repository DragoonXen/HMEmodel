/*
 * hme_tree.cpp
 *
 *  Created on: Feb 2, 2012
 *      Author: dragoon
 */

#include "hme_tree.h"

#include "hme_nodes/hme_tree_expert.h"
#include "hme_nodes/hme_tree_gateway.h"

namespace hme_model {

Hme_tree::Hme_tree(fstream &load_stream) {
	init(load_stream);
}

Hme_tree::~Hme_tree() {
	delete (root_node_);
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

} /* namespace hme_model */
