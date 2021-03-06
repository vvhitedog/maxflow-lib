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
 * @file maxflow_ibfs.h
 *
 * @brief Implementation of maxflow interface using the IBFS algorithm (with
 * excess scaling)
 *
 * @author Matt Gara
 *
 * @date 2019-04-13
 *
 */
#ifndef MAXFLOWLIB_MAXFLOW_IBFS_H
#define MAXFLOWLIB_MAXFLOW_IBFS_H

#include "algorithms/ibfs/ibfs.h"
#include "maxflow.h"
#include <stdexcept>

namespace maxflowlib {

template <typename _nodeid = int, typename _arcid = int, typename _cap = int,
          typename _flow = int>
class GraphIBFS {};

template <>
class GraphIBFS<int, int, int, int> : public Graph<int, int, int, int> {

public:
  using GraphImplType = ::IBFSGraph;
  typedef Graph<int, int, int, int> BaseGraph;
  typedef GraphImplType GraphImpl;
  typedef BaseGraph::nodeid nodeid;
  typedef BaseGraph::arcid arcid;
  typedef BaseGraph::cap cap;
  typedef BaseGraph::flow flow;

private:
  GraphImpl m_graph;

public:
  /**
   * @brief GraphIBFS class constructor
   *
   * @param nnode number of nodes in the graph
   * @param narc  number of arcs in the graph
   */
  GraphIBFS(nodeid nnode, arcid narc)
      : BaseGraph(nnode, narc), m_graph(::IBFSGraph::IB_INIT_FAST) {
    m_graph.initSize(nnode, narc);
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
    m_graph.addEdge(s, t, fcap, rcap);
  }

  /**
   * @brief Adds source and sink connection to node
   *
   * @param s node
   * @param scap capacity of arc source -> node
   * @param tcap capacity of arc node -> sink
   */
  void set_tweights(nodeid s, cap scap, cap tcap) {
    m_graph.addNode(s, scap, tcap);
  }

  /**
   * @brief Compute the maxflow
   *
   * @return the maxflow
   */
  flow maxflow() {
    m_graph.initGraph();
    return m_graph.computeMaxFlow();
  }

  /**
   * @brief Return which segment a node belongs to in the minimum cut
   *
   * @param s the node
   *
   * @return either 0 - indicates source segment or 1 - indicates sink segment
   */
  bool what_segment(nodeid s) { return m_graph.what_segment(s); }
};

} // namespace maxflowlib

#endif
