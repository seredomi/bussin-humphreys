#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include <string>
#include <set>

#include "stop.h"

class Route {
  public:
  std::string name;
  int first;
  int last;
  int freq;
  Route();

  struct route_stop {
    Stop stop;
    int time;
    
    // all the definitions required to use route_stop in a set
    route_stop() : stop(Stop()), time(0) {};
    route_stop(Stop s, int t) : stop(s), time(t) {};
    route_stop(const route_stop& rs) : stop(rs.stop), time(rs.time) {};
    route_stop& operator=(const route_stop& rs) {
      stop = rs.stop;
      time = rs.time;
      return *this;
    }
    bool operator<(const route_stop& rs) const {
      return time < rs.time;
    }
  };

  // set of stops on the route
  std::set<route_stop> route_stops;
};

#endif
