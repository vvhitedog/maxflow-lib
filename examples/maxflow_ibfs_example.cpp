#include "maxflow_ibfs.h"
#include <iostream>

int main () {

  maxflowlib::GraphIBFS<> g(2,1);
  g.set_tweights( 0,   /* capacities */  4, 0 );
  g.set_tweights( 1,   /* capacities */  0, 6 );
  g.add_arc( 0, 1,    /* capacities */  3, 0 );

  int flow = g.maxflow();
  std::cout << " the flow computed was: " << flow << std::endl;

  for ( int i = 0; i < 2; ++i ) {
    std::cout << " what segment for node " << i << ": " << g.what_segment(i)  << std::endl;
  }

  return 0;
}
