#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <sstream>

#include "stop.h"

using namespace std;

// default constructor
Stop::Stop()
  : id(""), name(""), desc(""), lat(0), lon(0) {};

// actual constructor
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
bool Stop::operator<(const Stop& rhs) const {
  return id < rhs.id;
}
  
int main() {
  // open stops.csv
  ifstream ifs("stops.csv");
  if (!ifs) {
    cout << "Error opening stops.csv\n";
    return 1;
  }

  set<Stop> stops;
  string line;
  // for each line in stops.csv, split line by commas, initialize stop, insert in stops set
  while (getline(ifs, line)) {
    stringstream ss(line);
    vector<string> args;
    string arg;
    while (getline(ss, arg, ',')) args.push_back(arg);
    for (auto a : args) cout << a << '\n';
    stops.insert(Stop(args[0], args[2], args[1], args[3], args[4]));
  }
  ifs.close();

  // print out stops
  cout.precision(20);
  int i = 0;
  for (Stop s : stops){
    cout << s << '\n';
    ++i;
  }
  cout << "total: " << i << '\n';
  return 0;
}
