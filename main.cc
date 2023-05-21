#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <sstream>
#include <math.h>

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


set<Stop> make_stops() {
  set<Stop> stops;
  // open stops.csv, construct stops set
  ifstream ifs("stops.csv");
  if (!ifs) {
    cout << "Error opening stops.csv\n";
    return stops;
  }
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
  return stops;
}


vector<Route> make_routes(set<Stop> &stops) {
  vector<Route> routes;
  ifstream ifs("routes.csv");
  if (!ifs) {
    cout << "Error opening routes.csv\n";
    return routes;
  }
  string line;
  // each line in routes.csv is of the form:
  // routename,monstart-end freq, tuestart-end freq, ... sunstart-end freq, stop1 stop1time, stop2 stop2time, ...
  // open routes.csv, construct routes vector
  while (getline(ifs, line)) {
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

          vector<Route> empty; return empty;
        }
        r.route_stops.insert(Route::route_stop(*it, arg.substr(arg.find(" ")+1)));
      }
    }
    routes.push_back(r);
  }
  ifs.close();
  return routes;
}


// Haversine formula
double time_bw_pts(Stop &p1, Stop &p2) {
  double dlat = (p2.lat - p1.lat) * M_PI / 180;
  double dlon = (p2.lon - p1.lon) * M_PI / 180;
  // 12.5 is average walking speed m/km, 6371 is radius of earth in km
  return 2 * 12.5 * 6371 * asin(sqrt(pow(sin((dlat) / 2), 2) + cos(p1.lat) * cos(p2.lat) * pow(sin((dlon) / 2), 2)));
}

 
int main() {

  set<Stop> stops = make_stops();
  if (stops.empty()) return 1;
  vector<Route> routes = make_routes(stops);
  if (routes.empty()) return 1;

  Stop s1 = *stops.find(Stop("PVC-W"));
  Stop s2 = *stops.find(Stop("PG"));

  cout << time_bw_pts(s1, s2) << '\n';



  return 0;
}
