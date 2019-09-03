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
 * @file maxflow_undirected_slimcuts.h
 *
 * @brief Implementation of maxflow interface in the undirected case using
 * Slimcuts and any of the directed maxflow implementations.
 *
 * @author Matt Gara
 *
 * @date 2019-08-26
 *
 */
#ifndef MAXFLOW_UNDIRECTED_SLIMCUTS_H
#define MAXFLOW_UNDIRECTED_SLIMCUTS_H

#include "maxflow_undirected.h"
#include <stdexcept>

#include <unordered_map>
#include <vector>

namespace maxflowlib {

template <typename GraphMaxflow>
class UndirectedGraphSlimCuts
    : public UndirectedGraph<
          typename GraphMaxflow::nodeid, typename GraphMaxflow::arcid,
          typename GraphMaxflow::cap, typename GraphMaxflow::flow> {
public:
  typedef typename GraphMaxflow::nodeid nodeid;
  typedef typename GraphMaxflow::arcid arcid;
  typedef typename GraphMaxflow::cap cap;
  typedef typename GraphMaxflow::flow flow;

  void add_to_adj_map(nodeid s, nodeid t, cap c) {
    adjacency_map &map = m_adj[s];
    adjacency_map::iterator it = map.find(t);
    if (it != map.end()) { // found, increase
      it->second = c;
    } else { // not found, create
      map.insert(std::make_pair(t, c));
    }
  }

private:
  typedef std::unordered_map<nodeid, flow> adjacency_map;
  typedef std::vector<adjacency_map> adjacency_list;

  const nodeid SOURCEID, SINKID;

  adjacency_list m_adj;

  UndirectedGraphSlimCuts(nodeid nnode)
      : UndirectedGraph(nnode + 2), m_adj(UndirectedGraph::m_nnode),
        SOURCEID(UndirectedGraph::m_nnode - 2),
        SINKID(UndirectedGraph::m_nnode - 1) {}

  void add_arc(nodeid s, nodeid t, cap c) {
    if (c > 0) {
      add_to_adj_map(s, t, c);
      add_to_adj_map(t, s, c);
    }
  }

  void set_tweights(nodeid s, cap scap, cap tcap) {
    if (scap > 0) {
      add_to_adj_map(s, SOURCEID, scap);
      add_to_adj_map(SOURCEID, s, scap);
    }
    if (tcap > 0) {
      add_to_adj_map(s, SINKID, tcap);
      add_to_adj_map(SINKID, s, tcap);
    }
  }
};

} // namespace maxflowlib

#endif // MAXFLOW_UNDIRECTED_SLIMCUTS_H
