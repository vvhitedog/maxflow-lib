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
 * @file maxflow.h
 *
 * @brief Main interface for maxflow algorithms
 *
 * @author Matt Gara
 *
 * @date 2019-04-13
 *
 */
#ifndef MAXFLOWLIB_MAXFLOW_H
#define MAXFLOWLIB_MAXFLOW_H

namespace maxflowlib {

template <typename _nodeid, typename _arcid, typename _cap, typename _flow>
class Graph {

public:
  typedef _nodeid nodeid;
  typedef _arcid arcid;
  typedef _cap cap;
  typedef _flow flow;

protected:
  nodeid m_nnode;
  arcid m_narc;

public:
  /**
   * @brief Graph class constructor
   *
   * @param nnode number of nodes in the graph
   * @param narc  number of arcs in the graph
   */
  Graph(nodeid nnode, arcid narc) : m_nnode(nnode), m_narc(narc) {}

  /**
   * @brief Destructor
   */
  virtual ~Graph() {}

  /**
   * @brief Adds an arc to the residual graph (also adds the residual (reverse)
   * arc)
   *
   * @param s source node
   * @param t target node
   * @param fcap capacity of forward arc
   * @param rcap capacity of reverse arc
   */
  virtual void add_arc(nodeid s, nodeid t, cap fcap, cap rcap) = 0;

  /**
   * @brief Adds source and sink connection to node
   *
   * @param s node
   * @param scap capacity of arc source -> node
   * @param tcap capacity of arc node -> sink
   */
  virtual void add_tweights(nodeid s, cap scap, cap tcap) = 0;

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

#endif
