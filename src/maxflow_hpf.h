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
 * @file maxflow_hpf.h
 *
 * @brief Implementation of maxflow interface using the HPF algorithm
 *
 * @author Matt Gara
 *
 * @date 2019-04-15
 *
 */
#ifndef MAXFLOWLIB_MAXFLOW_HPF_H
#define MAXFLOWLIB_MAXFLOW_HPF_H

#include "algorithms/hpf/pseudo.h"
#include "maxflow.h"
#include <stdexcept>

namespace maxflowlib {

template <typename _nodeid = int, typename _arcid = int, typename _cap = int,
          typename _flow = int>
class GraphHPF {};

template <>
class GraphHPF<int, int, int, int> : public Graph<int, int, int, int> {

public:
  typedef Graph<int, int, int, int> BaseGraph;
  typedef BaseGraph::nodeid nodeid;
  typedef BaseGraph::arcid arcid;
  typedef BaseGraph::cap cap;
  typedef BaseGraph::flow flow;

private:
  bool m_inited_graph;
  bool m_pseudoflow_computed;
  bool m_use_pseudoflow_for_maxflow;

public:
  /**
   * @brief GraphHPF class constructor
   *
   * @param nnode number of nodes in the graph
   * @param narc  number of arcs in the graph
   */
  GraphHPF(nodeid nnode, arcid narc, bool use_pseudoflow_for_maxflow = true)
      : BaseGraph(nnode, narc), m_inited_graph(false),
        m_pseudoflow_computed(false),
        m_use_pseudoflow_for_maxflow(use_pseudoflow_for_maxflow) {
    allocateGraph(nnode, narc);
  }

  /**
   * @brief Adds an arc to the residual graph (also adds the residual (reverse)
   * arc)
   *
   * @param s source node
   * @param t target node
   * @param fcap capacity of forward arc
   * @param rcap capacity of reverse arc
   */
  void add_arc(nodeid s, nodeid t, cap fcap, cap rcap) {
    if (m_inited_graph) {
      throw std::logic_error("Initialized HPF graph: add_arc called.");
    }
    ::add_arc(s, t, fcap, rcap);
  }

  /**
   * @brief Adds source and sink connection to node
   *
   * @param s node
   * @param scap capacity of arc source -> node
   * @param tcap capacity of arc node -> sink
   */
  virtual void set_tweights(nodeid s, cap scap, cap tcap) {
    if (m_inited_graph) {
      throw std::logic_error("Initialized HPF graph: set_tweights called.");
    }
    ::set_tweights(s, scap, tcap);
  }

  flow pseudoflow() {
    if (!m_inited_graph) {
      ::initializeGraph();
      m_inited_graph = true;
    }
    flow mincut = ::pseudoflow();
    m_pseudoflow_computed = true;
    return mincut;
  }

  /**
   * @brief Compute the maxflow
   *
   * @return the maxflow
   */
  flow maxflow() {
    if ( m_use_pseudoflow_for_maxflow && !m_pseudoflow_computed ) {
      return pseudoflow();
    }
    if (!m_pseudoflow_computed) {
      pseudoflow();
    }
    return ::maxflow_from_pseudoflow();
  }

  /**
   * @brief Return which segment a node belongs to in the minimum cut
   *
   * @param s the node
   *
   * @return either 0 - indicates source segment or 1 - indicates sink segment
   */
  bool what_segment(nodeid s) { return ::what_segment(s); }
};

} // namespace maxflowlib

#endif
