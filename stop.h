#include <string>
#include <iostream>

class Stop {
  public:
    std::string stop_id;
    std::string desc;
    std::string id;
    std::string name;
    long double lat;
    long double lon;
    Stop();
    Stop(std::string, std::string, std::string, std::string, std::string);
    Stop(const Stop&);
    Stop & operator=(const Stop&);
    bool operator<(const Stop&) const;
};

std::ostream& operator<<(std::ostream& os, const Stop& s) {
  os << s.id << " " << s.stop_id << ' ' << s.name << "\n    " << s.desc << " " << s.lat << " " << s.lon;
  return os;
}
