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
 * @file maxflow_benchmark_dimacs.cpp
 *
 * @brief Benchmark multiple maxflow algorithms on supplied DIMACS inputs
 *
 * @author Matt Gara
 *
 * @date 2019-04-16
 *
 */

#include "maxflow.h"
#include "maxflow_bk.h"
#include "maxflow_hpf.h"
#include "maxflow_ibfs.h"
#include "util/timer.h"
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>

/**
 * @brief Stores arc to be added to graph as a post operation.
 *
 * @tparam Graph the graph type that is added to
 * @tparam dtype data type used for arc (assumes only one)
 */
template <typename Graph, typename dtype = int> class DelayedAddArc {

private:
  dtype m_s, m_t, m_cap, m_rcap;

public:
  /**
   * @brief Create a arc that will be added later
   *
   * @param s source node
   * @param t target node
   * @param cap forward capacity
   * @param rcap reverse capacity
   */
  DelayedAddArc(dtype s, dtype t, dtype cap, dtype rcap)
      : m_s(s), m_t(t), m_cap(cap), m_rcap(rcap) {}

  /**
   * @brief Adds arc to a graph
   *
   * @param g graph to add to
   * @param source source node id
   * @param sink sink node id
   */
  void add_arc(Graph &g, dtype source, dtype sink) const {
    if (m_s == source) {
      g.set_tweights(m_t - 3, m_cap, 0);
    } else if (m_t == sink) {
      g.set_tweights(m_s - 3, 0, m_cap);
    } else {
      g.add_arc(m_s - 3, m_t - 3, m_cap, 0);
    }
  }

  /**
   * @brief Checks to see if arc is a "real" arc or a source/sink arc
   *
   * @param source source id
   * @param sink sink id
   *
   * @return  true if it is a source/sink arc
   */
  bool is_source_or_sink_arc(dtype source, dtype sink) const {
    return m_s == source || m_t == sink;
  }
};

/**
 * @brief Reads a DIMACs format file into a Graph defined by template
 *
 * @tparam Graph the type of graph
 * @param filename the DIMACs format file
 *
 * @return a pointer to an allocated Graph
 */
template <typename Graph> Graph *read_dimacs(const std::string &filename) {
  Graph *g;
  const int line_length = 1024;
  char line[line_length];
  int n, m, s, t, cap, source, sink;
  char c;
  std::vector<DelayedAddArc<Graph> > arcs_to_add;
  FILE *stream = nullptr;
  source = -1;
  sink = -1;
  if (!(stream = fopen(filename.c_str(), "r"))) {
    std::string err_msg = "failed to open file for reading: " + filename;
    std::perror(err_msg.c_str());
    std::exit(EXIT_FAILURE);
  }
  while (fgets(line, line_length, stream)) {
    switch (*line) {
    case 'p':
      if (sscanf(line, "%*c %*s %d %d", &n, &m) != 2) {
        std::string err_msg =
            "p line is malformed in DIMACS file:" + filename + "\n";
        std::fprintf(stderr, err_msg.c_str());
        std::exit(EXIT_FAILURE);
      }
      break;
    case 'a':
      if (source == -1 || sink == -1) {
        std::string err_msg =
            "'a' line occured beforce setting source/sink in DIMACS file:" +
            filename + "\n";
        std::fprintf(stderr, err_msg.c_str());
        std::exit(EXIT_FAILURE);
      }
      if (sscanf(line, "%*c %d %d %d", &s, &t, &cap) != 3) {
        std::string err_msg =
            "'a' line is malformed in DIMACS file:" + filename + "\n";
        std::fprintf(stderr, err_msg.c_str());
        std::exit(EXIT_FAILURE);
      }
      if (t == source || s == sink || (s == source && t == sink)) {
        std::string err_msg =
            "specified source or sink as target or source node incorrectly:" +
            filename + "\n";
        std::fprintf(stderr, err_msg.c_str());
        std::exit(EXIT_FAILURE);
      }
      if ( cap > 0 ) {
        arcs_to_add.emplace_back(s, t, cap, 0);
      }
      break;
    case 'n':
      if (sscanf(line, "%*c %d %c", &s, &c) != 2) {
        std::string err_msg =
            "'n' line is malformed in DIMACS file:" + filename + "\n";
        std::fprintf(stderr, err_msg.c_str());
        std::exit(EXIT_FAILURE);
      }
      if (c == 's') {
        if (s != 1) {
          std::string err_msg = "'n' line specified source as something else "
                                "than 1, currently unsupported:" +
                                filename + "\n";
          std::fprintf(stderr, err_msg.c_str());
          std::exit(EXIT_FAILURE);
        }
        source = s;
      }
      if (c == 't') {
        if (s != 2) {
          std::string err_msg = "'n' line specified sink as something else "
                                "than 2, currently unsupported:" +
                                filename + "\n";
          std::fprintf(stderr, err_msg.c_str());
          std::exit(EXIT_FAILURE);
        }
        sink = s;
      }
      break;
    case 'c':
      // comment
      break;
    default:
      break;
    }
  }
  fclose(stream);
  // Count the number of real arcs
  int narcs = 0;
  for (DelayedAddArc<Graph> &a : arcs_to_add) {
    if (!a.is_source_or_sink_arc(source, sink)) {
      narcs++;
    }
  }
  // Create a graph with the correct number of arcs,
  // add the arcs appropriately as either source/sink
  // arcs or s,t arcs
  g = new Graph(n - 2, narcs);
  for (DelayedAddArc<Graph> &a : arcs_to_add) {
    a.add_arc(*g, source, sink);
  }
  return g;
}

/**
 * @brief Compute the maxflow given a DIMACs file
 *
 * @tparam Graph the type of Graph/algorithm to use
 * @param filename the DIMACs file
 *
 * @return the maxflow value
 */
template <typename Graph> int compute_maxflow(const std::string &filename) {
  int flow;
  auto *g = read_dimacs<Graph>(filename);
  util::Timer maxflow_timer;
  maxflow_timer.tic();
  flow = g->maxflow();
  maxflow_timer.toc();
  delete g;
  printf("%s: (MAXFLOW) : %d (TIME) : %lfs\n", __PRETTY_FUNCTION__, flow,
         maxflow_timer.elapsed_seconds());
  return flow;
}

/**
 * @brief Compute a rough benchmark on several different maxflow algorithms
 *
 * @param filename DIMACs file for which to compute maxflow
 */
void benchmark_maxflow(const std::string &filename) {

  using maxflowlib::GraphBK;
  using maxflowlib::GraphIBFS;
  using maxflowlib::GraphHPF;

  int bk_maxflow = compute_maxflow<GraphBK<int, int, int, int> >(filename);
  int ibfs_maxflow = compute_maxflow<GraphIBFS<int, int, int, int> >(filename);
  int hpf_maxflow = compute_maxflow<GraphHPF<int, int, int, int> >(filename);
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("usage: %s DIMACS_MAXFLOW_FILE\n", argv[0]);
    std::exit(EXIT_SUCCESS);
  }

  benchmark_maxflow(argv[1]);
}
