/* -*- C++ -*- ------------------------------------------------------------
   @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 */

#include <iostream>
#include "crtp_vector.h"

int main()
{
  vector< double, fixed<4> > v;
  vector< double, fixed<4> > w;
  vector< double, fixed<4> > z(w);
  double some_data[4], some_other_data[4];
  vector< double, external<4> > x(some_data);
  vector< double, external<> > y(4, some_other_data);
  x = v;
  v = w;
  y = x;
  for(int i = 0; i < 4; ++ i) std::clog << x[i] << "\n";
  return 0;
}

// -------------------------------------------------------------------------
// vim:ft=cpp
