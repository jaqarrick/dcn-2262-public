#ifndef FSM_H
#define FSM_H

#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
using namespace std;

class State {
public:
  string name;
  State(const string &name) : name(name) {}
};

class Event {
public:
  string name;
  Event(const string &name) : name(name) {}
};

class Action;

class Transition {
public:
  State *fromState;
  State *toState;
  Event *event;
  Action *action;

  Transition(State *from, State *to, Event *event, Action *action)
      : fromState(from), toState(to), event(event), action(action) {}
};

class FSMException : public runtime_error {
public:
  FSMException(const string &msg) : runtime_error(msg) {}
};

class Action {
public:
  virtual void execute(const Event &e, const State &nextState) = 0;
  virtual ~Action() {}
};

class FSM {
private:
  string name;
  State *currentState;
  bool verbose;
  map<pair<string, string>, Transition *> transitions;

  void log(const string &message) {
    cout << "LOG [" << name << "]: ";
    cout << message << endl;
  }

public:
  FSM(const string &name, State *initialState, bool verbose)
      : name(name), currentState(initialState), verbose(verbose){};

  void registerTransition(Transition *t) {
    transitions.emplace(make_pair(t->fromState->name, t->event->name), t);
    if (verbose) {
      log("Transition added - FROM: " + t->fromState->name +
          " TO: " + t->toState->name + " EVENT: " + t->event->name);
    }
  }

  void doEvent(Event *e) {
    auto key = make_pair(currentState->name, e->name);
    auto it = transitions.find(key);

    if (it == transitions.end()) {
      throw FSMException("FSM ERROR: No transition for event '" + e->name +
                         "' in state '" + currentState->name + "'");
    }

    Transition *t = it->second;
    currentState = t->toState;
    t->action->execute(*e, *currentState);
  }
};

#endif
