class Button {
  public:
    Button(int pin):
      pin(pin) {
      pinMode(pin, INPUT_PULLUP);
      state = digitalRead(pin);
      pastChangeTime = millis();
    }
    ~Button() {

    }

    void update() {
      onLowFlag = false;
      onHighFlag = false;
      
      now = millis();

      if (now > pastChangeTime + ignoreTime) {
        current = digitalRead(pin);
        if (state != current) {
          state = current;
          pastChangeTime = now;

          if (state == LOW) {
            onLowFlag = true;
          }else{
            onHighFlag = true;
          }
        }
      }
    }

    bool getState() {
      return state;
    }

    bool getOnLow() {
      return onLowFlag;
    }

    bool getOnHigh() {
      return onHighFlag;
    }

    bool getOnChangd() {
      return onLowFlag || onHighFlag;
    }

  private:
    int pin;
    bool state, onLowFlag, onHighFlag, current;
    const unsigned long int ignoreTime = 100; // msec
    unsigned long int pastChangeTime, now; // msec
};
