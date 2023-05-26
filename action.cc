#include <string>
#include <iostream>

#include "action.h"

using namespace std;

Action::Action() :
  act(""), initial_stop(Stop()), final_stop(Stop()), initial_time(0), final_time(0) {};
Action::Action(string a, Stop is, Stop fs, int it, int ft) :
  act(a), initial_stop(is), initial_time(it), final_time(ft) {};
Action::Action(string a, Stop is, int it, int ft) :
  act(a), initial_stop(is), initial_time(it), final_time(ft) {};

ostream& operator<<(ostream &os, const Action &a) {
  // action is for example get off at stop xyz at time t
  if (a.final_stop == Stop()) {
    os << "Action: " << a.act << " at stop " << a.initial_stop << " at " << a.initial_time;
  }
  else {
    // action might be ride green route from stop abc to stop xyz for t minutes
    os << "Action: " << a.act << " from " << a.initial_time << " to " << a.final_time
       << " at " << a.initial_time << " for " << a.final_time - a.initial_time << " minutes.";
  }
  return os;
}
