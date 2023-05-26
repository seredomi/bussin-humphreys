#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include <string>
#include <set>
#include <map>

#include "stop.h"
#include "stop.cc"

class Route {
  public:
  std::string name;
  Route();
  Route(std::string n);

  std::map<Stop, int> id_sorted_stops;      // allows quick .find() by id
  std::map<int, Stop> time_sorted_stops;    // allows iteration along actual route schedule

  // individual day_scheds to be stored in scheds array
  struct day_sched {
    int start;
    int end;
    int freq;
    day_sched();
    day_sched(std::string, std::string, std::string);
  };
  day_sched scheds[7];  // one for each day of the week

};

std::ostream & operator<<(std::ostream &, const Route &);

#endif
