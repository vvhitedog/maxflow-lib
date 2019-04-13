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
 * @file maxflow_bk.h
 *
 * @brief Implementation of maxflow interface using the BK algorithm
 *
 * @author Matt Gara
 *
 * @date 2019-04-13
 *
 */
#ifndef MAXFLOWLIB_MAXFLOW_BK_H

#include "algorithms/bk/graph.h"
#include "maxflow.h"

template <typename cap, typename tcap, typename flow>
using GraphImplType = Graph<cap, tcap, flow>;

namespace maxflowlib {

template <typename _cap, typename _flow>
class GraphBk : public Graph<int, int, _cap, _flow> {

public:
  typedef Graph<int, int, _cap, _flow> BaseGraph;
  typedef GraphImplType<_cap, _cap, _flow> GraphImpl;
  typedef int nodeid;
  typedef int arcid;
  typedef _cap cap;
  typedef _flow flow;

private:
  GraphImpl m_graph;

public:
  /**
   * @brief GraphBk class constructor
   *
   * @param nnode number of nodes in the graph
   * @param narc  number of arcs in the graph
   */
  GraphBk(nodeid nnode, arcid narc)
      : BaseGraph(nnode, narc), m_graph(nnode, narc) {
    m_graph.add_node(BaseGraph::m_nnode);
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
    m_graph.add_edge(s, t, fcap, rcap);
  }

  /**
   * @brief Adds source and sink connection to node
   *
   * @param s node
   * @param scap capacity of arc source -> node
   * @param tcap capacity of arc node -> sink
   */
  virtual void add_tweights(nodeid s, cap scap, cap tcap) {
    m_graph.add_tweights(s, scap, tcap);
  }

  /**
   * @brief Compute the maxflow
   *
   * @return the maxflow
   */
  flow maxflow() { return m_graph.maxflow(); }

  /**
   * @brief Return which segment a node belongs to in the minimum cut
   *
   * @param s the node
   *
   * @return either 0 - indicates source segment or 1 - indicates sink segment
   */
  bool what_segment(nodeid s) {
    return m_graph.what_segment(s) == GraphImpl::SINK;
  }
};

} // namespace maxflowlib

#endif
