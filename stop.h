#include <string>
#include <iostream>

class Stop {
  public:
    std::string id;
    std::string name;
    std::string desc;
    long double lat;
    long double lon;
    Stop();
    Stop(std::string, std::string, std::string, std::string, std::string);
    Stop(const Stop&);
    Stop & operator=(const Stop&);
    bool operator<(const Stop&) const;
};

std::ostream& operator<<(std::ostream& os, const Stop& s) {
  os << s.id << " " << s.name << " " << s.desc << "\n  " << s.lat << " " << s.lon;
  return os;
}
