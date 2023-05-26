#include <string>
#include <iostream>

#include "stop.h"

using namespace std;

// ctors
Stop::Stop() :
  id(""), name(""), desc(""), lat(0), lon(0) {};
Stop::Stop(string i) :
  id(i), name(""), desc(""), lat(0), lon(0) {};
Stop::Stop(string i, string n, string d, string la, string lo) :
  id(i), name(n), desc(d), lat(stold(la)), lon(stold(lo)) {};

// following are required to make a std::set<Stop>
// copy constructor
Stop::Stop(const Stop& s) {
  id = s.id;
  name = s.name;
  desc = s.desc;
  lat = s.lat;
  lon = s.lon;
}
// assignment operator
Stop& Stop::operator=(const Stop& rhs) {
  id = rhs.id;
  name = rhs.name;
  desc = rhs.desc;
  lat = rhs.lat;
  lon = rhs.lon;
  return *this;
}
// less than operator
bool Stop::operator<(const Stop& rhs) const { return id < rhs.id; }
bool Stop::operator==(const Stop& rhs) const { return id == rhs.id; }
bool Stop::operator!=(const Stop& rhs) const { return id != rhs.id; }

ostream& operator<<(ostream &os, const Stop &s) {
  return os << s.id << ' ' << s.name << ' ' << s.desc << ' ' << s.lat << ' ' << s.lon;
}
