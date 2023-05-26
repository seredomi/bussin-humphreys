#include <iostream>
#include <string>
#include <set>
#include <map>

#include "route.h"

using namespace std;

Route::Route() : name("") {};
Route::Route(string n) : name(n) {};

Route::day_sched::day_sched() : start(0), end(0), freq(0) {};
Route::day_sched::day_sched(string s, string e, string f) :
  start(stoi(s)), end(stoi(e)), freq(stoi(f)) {};

ostream & operator<<(ostream &os, const Route &r) {
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
