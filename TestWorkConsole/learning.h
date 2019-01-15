#pragma once
#ifndef _LEARNING_HPP
#define _LEARNING_HPP


#include <cmath>
#include <cstdlib>
#include <armadillo>
#include <sstream>
#include <string>

using namespace arma;

class Learn
{
public:
	void dataload(const std::string filedir = "re-central_london.txt"), grdesc(void), hypoth(void), square_lr(void);
	Col<float> square_adapt(int sqcoff, Col<float> xnsq, Col<float> xofuse);
	float costf(void);
	std::string renakb(int ls, std::string kb, char exclch = ' ');
	int kbimp(int ls, std::string kb, char exclch);
	long factorial(int n), pascaltri(int param_n, int value_k);

private:
	Mat<float> x, datadex; // The data  and example matrices.
	Col<float> y, theta; // The example results and the parameter vectors.
	int square_coeff; // The number of square processes
};

template <class tpe>

static tpe cint(std::string coutstr)
{
	std::string varstr;
	cout << coutstr;
	std::getline(std::cin, varstr);
	tpe varint;
	std::stringstream(varstr) >> varint;
	return varint;

};
#endif // !_LEARNING_
