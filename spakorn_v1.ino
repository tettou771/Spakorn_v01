#include "PiezoTouchSensor.h"
#include "Button.h"
#include "State.h"

//#define _DEBUG

long int chargeDuration = 3000; // ms

void setup() {
  pinMode(solenoidPin, OUTPUT);
  pinMode(buttonLedPin, OUTPUT);
  digitalWrite(buttonLedPin, LOW);

  // debug
  /*
    for (int p=0; p<17; ++p) {
    pinMode(p, OUTPUT);
    for (int i = 0; i < 3; ++i) {
      digitalWrite(p, HIGH);
      delay(100);
      digitalWrite(p, LOW);
      delay(100);
    }
    }
  */

  sensor = new PiezoTouchSensor(sensorPin);
  sensor->setThreshold(6);
  sensor->setNudge(0.0001, 0.01);

  button = new Button(buttonPin);

  playStartSound();

  setState(Waiting);
}

void loop() {
  unsigned long now = millis();
  unsigned long stateElapsedTime = now - stateBeginTime;
  button->update();

  switch (state) {
    case Waiting:
      {
        // blink led slow
        digitalWrite(buttonLedPin, stateElapsedTime % 2000 < 20);

        if (button->getOnLow()) {
          nextState();
        }
      }
      break;
    case Charging:
      {
        // if button pressed, cancel
        if (button->getOnLow()) {
          setState(Waiting);
          playCancelSound();
        } else {
          // blink led fast
          digitalWrite(buttonLedPin, stateElapsedTime % 100 < 50);
          float t = (float)stateElapsedTime / chargeDuration;
          tone(sensorPin, 200 + t * 600);

          // check end of state
          if (stateElapsedTime > chargeDuration) {
            nextState();
          }
        }
      }
      break;
    case Ready:
      // if button pressed, cancel
      if (button->getOnLow()) {
        playCancelSound();
        setState(Waiting);
      } else {
        sensor->update();

        // if touched, discharge and go next state
        if (sensor->getTouched()) {
          discharge();
          nextState();
        }
      }
      break;
  }

  delay(1);
}

void discharge() {
  digitalWrite(solenoidPin, HIGH);
  delay(200);
  digitalWrite(solenoidPin, LOW);
}

void playStartSound() {
  pinMode(sensorPin, OUTPUT);
  int duration = 60; // us
  tone(sensorPin, 262); // do
  digitalWrite(buttonLedPin, HIGH);
  delay(duration);
  tone(sensorPin, 294); // re
  digitalWrite(buttonLedPin, LOW);
  delay(duration);
  tone(sensorPin, 350); // fa
  digitalWrite(buttonLedPin, HIGH);
  delay(duration);
  tone(sensorPin, 392); // so
  digitalWrite(buttonLedPin, LOW);
  delay(duration);
  tone(sensorPin, 494); // si
  digitalWrite(buttonLedPin, HIGH);
  delay(duration);
  noTone(sensorPin);
  digitalWrite(buttonLedPin, LOW);
  pinMode(sensorPin, INPUT);
}

void playCancelSound() {
  pinMode(sensorPin, OUTPUT);
  digitalWrite(buttonLedPin, LOW);

  for (int i = 0, N = 20; i < N; ++i) {
    float f = map(i, 0, N, 800, 200);
    tone(sensorPin, f);
    delay(300 / N);
  }

  noTone(sensorPin);
  pinMode(sensorPin, INPUT);
}
