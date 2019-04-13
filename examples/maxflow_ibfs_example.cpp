#include "maxflow_bk.h"
#include <iostream>

int main () {

  maxflowlib::GraphBk<> g(2,1);
  g.add_tweights( 0,   /* capacities */  1, 5 );
  g.add_tweights( 1,   /* capacities */  2, 6 );
  g.add_arc( 0, 1,    /* capacities */  3, 4 );

  int flow = g.maxflow();
  std::cout << " the flow computed was: " << flow << std::endl;

  return 0;
}
