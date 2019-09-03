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
 * @file maxflow_undirected.h
 *
 * @brief Main interface for maxflow algorithms on undirected graphs
 *
 * @author Matt Gara
 *
 * @date 2019-08-26
 *
 */
#ifndef MAXFLOW_UNDIRECTED_H
#define MAXFLOW_UNDIRECTED_H

namespace maxflowlib {

template <typename _nodeid, typename _arcid, typename _cap, typename _flow>
class UndirectedGraph {

public:
  typedef _nodeid nodeid;
  typedef _arcid arcid;
  typedef _cap cap;
  typedef _flow flow;

protected:
  nodeid m_nnode;

public:
  /**
   * @brief UndirectedGraph class constructor
   *
   * @param nnode number of nodes in the graph
   */
  UndirectedGraph(nodeid nnode) : m_nnode(nnode) {}

  /**
   * @brief Destructor
   */
  virtual ~UndirectedGraph() {}

  /**
   * @brief Adds an arc to the residual graph
   *
   * @param s source node
   * @param t target node
   * @param cap capacity of arc
   */
  virtual void add_arc(nodeid s, nodeid t, cap c) = 0;

  /**
   * @brief Adds source and sink connection to node
   *
   * @param s node
   * @param scap capacity of arc source -> node
   * @param tcap capacity of arc node -> sink
   */
  virtual void set_tweights(nodeid s, cap scap, cap tcap) = 0;

  /**
   * @brief Compute the maxflow
   *
   * @return the maxflow
   */
  virtual flow maxflow() = 0;

  /**
   * @brief Return which segment a node belongs to in the minimum cut
   *
   * @param s the node
   *
   * @return either 0 - indicates source segment or 1 - indicates sink segment
   */
  virtual bool what_segment(nodeid s) = 0;
};

} // namespace maxflowlib

#endif // MAXFLOW_UNDIRECTED_H
