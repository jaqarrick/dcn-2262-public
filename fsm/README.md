# CSCI-GA 2662: Data Comms & Networks - Lab 6 - TCP Finite State Machine

This is a TCP Finite State Machine simulator implemented in C++. It receives a series of EVENTS from stdin and reports the transitions for the FSM if they are valid.

![FSM](fsm.png)

## Installation

### Prerequisites

- `make` and a valid C++ compiler. The `Makefile` specifies g++.
- To use `make format` the `clang-format` binary must be available on your path.

## Usage

- Compile with `make`

Provide an input file:
```
$ ./fsm < inputfile
```

Or provide events directly in stdin
```
$ ./fsm 
PASSIVE
Event PASSIVE received, current State is LISTEN
CLOSE
Event CLOSE received, current State is CLOSED
```

## Project Structure and Background
As the lab specification allows, I have opted to complete this assignment using C++ as opposed to Java.

Please note from section IV.3 of the specification

> ... (or different programming language platform if preferred)

Using C++ meant implementing a simple FSM wrapper from scratch. This abstraction defined in `fsm.h` is imported and injected into the `TCP_FSM` class defined in `tcp.h`. The core FSM engine exposes two methods. The first, `registerTransition`, creates a mapping between a pair (state + event) (e.g. `CLOSED - LISTEN`) and a transition, which is an object that includes the next state following the provided event (e.g. `LISTEN`). Importantly, the FSM engine keeps track of an initial state, which is passed in the FSM constructor method. The second exposed method is `doEvent`, which finds the registered transition (if it exists) from the pair of the current event and the event name. For example, `doEvent(PASSIVE)` with current state `CLOSED` will locate the transition for `CLOSED:PASSIVE` and set the next state to `LISTEN`. If no such transition exists, we throw an exception.

The `TCP_FSM` class injects the `FSM` engine. This is very straightforward. First, we define all of the TCP states, events, and actions for the transitions. We define an abstract `Action` with a virtual `execute` method. For the transitions that handle data events (RDATA, SDATA), we include a counter reference, so we can easily increment the amount of data that has been processed. For other actions, the execute is a simple log statement. 

On initialization, the `FSM_TCP` registers all transitions. Finally, the `doEvent` method is exposed, which is consumed in `main.cpp`.

```cpp
Tokenizer t(verbose);
TCP_FCM tcp_fsm(verbose);

string *token;
while ((token = t.getToken()) != nullptr) {
tcp_fsm.doEvent(*token);
}
```

Note, we implement a Tokenizer helper class that will process each EVENT from stdin.


### Example usage

The following event strings represent a valid TCP connection lifecycle:

`PASSIVE SYN SYNACK RDATA SDATA RDATA CLOSE FIN ACK`


```sh
$ ./fsm < inputs/in1

Event PASSIVE received, current State is LISTEN
Event SYN received, current State is SYN_RCVD
Event SYNACK received, current State is ESTABLISHED
DATA received 1
DATA sent 2
DATA received 3
Event CLOSE received, current State is FIN_WAIT_1
Event FIN received, current State is CLOSING
Event ACK received, current State is TIME_WAIT
```

Invalid events will print errors:
```sh
$./fsm
PASSIVE
Event PASSIVE received, current State is LISTEN
SYN
Event SYN received, current State is SYN_RCVD
FIN
FSM ERROR: No transition for event 'FIN' in state 'SYN_RCVD'
```


