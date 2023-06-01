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
#include <memory>

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
Action_Tree find_path(int day, int curr_time, Stop &s1, Stop &s2, set<Stop> all_stops, vector<Route> all_routes) {

  set<Stop> unvisited = all_stops;
  set<Stop> visited;
  map<Stop, shared_ptr<Action>>action_index;  // most recent time and action for each stop

  shared_ptr<Action> root = shared_ptr<Action>(new Action);
  Action_Tree action_tree(root);
  shared_ptr<Action> curr_action = root;


  for (auto s : all_stops) {
    action_index[s] = shared_ptr<Action>(new Action("", "", s, 99999));
  }

  Stop curr_stop = s1;
  (action_index[curr_stop])->time = curr_time;

  auto it = action_index.begin();
  while (it != action_index.end()) {
    cout << it->first.id << ' ' << it->second->time << '\n';
    ++it;
  }

  while (!unvisited.empty()) {

    if (curr_stop == s2) break; // exit if we've reached the destination

    // only routes that contain the curr_stop stop should be considered
    vector<Route> possible_routes;
    for (auto r : all_routes)
      if (r.id_sorted_stops.find(Stop(curr_stop.id)) != r.id_sorted_stops.end())
        possible_routes.push_back(r);

    // for each route, find the next stop and add it to neighbors
    for (auto r : possible_routes) {
      auto it = r.time_sorted_stops.begin();
      while (it->second != curr_stop && it != r.time_sorted_stops.end()) ++it;
      cout << "r: " << r.name << " | s: " << curr_stop.id << '\n';
      
      // if this stop isnt last and the next stop is unvisited
      if (++it != r.time_sorted_stops.end() && unvisited.find(it->second) != unvisited.end()) {

        // calculate time to get to next stop, including waiting
        if (r.name == curr_action->route) {  // if on the same route, just add time
          curr_time += (it->first - (--it)->first); ++it;
        }
        else {  // if on diff route, add minute to change stops, wait for next arrival
          int dep_time = r.scheds[day].start + r.id_sorted_stops[curr_stop.id];
          while (dep_time < curr_time + 1) dep_time += r.scheds[day].freq;
          curr_time = dep_time;
        }

        cout << "route: " << r.name << " | " << (it->second).id << " curr time: " << curr_time << '\n';
        // if this is the fastest way to get to this stop, update action_index, add to action_tree
        if (curr_time < action_index[it->second]->time) {  
          cout << "initial: " << action_index[it->second]->time << " new: " << curr_time << " faster!\n";
          action_index[it->second]->time = curr_time;


          cout << "PREV ROUTE: " << curr_action->route << " THIS ROUTE: " << r.name << '\n';
          // if switching routes, add a fork to action_tree
          if (curr_action->route != r.name) {
            cout << " MAKING SPLIT!\n";
            curr_action->add_child(shared_ptr<Action>(new Action("Get off", curr_action->route, it->second, 0)));
            curr_action = curr_action->children[0];
            curr_action->add_child(shared_ptr<Action>(new Action("get on", r.name, it->second, curr_time)));
          }
          else {
            curr_action->add_child(shared_ptr<Action>(new Action("Stay on", r.name, it->second, curr_time)));
          }
          // update action_index, actions
          cout << "mid route loop CA: " << *curr_action << '\n';
          action_index[it->second]->time = curr_time;
          action_index[it->second] = curr_action;
        } 
      }
      // return to previous stop before looping to other routes
      curr_stop = (--it)->second;

      //for (auto t : action_index) cout << "<   " << t.first.id << ' ' << t.second.first << '\n';
    }

    // update curr_stop to lowest time unvisited stop, update curr_time / curr_action accordingly
    unvisited.erase(curr_stop);
    curr_stop = *unvisited.begin();
    for (auto s : unvisited) {
      if (action_index[s]->time < action_index[curr_stop]->time) curr_stop = s;
      cout << s.name << ' ' << action_index[s]->time << '\n';
    }
    curr_time = action_index[curr_stop]->time;
    curr_action = action_index[curr_stop];

    cout << "curr_stop: " << curr_stop.name << '\n';
    cout << "curr_action: " << *curr_action << "\n----------\n";

  }
  
  // troubleshooting
  cout << '\n';
  for (auto t : action_index) if (t.second->time != 99999) cout << t.second->stop.name << ' ' << t.second->time << '\n';
  cout << '\n';

  // // construct path from prevs
  // while (curr_stop != s1) {
  //   path.push_back(curr_stop);
  //   curr_stop = prevs[curr_stop];
  // }

  // return path;
  return action_tree;
}
 
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
  
  Action_Tree path = find_path(1, 560, s1, s2, stops, routes);
  //path.print_tree();
  

  // Action a = Action("a", "", s1, 5);
  // Action b = Action("b", "", s1, 5);
  // Action c = Action("c", "", s1, 5);
  // Action d = Action("d", "", s1, 5);
  // Action e = Action("e", "", s1, 5);
  // Action f = Action("f", "", s1, 5);
  // Action h = Action("h", "", s1, 5);
  // Action i = Action("i", "", s1, 5);
  // Action j = Action("j", "", s1, 5);
  // Action l = Action("l", "", s1, 5);
  // Action m = Action("m", "", s1, 5);
  // Action n = Action("n", "", s1, 5);
  // Action o = Action("o", "", s1, 5);
  // Action p = Action("p", "", s1, 5);
  // Action q = Action("q", "", s1, 5);

  // Action_Tree at = Action_Tree(&a);
  // a.add_child(&b);
  // a.add_child(&c);
  // a.add_child(&d);
  // b.add_child(&e);
  // b.add_child(&f);
  // c.add_child(&h);
  // c.add_child(&i);
  // c.add_child(&j);
  // d.add_child(&l);
  // h.add_child(&m);
  // l.add_child(&n);
  // m.add_child(&o);
  // n.add_child(&p);
  // n.add_child(&q);

  // at.print_tree();

  //path.print_tree();




  return 0;
}
  
// in case i need to time anything
  // auto start = chrono::high_resolution_clock::now();
  // cout << chrono::duration<double, std::milli>(chrono::high_resolution_clock::now() - start).count() << '\n';
