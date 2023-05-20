#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <sstream>

#include "stop.h"

using namespace std;

// default constructor
Stop::Stop() : id(""), stop_id(""), name(""), desc(""), lat(0), lon(0) {};

// actual constructor
Stop::Stop(string i, string d, string n, string la, string lo) {
  stop_id = i;
  id = i + d;
  for (char& c : n) if (c == '_') c = ' ';
  name = n;
  if (d == "X") desc = "";
  else if (d == "N") desc = "Northern roadside";
  else if (d == "S") desc = "Southern roadside";
  else if (d == "E") desc = "Eastern roadside";
  else desc = "Western roadside";
  lat = stold(la);
  lon = stold(lo);
}

// copy constructor
Stop::Stop(const Stop& s) {
  id = s.id;
  stop_id = s.stop_id;
  name = s.name;
  desc = s.desc;
  lat = s.lat;
  lon = s.lon;
}

// assignment operator
Stop& Stop::operator=(const Stop& rhs) {
  id = rhs.id;
  stop_id = rhs.stop_id;
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
  // open stops.txt
  ifstream ifs("stops.txt");
  if (!ifs) {
    cout << "Error opening stops.txt\n";
    return 1;
  }
  set<Stop> stops;
  string line;
  // for each line in stops, split line by whitespace, initialize stop, insert in stops set
  while (getline(ifs, line)) {
    stringstream ss(line);
    vector<string> args;
    string arg;
    while (ss >> arg) args.push_back(arg);
    stops.insert(Stop(args[0], args[1], args[2], args[3], args[4]));
  }
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
