/**
 *  This file is part of maxflow-lib.
 *
 *  maxflow-lib is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  maxflow-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with maxflow-lib.  If not, see <https://www.gnu.org/licenses/>.
 *
 * @file create_wrapped_gaussian.cpp
 *
 * @brief Simple binary to create a wrapped gaussian simulated data and pipe to
 * stdout.
 *
 * @author Matt Gara
 *
 * @date 2019-08-26
 *
 */

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include <random>
#include <complex>
#include <algorithm>

void linspace(double start, double end, int nsamp, double *out) {

  double dsamp = (end - start) / (nsamp - 1);

  for (int i = 0; i < nsamp; ++i) {
    out[i] = dsamp * i + start;
  }
}

void generate_gaussian(int size, double *out, double gauss_sigma, double noise_sigma, double scale ) {

  int wid = size;
  int hgt = size;

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<>  dist(0.,1.);

  std::vector<double> xx(size);
  std::vector<double> yy(size);

  linspace(-1, 1, size, &xx[0]);
  linspace(-1, 1, size, &yy[0]);

  double factor = 1. / (gauss_sigma*gauss_sigma);

  for (int iy = 0; iy < hgt; ++iy) {
    for (int ix = 0; ix < wid; ++ix) {
      int idx = iy * wid + ix;
      out[idx] = scale*std::exp(- factor * (xx[ix] * xx[ix] + yy[iy] * yy[iy])) + noise_sigma * dist(rng);
    }
  }
}

class WrappingOp {
public:
    double operator() (double x) const {
        return std::arg(std::polar(1.,x));
    }
};


int main(int argc, char *argv[]) {

  if (argc < 5) {
    printf("usage: %s SIZE GAUSSIAN_SIGMA NOISE_SIGMA SCALE\n", argv[0]);
    std::exit(EXIT_SUCCESS);
  }
  int iarg = 1;
  int size = atoi(argv[iarg++]);
  double gauss_sigma = atof(argv[iarg++]);
  double noise_sigma = atof(argv[iarg++]);
  double scale = atof(argv[iarg++]);

  std::vector<double> gauss(size*size);
  generate_gaussian(size,&gauss[0],gauss_sigma,noise_sigma,scale);

  std::transform(gauss.begin(),gauss.end(),gauss.begin(),WrappingOp());

  fwrite(&size,sizeof(int),1,::stdout);
  fwrite(&gauss[0],sizeof(double),gauss.size(),::stdout);


}
