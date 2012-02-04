#include "hme_model/hme_tree.h"

#include <assert.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using hme_model::Hme_tree;
using std::vector;

int main(int argc, char* argv[]) {
	//tree configuration load
	std::fstream f_load_model("base_model.bin", std::ios_base::binary | std::ios_base::in);
	Hme_tree tree(f_load_model, 0.001);
	f_load_model.close();

	//load learning data
	char line[5000];
	std::ifstream learn_data("data.learn");
	vector<vector<double>*> *input_matrix = new vector<vector<double>*>();
	while (learn_data.getline(line, 5000)) {
		std::stringstream sstr;
		sstr << line;
		vector<double> *row = new vector<double>();
		double tmp;
		while (sstr >> tmp) {
			row->push_back(tmp);
		}
		if (input_matrix->size()) {
			assert(input_matrix->at(0)->size() == row->size());
		}
		input_matrix->push_back(row);
	}
	learn_data.close();

	assert(input_matrix->at(0)->size() == tree.parameters_count());
	size_t rows_count = input_matrix->size();
	double** params_matrix = new double*[rows_count];
	double* d_vector = new double[rows_count];
	for (size_t i = 0; i != rows_count; i++) {
		params_matrix[i] = new double[tree.parameters_count()];
		params_matrix[i][0] = 1.0; //set const parameter at first place
		std::copy(++input_matrix->at(i)->begin(), input_matrix->at(i)->end(), &params_matrix[i][1]);
		d_vector[i] = input_matrix->at(i)->front();
		delete (input_matrix->at(i));
	}
	delete (input_matrix);

	std::cout.setf(std::ios_base::fixed);
	std::cout.precision(6);

	double rez = 0;
	for (size_t i = 0; i != rows_count; i++) {
		double tmp = tree.evaluate_row(params_matrix[i]) - d_vector[i];
		rez += tmp * tmp;
	}
	std::cout << rez / rows_count << std::endl;

	tree.learn(params_matrix, d_vector, rows_count);

	for (size_t i = 0; i != rows_count; i++) {
		delete[] params_matrix[i];
	}
	delete[] d_vector;
	delete[] params_matrix;
	return 0;
}
