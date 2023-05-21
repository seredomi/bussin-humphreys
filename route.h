#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include <string>
#include <set>

#include "stop.h"

class Route {
  public:
  std::string name;
  Route() : name("") {};
  Route(std::string n) : name(n) {};

  // individual day_scheds to be stored in scheds array
  struct day_sched {
    int start;
    int end;
    int freq;
    day_sched() : start(0), end(0), freq(0) {};
    day_sched(std::string s, std::string e, std::string f)
      : start(stoi(s)), end(stoi(e)), freq(stoi(f)) {};
  };
  day_sched scheds[7];  // one for each day of the week

  // individual route_stops to be stored in route_stops set
  struct route_stop {
    Stop stop;
    int time;
    // all the definitions required to use route_stop in a set
    route_stop() : stop(Stop()), time(0) {};
    route_stop(const Stop &s, std::string t) : stop(s), time(stoi(t)) {};
    route_stop(const route_stop& rs) : stop(rs.stop), time(rs.time) {};
    route_stop& operator=(const route_stop& rs) {
      stop = rs.stop;
      time = rs.time;
      return *this;
    }
    bool operator<(const route_stop& rhs) const {
      return time < rhs.time;
    }
  };
  std::set<route_stop> route_stops;
};

std::ostream & operator<<(std::ostream &os, const Route &r) {
  os << r.name << "\n  ";
  for (auto s : r.scheds) {
    os << s.start << ' ' << s.end << ' ' << s.freq << "\n  ";
  }
  os << "\n   ";
  int i = 0;
  for (auto rs : r.route_stops) {
    os << ++i << " " << rs.stop << " @ " << rs.time << "\n   ";
  }
  os << '\n';
  return os;
}

#endif
