#include <string>
#include <vector>
#include <iostream>

#include "action.h"

using namespace std;

// Action::Action() {
// 
//   act = "";
//   stop = Stop();
//   time = 0;
//   // const Action &a = Action("", stop, 0);
//   // parent = Action("", stop, 0);
// }

Action::Action() : act(""), route(""), stop(Stop()), time(0), parent(nullptr) {};
Action::Action(string a, string r, Stop &s, int t) :
  act(a), route(r), stop(s), time(t), parent(nullptr) {};

Action& Action::operator*() { return *this; }

ostream& operator<<(ostream &os, const Action &a) {
  // action is for example get off at stop xyz at time t
  // if (a.final_stop == Stop()) {
  //   os << "Action: " << a.act << " at stop " << a.initial_stop << " at " << a.initial_time;
  // }
  // else {
  //   // action might be ride green route from stop abc to stop xyz for t minutes
  //   os << "Action: " << a.act << " from " << a.initial_time << " to " << a.final_time
  //      << " at " << a.initial_time << " for " << a.final_time - a.initial_time << " minutes.";
  // }
  // return os;
  os << a.act << ' ' << a.route << " route at " << a.stop.name << " stop at " << a.time / 60 << ':' << a.time % 60;
  return os;
}


void Action::add_child(shared_ptr<Action> child) {
  this->children.push_back(child);
  child->parent = shared_ptr<Action>(this);
}

void Action::print_below() {
  if ((this->children).empty()) {
    cout << '\n';
    return;
  }

  else {
    for (auto child : this->children) {
      cout << *child << ' ';
      child->print_below();
    }
  }
}

// Action* Action::find_below(string stop_id, string route) {
// 
//   if ((this->children).empty()) return nullptr;
// 
//   else {
//     for (auto child : this->children) {
//       if (child->stop.id == stop_id && child->route == route) return child;
//       else child->find_below(stop_id, route);
//     }
//   }




Action_Tree::Action_Tree() {
  root = nullptr; 
}
Action_Tree::Action_Tree(shared_ptr<Action> r) {
  root = r;
}

void Action_Tree::print_tree() { root->print_below(); }
// void Action_Tree:find_action(string stop_id, string route) { root->find_below(stop_id, route); }



ostream & operator<<(ostream &os, const Action_Tree &at) {
  os << "action tree\n";

    os <<  endl;
  return os;
}
