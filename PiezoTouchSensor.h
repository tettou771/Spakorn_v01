
class PiezoTouchSensor {
  public:
    PiezoTouchSensor(int _analogPin) {
      sensorPin = _analogPin;

      initialize();

      // 感度はここの数値で決まる。小さいと感度が高い
      setThreshold(20000);

      // センシングするバンドパスフィルターに関わる値
      setNudge(0.1, 0.01);
    }
    ~PiezoTouchSensor() {
      ;
    }

    void initialize() {
      pinMode(sensorPin, INPUT);
      delay(500);
      averageSlow = averageQuick = analogRead(sensorPin);
    }

    void update() {
      value = analogRead(sensorPin);
      averageSlow += (value - averageSlow) * nudgeSlow;
      averageQuick += (value - averageQuick) * nudgeQuick;

      if (abs(averageQuick - averageSlow) > threshold) {
        touched = true;
      }
      else {
        touched = false;
      }
    }

    bool getTouched() {
      return touched;
    }

    void setThreshold(double _threshold) {
      threshold = _threshold;
    }

    void setNudge(double _slow, double _quick) {
      nudgeSlow = constrain(_slow, 0, 1.0);
      nudgeQuick = constrain(_quick, 0, 1.0);
    }

    double getThreshold() {
      return threshold;
    }

  private:

    int sensorPin;
    int value;
    double averageSlow, averageQuick;
    double threshold, nudgeSlow, nudgeQuick;
    bool touched;
};
