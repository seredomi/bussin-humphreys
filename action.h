#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <iostream>

#include "stop.h"

class Action {
  public:
    Action();
    Action(std::string, Stop, Stop, int, int);
    Action(std::string, Stop, int, int);
    
    std::string act;
    Stop initial_stop;
    Stop final_stop;
    int initial_time;
    int final_time;
};

std::ostream & operator<<(std::ostream &, const Action &);

#endif
