// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _MOTORS_H
#define _MOTORS_H


# define L_PWM_PIN 10
# define L_DIR_PIN 16
# define R_PWM_PIN 9
# define R_DIR_PIN 15


# define FWD LOW
# define REV HIGH

// Class to operate the motor(s).
class Motors_c {
  public:

    // Constructor, must exist.
    Motors_c() {

    }

    // Use this function to
    // initialise the pins and
    // state of your motor(s).
    void initialise() {
      pinMode(L_PWM_PIN, OUTPUT);
      pinMode(L_DIR_PIN, OUTPUT);
      pinMode(R_PWM_PIN, OUTPUT);
      pinMode(R_DIR_PIN, OUTPUT);
    }

    // Write a function to operate
    // your motor(s)
    // ...

    void goForward(float errorLine) {
      if (errorLine >= -0.1 && errorLine <= 0.1) {
        //goForward
        digitalWrite(L_DIR_PIN, FWD);
        digitalWrite(R_DIR_PIN, FWD);
        setPower(errorLine);
      }

      else if (errorLine < -0.1) {
        //turnRight
        digitalWrite(L_DIR_PIN, FWD);
        digitalWrite(R_DIR_PIN, REV);
        setPower(errorLine);
      }

      else if (errorLine > 0.1) {
        //turnLeft
        digitalWrite(L_DIR_PIN, REV);
        digitalWrite(R_DIR_PIN, FWD);
        setPower(errorLine);
      }
    }

    void TurnLeft() {
      digitalWrite(L_DIR_PIN, REV);
      digitalWrite(R_DIR_PIN, FWD);
      analogWrite(L_PWM_PIN, 25);
      analogWrite(R_PWM_PIN, 25);

    }

    void TurnRight() {
      digitalWrite(L_DIR_PIN, FWD);
      digitalWrite(R_DIR_PIN, REV);
      analogWrite(L_PWM_PIN, 25);
      analogWrite(R_PWM_PIN, 25);
    }

    void Stop(){
      digitalWrite(L_DIR_PIN, HIGH);
      digitalWrite(R_DIR_PIN, HIGH);
      analogWrite(L_PWM_PIN, 0);
      analogWrite(R_PWM_PIN, 0);
      }

    void setPower(float e) {
      analogWrite(L_PWM_PIN, 20 - (20 * e));
      analogWrite(R_PWM_PIN, 20 + (20 * e));
      //Serial.println(20 - (20 * e));
    }
};

#endif
