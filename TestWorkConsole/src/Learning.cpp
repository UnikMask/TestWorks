#include "learning.h"
#include <cstdlib>
#include <fstream>

// using namespace System;
using namespace std;
using namespace arma;

long long
Learn::factorial(int n) // function to calculate factorials - hope it works
{
  long returnval = 1;
  for (int i = 1; i <= n; i++) {
    returnval *= i;
  }
  return returnval;
}

inline long long
Learn::pascaltri(int param_n,
                 int value_k) // Based on the Pascale triangle - use for later
{
  return factorial(param_n) /
         (factorial(value_k) * factorial(param_n - value_k));
}

inline double Learn::costf() // Cost function algorithm
{
  uword m = y.n_rows;
  double cost = sum(((x * theta) - y) % ((x * theta) - y)) / (2 * m);
  return cost;
}

void Learn::grdesc() // The Gradient descent algorithm.
{
  int nmrp, i;
  double lrncf;
  nmrp = cint<int>("Repetitions : ");
  cout << "\n";
  lrncf = cint<double>("Learning Coefficient : ");
  cout << "\n";
  Col<double> theta_t;

  theta_t = theta;
  theta -= lrncf * (1 / double(y.n_rows)) * x.t() * (x * theta - y);
  for (i = 0; i < theta.n_elem; i++) {
    while (abs(theta_t(i)) < abs(theta(i))) {
      lrncf = lrncf / 10;
      theta = theta_t;
      theta -= lrncf * (1 / double(y.n_rows)) * x.t() * (x * theta - y);
    }
  }
  for (i = 0; i < nmrp; i++) {
    theta -= lrncf * (1 / double(y.n_rows)) * x.t() * (x * theta - y);
  }
  cout << " new optimized parameters : "
       << "\n"
       << theta << "\n New cost: " << costf()
       << "\n Adapted Learning Coefficient: " << lrncf;
  return;
}

void Learn::feature_scale() {
  rowvec maxes = zeros<rowvec>(x.n_cols);
  for (size_t i = 0; i < x.n_cols; i++) {
    maxes(i) = x.col(i).max();
  }
  for (size_t i = 0; i < x.n_cols; i++) {
    x.col(i) = x.col(i) / maxes(i);
  }
  y /= y.max();
}

void Learn::hypoth() // The hypothesis function to predict the cost of real
                     // estate.
{
  Col<double> xs(datadex.n_cols, fill::zeros);
  /*xs(0) = cint<double>("Constructed surface: ");
  xs(1) = 10 * cint<double>("Bathrooms: ");
  xs(2) = 10 * cint<double>("Bedrooms: ");
  xs(3) = 10 * cint<double>("Floor concerned (if it is an appartment, = to the
  first floor concerned, else = 0.) : ");*/
  xs(0) = 1;
  for (size_t i = 0; i < datadex.n_cols - 1; i++) {
    std::string typ_string = "Choose Element ";
    stringstream convert;
    convert << (i + 1);
    typ_string.append(convert.str());
    typ_string.append(":");
    xs(i + 1) = cint<double>(typ_string);
  }
  for (size_t i = 0; i < datadex.n_cols - 1; i++) {
    xs(i + 1) /= datadex.col(i + 1).max();
  }
  xs = square_adapt(square_coeff, xs); // Later...
  double rslt =
      sum(xs.t() * theta) * datadex.col(0).max(); // The predicted price.
  cout << "The price of the real estate concerned should be: " << rslt;
  return;
}

void Learn::dataload(
    const string filedir) // Load the datadex matrix, and initialize the
                          // necessary components for the predictions.
{
  ifstream datafle;
  string lnar;
  int matx, maty, k = 0;
  datafle.open(filedir);
  getline(datafle, lnar);

  // Initialization of the data matrix.
  stringstream(renakb(0, lnar, ':')) >> maty;
  stringstream(renakb(kbimp(0, lnar, ':'), lnar)) >> matx;
  datadex.set_size(matx, maty);

  // Loop system to read text files.
  while (getline(datafle, lnar)) {
    int i = 0;
    for (int j = 0; renakb(i, lnar, ',') != "" && renakb(i, lnar, '#') != "";
         j++) {
      double exex = 0;
      stringstream(renakb(i, lnar, ',')) >> exex;
      datadex(k, j) = exex;
      i = kbimp(i, lnar, ',');
    }
    k += 1;
  }
  datafle.close();

  // Initialize and set up x, y, and theta.
  x.set_size(datadex.n_rows, datadex.n_cols);
  y.set_size(datadex.n_rows);
  x(span(0, datadex.n_rows - 1), span(1, datadex.n_cols - 1)) =
      datadex(span(0, datadex.n_rows - 1), span(1, datadex.n_cols - 1));
  x.col(0).fill(1);
  y = vectorise(datadex.col(0));
  theta.set_size(x.n_cols);
  theta.randn();
  square_coeff = 0;
  cout << "Applying feature scaling...";
  this->feature_scale();
  cout << "\r"
       << "Data, examples, and parameters are initialized for " << filedir
       << ". " << flush;
}

void Learn::check_oos(string filedir) {
  ifstream datafle;
  string lnar;
  int matx, maty, k = 0;
  datafle.open(filedir);
  getline(datafle, lnar);

  // Initialization of the data matrix.
  stringstream(renakb(0, lnar, ':')) >> maty;
  stringstream(renakb(kbimp(0, lnar, ':'), lnar)) >> matx;
  Mat<double> hypoth_dat(matx, maty);
  while (getline(datafle, lnar)) {
    int i = 0;
    for (int j = 0; renakb(i, lnar, ',') != "" && renakb(i, lnar, '#') != "";
         j++) {
      double exex = 0;
      stringstream(renakb(i, lnar, ',')) >> exex;
      hypoth_dat(k, j) = exex;
      i = kbimp(i, lnar, ',');
    }
    k++;
  }
  datafle.close();
  for (size_t i = 0; i < hypoth_dat.n_cols; i++) {
    double max = hypoth_dat.col(i).max();
    hypoth_dat.col(i) = hypoth_dat.col(i) / max;
  }
  Mat<double> vec_params(hypoth_dat.n_rows, hypoth_dat.n_cols);
  vec_params.col(0).fill(1);
  vec_params(span(0, hypoth_dat.n_rows - 1), span(1, hypoth_dat.n_cols - 1)) =
      hypoth_dat(span(0, hypoth_dat.n_rows - 1),
                 span(1, hypoth_dat.n_cols - 1));
  vec_params = square_adapt(square_coeff, vec_params);

  Col<double> vec_rslts = vec_params * theta * datadex.col(0).max();
  cout << vec_rslts;
  vec_rslts = vec_rslts / (vec_rslts % vec_rslts);
  cout << "The average precision of the data is of "
       << (sum(vec_rslts % hypoth_dat.col(0)) / hypoth_dat.col(0).n_rows) * 100
       << "%.";
}

void Learn::square_lr() // Function to build a square function to fit the
                        // samples.
{
  Mat<double> *temp_x = new Mat<double>;
  *temp_x = x;
  x.set_size(x.n_rows,
             theta.n_elem * theta.n_elem - pascaltri(theta.n_elem, 2));
  theta.set_size(theta.n_rows * theta.n_rows - pascaltri(theta.n_elem, 2));

  // Those for loops develop the square of the original function. for 1
  // parameter, (x0 + x1)^2 = x0^2 + x1^2 + 2x1x0.
  //
  x(span(0, temp_x->n_rows - 1), span(0, temp_x->n_cols - 1)) =
      *temp_x % *temp_x;
  size_t extr = 0;
  for (size_t i = 0; i < x.n_cols - 1; i++) {
    for (size_t k = i; k < temp_x->n_cols - 1; k++) {
      x.col(temp_x->n_cols + extr) = 2 * (temp_x->col(i) % temp_x->col(k + 1));
      extr++;
    }
  }

  // Assign random numbers to the weights.
  delete temp_x;
  theta.randn();
  square_coeff++;
}

Col<double> Learn::square_adapt(
    int sqcoff, Col<double> xnsq) // This function adapts the user's input to
                                  // the degree of the matrice's polynomial
{
  Col<double> *xrefsq = new Col<double>;
  for (size_t i = 0; i < sqcoff; i++) {
    *xrefsq = xnsq;
    xnsq.set_size(xrefsq->n_rows * xrefsq->n_rows -
                  pascaltri(xrefsq->n_rows, 2));

    // Those for loops develop the square of the original function. for 1
    // parameter, (x0 + x1)^2 = x0^2 + x1^2 + 2x1x0.
    //
    xnsq(span(0, xrefsq->n_rows - 1)) = *xrefsq % *xrefsq;
    size_t extr = 0;
    for (size_t k = 0; k < xnsq.n_rows - 1; k++) {
      for (size_t l = k; l < xrefsq->n_rows - 1; l++) {
        xnsq.row(xrefsq->n_rows + extr) =
            2 * (xrefsq->row(k) % xrefsq->row(l + 1));
        extr++;
      }
    }

    // Assign random numbers to the weights.
  }
  delete xrefsq;
  return xnsq;
}

Mat<double> Learn::square_adapt(int sqcoff, Mat<double> xnsq) {
  Mat<double> *xrefsq = new Mat<double>;
  for (size_t i = 0; i < sqcoff; i++) {
    *xrefsq = xnsq;
    xnsq.set_size(xnsq.n_rows, xrefsq->n_cols * xrefsq->n_cols -
                                   pascaltri(xrefsq->n_cols, 2));

    // Those for loops develop the square of the original function. for 1
    // parameter, (x0 + x1)^2 = x0^2 + x1^2 + 2x1x0.
    //
    xnsq(span(0, xrefsq->n_rows - 1), span(0, xrefsq->n_cols - 1)) =
        *xrefsq % *xrefsq;
    size_t extr = 0;
    for (size_t k = 0; k < xnsq.n_rows - 1; k++) {
      for (size_t l = k; l < xrefsq->n_rows - 1; l++) {
        xnsq.col(xrefsq->n_cols + extr) =
            2 * (xrefsq->col(k) % xrefsq->col(l + 1));
        extr++;
      }
    }

    // Assign random numbers to the weights.
  }
  delete xrefsq;
  return xnsq;
}

std::string
Learn::renakb(int ls, std::string kb,
              char exclch) // Approximate duplicate of the cmdkb in CmdCall.
{
  char cmdchar[1024]; // The memory allocation size has to be a constant, so a
                      // high number is better against shady users, but a lower
                      // number favors memory use.
  int i = ls;
  strncpy(cmdchar, kb.c_str(), sizeof(cmdchar));
  string kwstr = "";
  while (cmdchar[i] != '\0' && cmdchar[i] != exclch) {
    kwstr += cmdchar[i];
    ++i;
  }
  return kwstr;
}

int Learn::kbimp(
    int ls, std::string kb,
    char exclch) // Function dependent on renakb, gives the location of the next
                 // key word to analyse with renakb.
{
  char cmdchar[1024]; // The memory allocation size has to be a constant, so a
                      // high number is better against shady users, but a lower
                      // number favors memory use.
  int i = ls;
  strncpy(cmdchar, kb.c_str(), sizeof(cmdchar)); // See the kbcmd in CmdCall.cpp
  do {
    i++;
  } while (cmdchar[i] != '\0' && cmdchar[i - 1] != exclch);
  return i;
}
