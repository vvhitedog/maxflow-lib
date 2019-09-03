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
 * @file py_visualize_gaussian.cpp
 *
 * @brief Generates python code to visualize gaussian from
 * `create_wrapped_gaussian` binary. stdout.
 *
 * @author Matt Gara
 *
 * @date 2019-08-26
 *
 */

#include <cstdio>
#include <cstdlib>
#include <vector>

int main(int argc, char *argv[]) {

    size_t read;

    int size;
    std::vector<double> gauss;
    if ( ( read = fread(&size,sizeof(int),1,::stdin) ) < 1 )  {
        fprintf(stderr,"Failed to read size from stream.\n");
        std::exit(EXIT_FAILURE);
    }
    gauss.resize(size*size);
    if ( ( read = fread(&gauss[0],sizeof(double),size*size,::stdin)) < size*size )  {
        fprintf(stderr,"Failed to read right amount of data from stream.\n");
        std::exit(EXIT_FAILURE);
    }

    // Imports
    fprintf(::stdout,"#!/usr/bin/python\n");
    fprintf(::stdout,"import numpy as np\n");
    fprintf(::stdout,"import sys\n");
    fprintf(::stdout,"from matplotlib import pyplot as plt\n");
    // Get size of data to display/read
    fprintf(::stdout,"size = %d\n",size);
    fprintf(::stdout,"gauss = np.asarray([");
    for (auto &x  : gauss) {
        fprintf(::stdout,"%f,",x);
    }
    fprintf(::stdout,"]).reshape(size,size)\n");
    fprintf(::stdout,"plt.matshow(gauss)\n");
    fprintf(::stdout,"plt.colorbar()\n");
    fprintf(::stdout,"plt.show(block=True)\n");


}
