#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include "stop.h"

class Action {
  public:
    Action();
    Action(std::string, std::string, Stop&, int);
    Action& operator=(const Action&);
    Action& operator*();
    
    std::string act;
    std::string route;
    Stop stop;
    int time;

    shared_ptr<Action> parent;
    vector<shared_ptr<Action>> children;

    void add_child(shared_ptr<Action>);
    void print_below();
};

std::ostream & operator<<(std::ostream &, const Action &);

class Action_Tree {
  public:
    Action_Tree();
    Action_Tree(shared_ptr<Action>);

    shared_ptr<Action> root;

    void print_tree();

    Action* find_action(std::string);
};

std::ostream & operator<<(std::ostream &, const Action_Tree &);
    
#endif
