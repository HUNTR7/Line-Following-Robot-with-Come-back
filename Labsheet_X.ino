#include "motors.h"
#include "linesensor.h"
#include "encoders.h"
#include "kinematics.h"
#include "pid.h"
#include <math.h>

#define LED_PIN 13  // Pin to activate the orange LED
boolean led_state;  // Variable to "remember" the state
// of the LED, and toggle it.

LineSensor_c ls;
Motors_c motors;
Kinematics_c kinematics;

float threshold = 2000;
float e_line;

int state = 0;
bool passedGap = false;
bool turn = false;

int i = 0;

bool u = false;

bool a = true;
bool b = true;
bool c = true;
bool d = true;

long e0, e1;

unsigned long startTime, currentTime, elapsed;

// put your setup code here, to run once:
void setup() {

  Serial.begin(9600);
  delay(1000);
  Serial.println("***RESET***");

  // Set LED pin as an output
  pinMode( LED_PIN, OUTPUT );

  // Set initial state of the LED
  led_state = false;

  setupEncoder1();
  setupEncoder0();
  motors.initialise();
  EnableSensors();

  startTime = millis();
}


// put your main code here, to run repeatedly:
void loop() {

  // Using an if statement to toggle a variable
  // with each call of loop()
  if ( led_state == true ) {
    led_state = false;
  } else {
    led_state = true;
  }

  // We use the variable to set the
  // debug led on or off on the 3Pi+
  digitalWrite( LED_PIN, led_state );


  e_line = ls.getLineError();
  ReadSensors();

  currentTime = millis();
  elapsed = currentTime - startTime;
  if (elapsed >= 40000) {

    state = 2;
  }

  if (state == 0) {
    JoinLine();

  }

  else if (state == 1) {
    FollowLine();
    uTurn();
  }

  else if (state == 2) {
    GoBack();
  }

}

void beep() {
  analogWrite(6, 100);
  delay (500);
  analogWrite(6, 0);
}

void EnableSensors() {
  ls.chargeCapacitors();
  ls.enableIRLeds();

}

void ReadSensors() {
  EnableSensors();
  ls.parallelLineSensorRead();
  kinematics.update(count_e0, count_e1);
}

void JoinLine() {
  if (ls.ls_reading[0] < threshold && ls.ls_reading[1] < threshold && ls.ls_reading[2] < threshold) {
    i += 1;
    motors.goForward(e_line);


  }
  else if (ls.ls_reading[1] >= threshold) {
    //start following line
    beep();
    motors.Stop();
    state = 1;
  }
}

void FollowLine() {
  if (!turn) {
    motors.TurnLeft();
    if (ls.ls_reading[1] >= threshold) {
      turn = true;
    }

  }
  else if (turn) {
    if (ls.ls_reading[1] >= threshold) {
      motors.goForward(e_line);

    }

    else if (ls.ls_reading[2] <= threshold) {

      motors.goForward(e_line);


    }
    else if (ls.ls_reading[0] <= threshold) {

      motors.goForward(e_line);



    }
    else if (ls.ls_reading[4] <= 6500) {
      //movement();
      //motors.goForward(e_line);
      motors.TurnRight();

    }
    else if (ls.ls_reading[3] <= 6500) {

      motors.TurnLeft();

    }
    else if (ls.ls_reading[1] < threshold) {
      motors.goForward(e_line);



    }
  }

}

void uTurn() {
  if (ls.ls_reading[1] < threshold && elapsed < 6000) {
    motors.TurnLeft();
  }

}



void GoBack() {
  while (d == true) {
    while (a == true) {
      motors.Stop();
      delay(1000);
      a = false;
    }
    kinematics.update(count_e1, count_e0);

    if (kinematics.alpha < 0 && b == true) {
      Serial.println("B running");
      e0 = count_e0 + 213;
      e1 = count_e1 - 213;
      while (count_e0 < e0 && count_e1 > e1) {
        digitalWrite(16, HIGH);
        digitalWrite(15, LOW);
        analogWrite(10, 20);
        analogWrite(9, 20);
      }
      b = false;
    }

    else if (kinematics.alpha > 0 && c == true) {
      Serial.println("C running");
      e0 = count_e0 - 283;
      e1 = count_e1 + 283;
      while (count_e0 > e0 && count_e1 < e1) {
        digitalWrite(16, LOW);
        digitalWrite(15, HIGH);
        analogWrite(10, 20);
        analogWrite(9, 20);
      }
      c = false;
    }
    unsigned long start = millis();
    unsigned long current = millis() - start;
    while (current < 12500) {
      Serial.println("Going Back");
      digitalWrite(16, LOW);
      digitalWrite(15, LOW);
      analogWrite(10, 24);
      analogWrite(9, 26);
      current = millis() - start;
    }
    d = false;
  }
  motors.Stop();
}
