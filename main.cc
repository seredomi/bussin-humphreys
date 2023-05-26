#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <math.h>
#include <chrono>
#include <ctime>

#include "route.h"
#include "route.cc"
#include "action.h"
#include "action.cc"

using namespace std;

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
          routes.clear(); return routes;
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


// check stop is in route, and that route is running on day and at time
bool valid_route(int day, int curr_time, Stop s, Route &r) {
  return r.id_sorted_stops.find(s) != r.id_sorted_stops.end()
      && r.scheds[day].start + r.id_sorted_stops[s] <= curr_time
      && r.scheds[day].end + r.id_sorted_stops[s] >= curr_time;
}

int next_time(int day, int curr_time, Stop s, Route &r) {

  int dep_time = r.scheds[day].start + r.id_sorted_stops[s.id];
  while (dep_time < curr_time) dep_time += r.scheds[day].freq;
  return dep_time;

}


// use Haversine formula to find walking time between stops
double time_bw_pts(Stop &p1, Stop &p2) {
  double dlat = (p2.lat - p1.lat) * M_PI / 180;
  double dlon = (p2.lon - p1.lon) * M_PI / 180;
  // 12.5 is average walking speed min/km, 6371 is radius of earth in km
  return 2 * 12.5 * 6371 *
    asin(sqrt(pow(sin((dlat) / 2), 2) + cos(p1.lat) * cos(p2.lat) * pow(sin((dlon) / 2), 2)));
}

// use modified Dijkstra's algorithm to find shortest path between two stops
vector<Stop> find_path(int day, int curr_time, Stop &s1, Stop &s2, set<Stop> all_stops, vector<Route> all_routes) {

  set<Stop> unvisited = all_stops;
  set<Stop> visited;
  map<Stop, int> times;
  map<Stop, Stop> prevs;

  vector<Stop> path;

  for (auto s : all_stops) times[s] = 99999;

  Stop current = s1;
  times[current] = curr_time;

  
  cout << "current: " << current.name << '\n';

  while (!unvisited.empty()) {

    if (current == s2) break; // exit if we've reached the destination

    // only routes that are open and contain the current stop should be used
    vector<Route> possible_routes;
    for (auto r : all_routes)
      if (r.id_sorted_stops.find(Stop(current.id)) != r.id_sorted_stops.end())
        possible_routes.push_back(r);

    cout << "num routes: " << possible_routes.size() << '\n';


    // for each route, find the next stop and add it to neighbors
    for (auto r : possible_routes) {
      auto it = r.time_sorted_stops.begin();
      while (it->second != current) ++it;
      int test_dep_time = next_time(day, curr_time, it->second, r);
      // cout << it->first << ' ' << it->second.id << '\n';
      //if (current.id == "MH-W") {
      //  cout << (++it)->second << endl;
      //  for (auto uv : unvisited) cout << uv.id << '\n';
      //}

      
      // if this stop isnt last and the next stop is unvisited
      if (++it != r.time_sorted_stops.end() && unvisited.find(it->second) != unvisited.end()) {
        // cout << "TEST!!\n";
        // calculate time to get to next stop, including waiting
        test_dep_time += (it->first - (--it)->first); ++it;
        //int dep_time = r.scheds[day].start + (--it)->first; ++it;
        //// cout << "dt: " << dep_time << " ct: " << curr_time << '\n';
        //while (dep_time < curr_time) dep_time += r.scheds[day].freq;
        //int tot_time = times[current] + (dep_time - curr_time) + (it->first - (--it)->first); ++it;
        if (test_dep_time < times[it->second]) {
          times[it->second] = test_dep_time;
          prevs[it->second] = current;
        } 
      }
    }


    unvisited.erase(current);
    // cout << "unvisited:\n";
    // for (auto s : unvisited) cout << "  " << s.name << " / " << times[s] << '\n';
    // find unvisited stop with smallest time
    current = *unvisited.begin();
    for (auto s : unvisited) if (times[s] < times[current]) current = s;
    curr_time = times[current];

    cout << "current: " << current.name << '\n';
    
  }
  

  // troubleshooting
  cout << '\n';
  for (auto t : times) if (t.second != 99999) cout << (t.first).name << ' ' << t.second << '\n';
  cout << '\n';

  // construct path from prevs
  while (current != s1) {
    path.push_back(current);
    current = prevs[current];
  }

  return path;
}




  // auto start = chrono::high_resolution_clock::now();
  // cout << chrono::duration<double, std::milli>(chrono::high_resolution_clock::now() - start).count() << '\n';

 
int main() {

  cout.precision(10);

  set<Stop> stops = make_stops("stops.csv");
  if (stops.empty()) return 1;
  vector<Route> routes = make_routes(stops, "routes.csv");
  if (routes.empty()) return 1;

  vector<Route> temp_routes;
  temp_routes.push_back(routes[0]);
  temp_routes.push_back(routes[2]);

  Stop query("PVC-E");
  Stop s1 = *stops.find(query);
  query = Stop("PD-N");
  Stop s2 = *stops.find(query);
  
  vector<Stop> path = find_path(1, 560, s1, s2, stops, temp_routes);
  for (auto s : path) cout << s.name << ' ' << '\n';
  

  Action a = Action("get on green route", s1, 5, 5);
  cout << a << '\n';



  return 0;
}
