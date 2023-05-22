#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include <string>
#include <set>
#include <map>

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

  std::map<int, Stop> time_sorted_stops;
  std::map<Stop, int> id_sorted_stops;
};

std::ostream & operator<<(std::ostream &os, const Route &r) {
  os << r.name << "\n  ";
  for (auto s : r.scheds) {
    os << s.start << ' ' << s.end << ' ' << s.freq << "\n  ";
  }
  os << "\n   ";
  int i = 0;
  for (const auto &[t, s] : r.time_sorted_stops) {
    os << ++i << " " << s << " @ " << t << "\n   ";
  }
  os << '\n';
  return os;
}

#endif
