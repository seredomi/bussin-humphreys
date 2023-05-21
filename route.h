#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include <string>
#include <pair>
#include <set>

#include "stop.h"

class Route {
  std::string name;
  int first;
  int last;
  int freq;

  struct route_stop {
    Stop stop;
    int time;
    
    // all the definitions required to use route_stop in a set
    struct() : stop(Stop()), time(0) {};
    struct(Stop s, int t) : stop(s), time(t) {};
    struct(const struct& rs) : stop(rs.stop), time(rs.time) {};
    struct& operator=(const struct& rs) {
      stop = rs.stop;
      time = rs.time;
      return *this;
    }
    bool operator<(const struct& rs) const {
      return time < rs.time;
    }
  };

  // set of stops on the route
  std::set<route_stop> route_stops;
}

#endif
