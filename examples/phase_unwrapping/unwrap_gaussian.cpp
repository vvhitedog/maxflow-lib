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
 * @file unwrap_gaussian.cpp
 *
 * @brief A binary that unwraps a gaussian produced by
 * `create_wrapped_gaussian.cpp`.
 *
 * @author Matt Gara
 *
 * @date 2019-08-26
 *
 */

#include <complex>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "maxflow.h"
#include "maxflow_bk.h"
#include "maxflow_hpf.h"
#include "maxflow_ibfs.h"
#include "maxflow_undirected.h"
#include "maxflow_undirected_slimcuts.h"
#include "util/timer.h"

//#define USE_DIRECTED

template <typename vector> void create_unwrapping_grid(int size, vector &arc) {
  for (int y = 0; y < size; ++y) {
    for (int x = 0; x < size; ++x) {
      int id0 = y * size + x;
      if (x < size - 1) {
        int id1 = id0 + 1;
        arc.push_back(id0);
        arc.push_back(id1);
      }
      if (y < size - 1) {
        int id1 = id0 + size;
        arc.push_back(id0);
        arc.push_back(id1);
      }
    }
  }
}

template <typename int_vector, typename real_vector>
void create_unwrapping_problem(const real_vector &wr, const int_vector &arcs,
                               int_vector &ambigs) {
  size_t narc = arcs.size() / 2;
  for (size_t iarc = 0; iarc < narc; ++iarc) {
    int s = arcs[2 * iarc + 0];
    int t = arcs[2 * iarc + 1];
    double dphi = wr[t] - wr[s];
    double est = std::arg(std::polar(1., dphi));
    int ambig = -std::round((dphi - est) / (2 * M_PI));
    ambigs.push_back(ambig);
  }
}

template <typename int_vector, typename Graph>
void solve_unwrapping_problem(int npt, const int_vector &arcs,
                              const int_vector &ambig, int_vector &x) {
  x.resize(npt, 0);

  size_t narc = arcs.size() / 2;

  int iter = 0;
  const int max_iter = 200;
  while (iter++ < max_iter) {
    util::Timer timer_setup, timer_maxflow;
    timer_setup.tic();
#ifdef USE_DIRECTED
    Graph graph(npt, narc);
#else
    Graph graph(npt);
#endif
    int_vector tweights(npt, 0);
    for (size_t iarc = 0; iarc < narc; ++iarc) {
      int s = arcs[2 * iarc + 0];
      int t = arcs[2 * iarc + 1];
      int amb = ambig[iarc];
      int shifted_amb = amb + x[s] - x[t];
      if (shifted_amb < 0) {
        std::swap(s, t);
        shifted_amb = -shifted_amb;
      }
      if (shifted_amb == 0) {
        int wgt = 100 + (std::rand() % 10000); // random
//        int wgt = 1; // random
#ifdef USE_DIRECTED
        graph.add_arc(s, t, wgt, wgt );
#else
        graph.add_arc(s, t, wgt);
#endif
      } else if (shifted_amb > 0) {
        tweights[s] += 1;
        tweights[t] -= 1;
      }
    }
    for (int i = 0; i < npt; ++i) {
      int tw = tweights[i];
      if (std::abs(tw) > 1)
        //        fprintf(stderr,"tw=%d\n",tw);
        if (tw > 0) {
          graph.set_tweights(i, tw, 0);
        } else if (tw < 0) {
          graph.set_tweights(i, 0, -tw);
        }
    }
    //    graph.test_contraction();
    timer_setup.toc();
    timer_maxflow.tic();
    long mf = graph.maxflow();
    timer_maxflow.toc();
    fprintf(stderr, "mf=%ld\n", mf);
    fprintf(stderr, "setup time: %f, maxflow timer: %f\n",
            timer_setup.elapsed_seconds(), timer_maxflow.elapsed_seconds());
    bool something_source = false;
    bool something_sink = false;
    for (int i = 0; i < npt; ++i) {
      if (graph.what_segment(i) == 1) {
        x[i] += 1;
        something_sink = true;
      } else {
        something_source = true;
      }
    }
    bool something_changed = something_sink && something_source;
    if (!something_changed) {
      break;
    }
  }
}

int main(int argc, char *argv[]) {

  size_t read;

  int size;
  std::vector<double> gauss;
  if ((read = fread(&size, sizeof(int), 1, ::stdin)) < 1) {
    fprintf(stderr, "Failed to read size from stream.\n");
    std::exit(EXIT_FAILURE);
  }
  gauss.resize(size * size);
  if ((read = fread(&gauss[0], sizeof(double), size * size, ::stdin)) <
      size * size) {
    fprintf(stderr, "Failed to read right amount of data from stream.\n");
    std::exit(EXIT_FAILURE);
  }

  typedef std::vector<int> int_vector;
  typedef std::vector<double> real_vector;

  int_vector arcs;
  int_vector ambigs;
  int_vector x;

  int npt = size * size;

  create_unwrapping_grid(size, arcs);
  create_unwrapping_problem(gauss, arcs, ambigs);

#ifdef USE_DIRECTED
    typedef  maxflowlib::GraphBK<int,int,int,int> GraphType;
#else
  typedef maxflowlib::UndirectedGraphSlimCuts<
      maxflowlib::GraphBK<int, int, int, int>>
      GraphType;
#endif

  solve_unwrapping_problem<int_vector, GraphType>(npt, arcs, ambigs, x);

  real_vector unw(npt);
  for (int i = 0; i < npt; ++i) {
    unw[i] += gauss[i] + 2 * M_PI * x[i];
  }

  fwrite(&size, sizeof(int), 1, ::stdout);
  fwrite(&unw[0], sizeof(double), unw.size(), ::stdout);
}
