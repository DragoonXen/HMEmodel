#include "hme_model/hme_tree.h"

#include <fstream>

using hme_model::Hme_tree;
int main() {
	std::fstream f_load_model("base_model.bin", std::ios_base::binary | std::ios_base::in);
	Hme_tree tree(f_load_model);
	f_load_model.close();

	std::fstream f_save_model("save_model.bin", std::ios_base::binary | std::ios_base::out);
	tree.save_model(f_save_model);
	f_save_model.close();
	return 0;
}
