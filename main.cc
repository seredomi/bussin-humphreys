#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <sstream>
#include <regex>

#include "stop.h"
#include "route.h"

using namespace std;

// ---- stop stuff -------
// ctors
Stop::Stop()
  : id(""), name(""), desc(""), lat(0), lon(0) {};
Stop::Stop(string i) : id(i), name(""), desc(""), lat(0), lon(0) {};
Stop::Stop(string i, string n, string d, string la, string lo)
  : id(i), name(n), desc(d), lat(stold(la)), lon(stold(lo)) {};
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

 
int main() {

  cout.precision(20);

  // open stops.csv, construct stops set
  ifstream ifs("stops.csv");
  if (!ifs) {
    cout << "Error opening stops.csv\n";
    return 1;
  }
  set<Stop> stops;
  string line;
  // each line is stops.csv is of the form: id,name,desc,lat,lon
  // for each line in stops.csv, split line by commas, initialize stop, insert in stops set
  while (getline(ifs, line)) {
    stringstream ss(line);
    vector<string> args;
    string arg;
    while (getline(ss, arg, ',')) args.push_back(arg);
    stops.insert(Stop(args[0], args[2], args[1], args[3], args[4]));
  }
  ifs.close();

  // each line in routes.csv is of the form:
  // routename,monstart-end freq, tuestart-end freq, ... sunstart-end freq, stop1 stop1time, stop2 stop2time, ...
  // open routes.csv, construct routes vector
  ifs.open("routes.csv");
  if (!ifs) {
    cout << "Error opening routes.csv\n";
    return 1;
  }
  vector<Route> routes;
  while (getline(ifs, line)) {
    cout << line << '\n';
    stringstream ss(line);
    string arg;
    getline(ss, arg, ',');
    Route r(arg); // initialize route with name
    // add each day's schedule to scheds array
    for (int d = 0; d < 7; ++d) {
      getline(ss, arg, ',');
      // arg is of the form: start-end freq, split and initialize day_sched accordingly
      Route::day_sched sched(arg.substr(0, arg.find("-")),
                      arg.substr(arg.find("-")+1, arg.find(" ")),
                      arg.substr(arg.find(" ")+1));
      r.scheds[d] = sched;
    }
    while (getline(ss, arg, ',')) {
      if (arg != "" && arg != "\n") {
        Stop query(arg.substr(0, arg.find(" "))); 
        auto it = stops.find(query);
        if (it == stops.end()) {
          cout << "Error: " << " route " << r.name << " " << arg.substr(0, arg.find(" ")) << " not found" << '\n';
          return 1;
        }
        r.route_stops.insert(Route::route_stop(*it, arg.substr(arg.find(" ")+1)));
      }
    }
    routes.push_back(r);
  }
  ifs.close();

  for (auto r : routes) cout << r << '\n';

  return 0;
}
