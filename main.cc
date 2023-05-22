#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <sstream>
#include <math.h>

#include <chrono>
#include <ctime>

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


set<Stop> make_stops(string filename) {
  set<Stop> stops;
  // open stops.csv, construct stops set
  ifstream ifs(filename);
  if (!ifs) {
    cout << "Error opening " << filename << "\n";
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


vector<Route> make_routes(set<Stop> &stops, string filename) {
  vector<Route> routes;
  ifstream ifs(filename);
  if (!ifs) {
    cout << "Error opening " << filename << "\n";
    return routes;
  }
  string line;
  // each line in routes.csv is of the form:
  // routename,monstart-end freq, tuestart-end freq, ... ,sunstart-end freq,stop1id time,stop2id time, ...
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
      if (true) { // TODO: implement check for trailing commas?
        Stop query(arg.substr(0, arg.find(" "))); 
        auto it = stops.find(query);
        if (it == stops.end()) {
          cout << "Error: " << " route " << r.name << " " << arg.substr(0, arg.find(" ")) << " not found" << '\n';
          vector<Route> empty; return empty;
        }
        r.time_sorted_stops[stoi(arg.substr(arg.find(" ")+1))] = *it;
        r.id_sorted_stops[*it] = stoi(arg.substr(arg.find(" ")+1));
      }
    }
    routes.push_back(r);
  }
  ifs.close();
  return routes;
}


bool valid_route(int day, int curr_time, Stop s, Route &r) {
  return r.id_sorted_stops.find(s) != r.id_sorted_stops.end()
      && r.scheds[day].start + r.id_sorted_stops[s] <= curr_time
      && r.scheds[day].end + r.id_sorted_stops[s] >= curr_time;
}


// use Haversine formula to find distance between stops
double time_bw_pts(Stop &p1, Stop &p2) {
  double dlat = (p2.lat - p1.lat) * M_PI / 180;
  double dlon = (p2.lon - p1.lon) * M_PI / 180;
  // 12.5 is average walking speed m/km, 6371 is radius of earth in km
  return 2 * 12.5 * 6371 *
    asin(sqrt(pow(sin((dlat) / 2), 2) + cos(p1.lat) * cos(p2.lat) * pow(sin((dlon) / 2), 2)));
}

// use modified Dijkstra's algorithm to find shortest path between two stops
set<Stop> find_path(int day, int time, Stop &s1, Stop &s2, set<Stop> all_stops, vector<Route> all_routes) {

  set<Stop> unvisited = all_stops;
  set<Stop> visited;
  set<Stop> neighbors;
  map<Stop, int> times;
  set<Stop> path;

  for (auto s : all_stops) times[s] = 99999;

  Stop current = s1;
  times[current] = time;


  for (int i = 0; i < 6; ++i) {
    cout << all_routes[i].name << " " << boolalpha << valid_route(day, time, s1, all_routes[i]) << '\n';
  }
  
//  while (current != s2) {
//
//    visited.insert(current);
//    unvisited.erase(current);
//
//    // only routes that are open and contain the current stop should be used
//    vector<Route> possibleRoutes;
//    for (auto r : allRoutes)
//      if (r.route_stops.find(Route::route_stop(current)) != r.route_stops.end()
//       && r.scheds[day].start+r.route_stops.find(Route::route_stop(current))->time <= time
//       && r.scheds[day].end+r.route_stops.find(Route::route_stop(current))->time >= time)
//        possibleRoutes.push_back(r);
//  }


  return path;
}




  // auto start = chrono::high_resolution_clock::now();
  // cout << chrono::duration<double, std::milli>(chrono::high_resolution_clock::now() - start).count() << '\n';

 
int main() {

  set<Stop> stops = make_stops("stops.csv");
  if (stops.empty()) return 1;
  vector<Route> routes = make_routes(stops, "routes.csv");
  if (routes.empty()) return 1;
  

  Stop s1 = *stops.find(Stop("H-N"));
  Stop s2 = *stops.find(Stop("PG"));

  find_path(0, 1173, s1, s2, stops, routes);



  return 0;
}
