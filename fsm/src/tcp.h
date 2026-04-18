#ifndef TCP_FSM_H
#define TCP_FSM_H

#include "fsm.h"

class RegularAction : public Action {
public:
  void execute(const Event &e, const State &nextState) override {
    cout << "Event " << e.name << " received, current State is "
         << nextState.name << endl;
  }
};

class DataAction : public Action {
private:
  int &counter;
  bool receive;

public:
  DataAction(int &counter, bool receive) : counter(counter), receive(receive) {}

  void execute(const Event &e, const State &nextState) override {
    counter++;
    if (receive)
      cout << "DATA received " << counter << endl;
    else
      cout << "DATA sent " << counter << endl;
  }
};

class TCP_FCM {

private:
  bool verbose;
  State CLOSED{"CLOSED"}, LISTEN{"LISTEN"}, SYN_RCVD{"SYN_RCVD"},
      SYN_SENT{"SYN_SENT"}, ESTABLISHED{"ESTABLISHED"},
      FIN_WAIT_1{"FIN_WAIT_1"}, FIN_WAIT_2{"FIN_WAIT_2"},
      CLOSE_WAIT{"CLOSE_WAIT"}, CLOSING{"CLOSING"}, LAST_ACK{"LAST_ACK"},
      TIME_WAIT{"TIME_WAIT"};

  Event PASSIVE{"PASSIVE"}, ACTIVE{"ACTIVE"}, SYN{"SYN"}, SYNACK{"SYNACK"},
      ACK{"ACK"}, RDATA{"RDATA"}, SDATA{"SDATA"}, FIN{"FIN"}, CLOSE{"CLOSE"},
      TIMEOUT{"TIMEOUT"};

  FSM fsm{"TCP", &CLOSED, verbose};

  int numDataEvents = 0;
  RegularAction regularAction{};
  DataAction dataActionSend{numDataEvents, false};
  DataAction dataActionReceive{numDataEvents, true};

  void registerTransitions() {
    fsm.registerTransition(
        new Transition(&CLOSED, &LISTEN, &PASSIVE, &regularAction));
    fsm.registerTransition(
        new Transition(&CLOSED, &SYN_SENT, &ACTIVE, &regularAction));
    fsm.registerTransition(
        new Transition(&LISTEN, &SYN_RCVD, &SYN, &regularAction));    
    fsm.registerTransition(
        new Transition(&LISTEN, &CLOSED, &CLOSE, &regularAction));
    fsm.registerTransition(
        new Transition(&SYN_RCVD, &ESTABLISHED, &SYNACK, &regularAction));
    fsm.registerTransition(
        new Transition(&SYN_SENT, &ESTABLISHED, &SYNACK, &regularAction));
    fsm.registerTransition(
        new Transition(&SYN_SENT, &CLOSED, &CLOSE, &regularAction));
    fsm.registerTransition(
        new Transition(&ESTABLISHED, &ESTABLISHED, &RDATA, &dataActionReceive));
    fsm.registerTransition(
        new Transition(&ESTABLISHED, &ESTABLISHED, &SDATA, &dataActionSend));
    fsm.registerTransition(
        new Transition(&SYN_RCVD, &FIN_WAIT_1, &CLOSE, &regularAction));
    fsm.registerTransition(
        new Transition(&FIN_WAIT_1, &FIN_WAIT_2, &ACK, &regularAction));
    fsm.registerTransition(
        new Transition(&FIN_WAIT_1, &CLOSING, &FIN, &regularAction));
    fsm.registerTransition(
        new Transition(&FIN_WAIT_2, &TIME_WAIT, &FIN, &regularAction));
    fsm.registerTransition(
        new Transition(&CLOSING, &TIME_WAIT, &ACK, &regularAction));
    fsm.registerTransition(
        new Transition(&TIME_WAIT, &CLOSED, &TIMEOUT, &regularAction));
    fsm.registerTransition(
        new Transition(&ESTABLISHED, &CLOSE_WAIT, &FIN, &regularAction));
    fsm.registerTransition(
        new Transition(&ESTABLISHED, &FIN_WAIT_1, &CLOSE, &regularAction));
    fsm.registerTransition(
        new Transition(&CLOSE_WAIT, &LAST_ACK, &CLOSE, &regularAction));
    fsm.registerTransition(
        new Transition(&LAST_ACK, &CLOSED, &ACK, &regularAction));
  }

  map<string, Event *> eventMap = {
      {"PASSIVE", &PASSIVE}, {"ACTIVE", &ACTIVE}, {"SYN", &SYN},
      {"SYNACK", &SYNACK},   {"ACK", &ACK},       {"RDATA", &RDATA},
      {"SDATA", &SDATA},     {"FIN", &FIN},       {"CLOSE", &CLOSE},
      {"TIMEOUT", &TIMEOUT}};
  bool isValidEvent(const string &token) { return eventMap.count(token) > 0; }

public:
  TCP_FCM(bool v) : verbose(v), fsm("TCP", &CLOSED, v) {
    registerTransitions();
  }

  void doEvent(const string &token) {
    try {
      if (!isValidEvent(token))
        throw FSMException("Error: unexpected Event: " + token);
      fsm.doEvent(eventMap[token]);
    } catch (FSMException &e) {
      cout << e.what() << endl;
    }
  }
};

#endif