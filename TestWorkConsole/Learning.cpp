#include "learning.h"
#include <fstream>
#include <cstdlib>

//using namespace System;
using namespace std;
using namespace arma;

long Learn::factorial(int n) //function to calculate factorials - hope it works
{
	long returnval = 1;
	for(int i = 1; i<=n; i++)
	{
		returnval *= i;
	}
	return returnval;
}

inline long Learn::pascaltri(int param_n, int value_k) //Based on the Pascale triangle - use for later
{
	return factorial(param_n) / (factorial(value_k)*factorial(1 - value_k));
}

inline float Learn::costf() //Cost function algorithm
{
	uword m = y.n_rows;
	float cost = sum(((x * theta) - y) % ((x * theta) - y)) / (2*m);
	return cost;
}

void Learn::grdesc() //The Gradient descent algorithm.
{ 
	int nmrp, i;
	float lrncf;
	nmrp = cint<int>("Repetitions : ");
	cout << "\n";
	lrncf = cint<float>("Learning Coefficient : ");
	cout << "\n";
	Col<float> theta_t;

	theta_t = theta;
	theta -= lrncf * (1 / float(y.n_rows)) * x.t() * (x * theta - y);
	for (i = 0; i < theta.n_elem; i++ ) 
	{
		while (abs(theta_t(i)) < abs(theta(i)))
		{
			lrncf = lrncf / 10;
			theta_t = theta;
			theta -= lrncf * (1 / float(y.n_rows)) * x.t() * (x * theta - y);
		}
	}
	for (i = 0; i < nmrp; i++)
	{
		theta -= lrncf * (1 / float(y.n_rows) ) * x.t() * (x * theta - y);
	}
	cout << " new optimized parameters : " << "\n" << theta << "\n New cost: " << costf() << "\n Adapted Learning Coefficient: " << lrncf;
	return;
}


void Learn::feature_scale()
{
	frowvec maxes = zeros<frowvec>(x.n_cols);
	for (size_t i = 0; i < x.n_cols ; i++)
	{
		maxes(i) = x.col(i).max();
	}
	for (size_t i = 0; i < x.n_cols ; i++)
	{
		x.col(i) = x.col(i) / maxes(i);
	}
	y /= y.max();
}


void Learn::hypoth() //The hypothesis function to predict the cost of real estate.
{
	Col<float> xs(x.n_cols, fill::zeros), xsq(x.n_cols, fill::zeros);
	/*xs(0) = cint<float>("Constructed surface: ");
	xs(1) = 10 * cint<float>("Bathrooms: ");
	xs(2) = 10 * cint<float>("Bedrooms: ");
	xs(3) = 10 * cint<float>("Floor concerned (if it is an appartment, = to the first floor concerned, else = 0.) : ");*/
	for (size_t i = 0; i < x.n_cols; i++)
	{
		std::string typ_string = "Choose Element ";
		stringstream convert;
		convert << (i+1);
		typ_string.append(convert.str());
		typ_string.append(":");
		xs(i) = cint<float>(typ_string);
	}
	for (size_t i = 0; i < x.n_cols; i++)
	{
		xs(i) /= x.col(i).max();
	}
	xsq = xs;
	//xsq = square_adapt(square_coeff, xs, xsq); Later...
	float rslt = sum(xsq.t() * theta) * datadex.col(0).max(); //The predicted price.
	cout << "The price of the real estate concerned should be: " << rslt;
	return;
}

void Learn::dataload(const string filedir) //Load the datadex matrix, and initialize the necessary components for the predictions.
{
	ifstream datafle;
	string lnar;
	int matx, maty, k = 0;
	datafle.open(filedir);
	getline(datafle, lnar);

	//Initialization of the data matrix.
	stringstream(renakb(0, lnar, ':')) >> maty;
	stringstream(renakb(kbimp(0, lnar, ':'), lnar)) >> matx;
	datadex.set_size(matx, maty);

	//Loop system to read text files.
	while (getline(datafle, lnar))
	{
		int i = 0;
		for (int j = 0; renakb(i, lnar,',') != "" && renakb(i, lnar, '#') != ""; j++)
		{
			float exex = 0;
			stringstream(renakb(i, lnar, ',')) >> exex;
			datadex(k, j) = exex;
			i = kbimp(i, lnar, ',');
		}
		k += 1;
	}
	datafle.close();

	//Initialize and set up x, y, and theta.
	x.set_size(datadex.n_rows, datadex.n_cols - 1);
	y.set_size(datadex.n_rows);
	x = datadex(span(0, datadex.n_rows - 1) , span(1, datadex.n_cols - 1));
	y = vectorise(datadex(span(0, datadex.n_rows-1), 0));
	theta.set_size(datadex.n_cols-1);
	theta.randn();
	square_coeff = 0;
	cout << "Applying feature scaling...";
	this->feature_scale();
	cout << "\r" << "Data, examples, and parameters are initialized for " << filedir << ". " << flush;
}

void Learn::square_lr() //Function to build a square function to fit the samples.
{
	x.set_size(theta.n_rows - pascaltri(theta.size() + 1, 3), x.n_cols-1);
	y.set_size(theta.n_rows - pascaltri(theta.size() + 1, 3));
	theta.set_size(theta.n_rows - (theta.size() + 1, 3));

	// Those for loops develop the square of the original function. for 1 parameter, (x0 + x1)^2 = x0^2 + x1^2 + 2x1x0.
	//
	for (size_t i = 0; i< datadex.n_rows-1; i++)
	{
		x(span(0, x.n_cols - 1), i) = datadex(span(1 ,x.n_cols), i) % datadex(span(1, x.n_cols), i);
		y(i) = datadex(0, i) * datadex(0, i);

		for (size_t k = i + 1; k < datadex.n_rows - 2; k++)
		{
			x.col(datadex.n_cols - 2 + k) = 2 * datadex(span(1, x.n_cols), i) % datadex(span(1, x.n_cols), k);
			y(datadex.n_cols - 2 + k) = datadex(0, i) * datadex(0, k);
		}
	}

	//Assign random numbers to the weights.
	theta.randn();
	square_coeff++;
}

Col<float> Learn::square_adapt(int sqcoff, Col<float> xnsq, Col<float> xofuse)
{
	xofuse = xnsq;
	for (size_t i = 0; i < sqcoff; i++)
	{
		xnsq = xofuse;
		xofuse.set_size(xnsq.n_cols - 1 - pascaltri(xnsq.n_cols - 1, 3));
		for (size_t k = 0; k < xnsq.n_cols - 1; k++)
		{
			xofuse(k) = xnsq(k) * xnsq(k);
			for (size_t l = k + 1; l < xnsq.n_cols - 2; l++)
			{
				xofuse(xnsq.n_cols - 1 + l) = xnsq(k) * xnsq(l);
			}
		}
	}
	return xofuse;
}




std::string Learn::renakb(int ls, std::string kb,char exclch)//Approximate duplicate of the cmdkb in CmdCall.
{
	char cmdchar[1024]; //The memory allocation size has to be a constant, so a high number is better against shady users, but a lower number favors memory use.
	int i = ls;
	strncpy_s(cmdchar, kb.c_str(), sizeof(cmdchar));
	string kwstr = "";
	while (cmdchar[i] != '\0' && cmdchar[i] != exclch)
	{
		kwstr += cmdchar[i];
		++i;
	}
	return kwstr;
}

int Learn::kbimp(int ls, std::string kb, char exclch)//Function dependent on renakb, gives the location of the next key word to analyse with renakb.
{
	char cmdchar[1024]; //The memory allocation size has to be a constant, so a high number is better against shady users, but a lower number favors memory use.
	int i = ls;
	strncpy_s(cmdchar, kb.c_str(), sizeof(cmdchar)); //See the kbcmd in CmdCall.cpp
	do 
	{
		i++;
	}while (cmdchar[i] != '\0' && cmdchar[i-1] != exclch);
	return i;
}
