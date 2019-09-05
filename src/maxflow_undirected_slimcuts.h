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

#include <cassert>

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
  typedef UndirectedGraph<nodeid, arcid, cap, flow> BaseGraph;

private:
  typedef std::unordered_map<nodeid, flow> adjacency_map;
  typedef std::vector<adjacency_map> adjacency_list;

  const nodeid SOURCEID, SINKID;

  adjacency_list m_adj;
  std::vector<flow> m_total_cap_at_node;
  std::vector<nodeid> m_super_node;
  std::vector<nodeid> m_new_super_node_id;
  std::vector<bool> m_what_segment;

  void add_to_adj_map(nodeid s, nodeid t, cap c) {
    adjacency_map &map = m_adj[s];
    typename adjacency_map::iterator it = map.find(t);
    if (it != map.end()) { // found, increase
      it->second = c;
    } else { // not found, create
      map.insert(std::make_pair(t, c));
    }
  }

  void init_graph_contraction() {
    m_total_cap_at_node.resize(BaseGraph::m_nnode);
    for (nodeid i = 0; i < BaseGraph::m_nnode; ++i) {
      adjacency_map &lmap = m_adj[i];
      flow total = 0;
      for (auto &it : lmap) {
        total += it.second;
      }
      m_total_cap_at_node[i] = total;
    }
    m_super_node.assign(BaseGraph::m_nnode, 0);
    for (nodeid i = 0; i < BaseGraph::m_nnode; ++i) {
      m_super_node[i] = i;
    }
  }

  void contract_edge(nodeid u, nodeid v) {
    // contract u into v
    m_super_node[u] = v;
    adjacency_map &umap = m_adj[u];
    adjacency_map &vmap = m_adj[v];
    for (auto &it : umap) {
      nodeid t = it.first;
      if (t == v)
        continue;
      //      fprintf(stderr, "curr: %d, rewiring (%d,%d) ->
      //      (%d,%d)\n",u,u,v,t,v);
      flow f = it.second;
      m_adj[t][v] += f;
      m_adj[v][t] += f;
      m_total_cap_at_node[v] += f;
      m_adj[t].erase(u); // get rid of any trace of u
    }
    // u no longer exists
    vmap.erase(u);
    m_total_cap_at_node[u] = 0;
    m_total_cap_at_node[v] -= umap[v];
    umap.clear();
    //    fprintf(stderr," clearing map of %d\n", u);
  }

  bool contract_node(nodeid u) {
    flow total_cap = m_total_cap_at_node[u];
    adjacency_map &umap = m_adj[u];
    for (auto &it : umap) {
      nodeid v = it.first;
      flow f = it.second;
      if (f > total_cap - f) {
        contract_edge(std::min(u, v), std::max(u, v));
        //        fprintf(stderr,"contracting edge (%d,%d)\n",std::min(u, v),
        //        std::max(u, v));
        return true;
      }
    }
    return false;
  }

  void contract_graph() {
    init_graph_contraction();
    nodeid num_contracted = 0;
    bool change;
    do {
      change = false;
      for (nodeid id = 0; id < BaseGraph::m_nnode - 2; ++id) {
        if (m_super_node[id] == id) { // is a supernode
          bool success = contract_node(id);
          change |= success;
          if (success) {
            num_contracted++;
          }
        }
      }
    } while (change);

    fprintf(stderr, "number contracted: %d\n", num_contracted);
    nodeid num_supernode = 0;
    for (nodeid id = 0; id < BaseGraph::m_nnode; ++id) {
      if (m_super_node[id] == id) { // is a supernode
        num_supernode++;
      }
    }

    fprintf(stderr, "number supernode: %d\n", num_supernode);
    fprintf(stderr, "number original: %d\n", BaseGraph::m_nnode);
  }

  nodeid count_supernodes() const {
    nodeid num_supernode = 0;
    for (nodeid id = 0; id < BaseGraph::m_nnode; ++id) {
      if (m_super_node[id] == id) { // is a supernode
        num_supernode++;
      }
    }
    return num_supernode;
  }

  arcid count_arcs() const {
    arcid num_arc = 0;
    for (nodeid u = 0; u < BaseGraph::m_nnode - 2; ++u) {
      const adjacency_map &umap = m_adj[u];
      for (auto &it : umap) {
        nodeid v = it.first;
        if (u < v) {
          num_arc++;
        }
      }
    }
    return num_arc;
  }

  void resolve_super_nodes() {
    for (nodeid u = 0; u < BaseGraph::m_nnode - 2; ++u) {
      nodeid v = m_super_node[u];
      while (v != m_super_node[v]) {
        v = m_super_node[v];
      }
      m_super_node[u] = v;
    }
  }

  void set_new_super_node_id() {
    m_new_super_node_id.assign(BaseGraph::m_nnode, -1);
    nodeid super_node_count = 0;
    for (nodeid u = 0; u < BaseGraph::m_nnode - 2; ++u) {
      if (m_super_node[u] == u) { // is a supernode
        m_new_super_node_id[u] = super_node_count++;
      } else {
        if (m_adj[u].size() > 0) {
          //      fprintf(stderr, "non-supernode with edges: %d\n", u);
          assert(false);
        }
      }
    }
  }

  void simplify_st_arcs() {
    adjacency_map &smap = m_adj[SOURCEID];
    smap.erase(SINKID);
    adjacency_map &tmap = m_adj[SINKID];
    tmap.erase(SOURCEID);
    for (nodeid u = 0; u < BaseGraph::m_nnode - 2; ++u) {
      adjacency_map &umap = m_adj[u];
      if (umap.find(SOURCEID) != umap.end() &&
          umap.find(SINKID) != umap.end()) {
        flow &sflow = umap[SOURCEID];
        flow &tflow = umap[SINKID];
        if (sflow > tflow) { // source wins
          sflow -= tflow;
          smap[u] -= tflow;
          umap.erase(SINKID);
          tmap.erase(u);
        } else if (sflow < tflow) { // sink wins
          tflow -= sflow;
          tmap[u] -= sflow;
          umap.erase(SOURCEID);
          smap.erase(u);
        } else { // erase everthing
          umap.erase(SOURCEID);
          smap.erase(u);
          umap.erase(SINKID);
          tmap.erase(u);
        }
      }
    }
  }

  void add_arcs(GraphMaxflow &g) const {
    for (nodeid u = 0; u < BaseGraph::m_nnode - 2; ++u) {
      const adjacency_map &umap = m_adj[u];
      for (auto &it : umap) {
        nodeid v = it.first;
        flow f = it.second;
        if (u < v && (v < BaseGraph::m_nnode - 2)) {
          g.add_arc(m_new_super_node_id[u], m_new_super_node_id[v], f, f);
        }
      }
    }
    { // add source capacities
      const adjacency_map &smap = m_adj[SOURCEID];
      for (auto &it : smap) {
        nodeid v = it.first;
        flow f = it.second;
        g.set_tweights(m_new_super_node_id[v], f, 0);
      }
    }
    { // add sink capacities
      const adjacency_map &smap = m_adj[SINKID];
      for (auto &it : smap) {
        nodeid v = it.first;
        flow f = it.second;
        g.set_tweights(m_new_super_node_id[v], 0, f);
      }
    }
  }

  void get_what_segments(GraphMaxflow &g) {
    m_what_segment.assign(BaseGraph::m_nnode - 2, false);
    for (nodeid u = 0; u < BaseGraph::m_nnode - 2; ++u) {
      nodeid sn = m_super_node[u];
      if (sn == SINKID) {
        m_what_segment[u] = true;
      } else if (sn == SOURCEID) {
        m_what_segment[u] = false;
      } else {
        nodeid nu = m_new_super_node_id[sn];
        m_what_segment[u] = g.what_segment(nu);
      }
    }
  }

public:
  UndirectedGraphSlimCuts(nodeid nnode)
      : BaseGraph(nnode + 2), m_adj(BaseGraph::m_nnode),
        SOURCEID(BaseGraph::m_nnode - 2), SINKID(BaseGraph::m_nnode - 1) {}

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

  //  void test_contraction() { contract_graph(); }

  flow maxflow() {

    contract_graph();
    resolve_super_nodes();

    nodeid num_node = count_supernodes() - 2; // don't count source/sink
    if (!num_node) {
      m_what_segment.assign(BaseGraph::m_nnode - 2, false);
      return 0;
    }
    arcid num_arc = count_arcs();


    // sets new ids to use that are collapsed to contigious in a range
    set_new_super_node_id();

    simplify_st_arcs();

    GraphMaxflow graph(num_node, num_arc);
    add_arcs(graph);
    flow f = graph.maxflow();

    get_what_segments(graph);

    return f;
  }

  bool what_segment(nodeid s) { return m_what_segment[s]; }
};

} // namespace maxflowlib

#endif // MAXFLOW_UNDIRECTED_SLIMCUTS_H
