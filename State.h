#include "DefinedPins.h"

enum State {
  Waiting,
  Charging,
  Ready,
  StateNum
};

State state;
unsigned long int stateBeginTime;

void setState(State next) {
  // on state begin
  switch (next) {
    case Waiting:
      // button led OFF
      digitalWrite(buttonLedPin, HIGH);
      break;
    case Charging:
      pinMode(sensorPin, OUTPUT);
      break;
    case Ready:
      noTone(sensorPin);
      pinMode(sensorPin, INPUT);
      // button LED ON
      digitalWrite(buttonLedPin, HIGH);

      // sensor ready
      sensor->initialize();

      break;
  }

  state = next;
  stateBeginTime = millis();
}

void nextState() {
  // go to next state
  State next = ((int)state + 1) % StateNum;
  setState(next);
}
