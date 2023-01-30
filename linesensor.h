// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _LINESENSOR_H
#define _LINESENSOR_H

#define LS_LEFT_PIN A0
#define LS_MIDDLE_PIN A2
#define LS_RIGHT_PIN A3
#define LS_EMIT_PIN 11

#define left A11
#define right A4

#define NB_LS_PINS 5

// Class to operate the linesensor(s).
class LineSensor_c {
  public:
    int ls_pins[NB_LS_PINS] = { LS_LEFT_PIN, LS_MIDDLE_PIN, LS_RIGHT_PIN, left, right };
    float ls_reading[NB_LS_PINS];
    float ls_normalized_reading[NB_LS_PINS];
    float ls_weighted_reading[NB_LS_PINS];
    int ls_digital_reading[NB_LS_PINS];
    int ls_value[NB_LS_PINS];
    float weight_left, weight_right, e_line;
    float thresholdValue = 2000;
    // Constructor, must exist.
    LineSensor_c() {

    }

    void disableIRLeds() {

      pinMode(LS_EMIT_PIN, OUTPUT);
      pinMode(LS_EMIT_PIN, LOW);
    }

    void chargeCapacitors() {

      pinMode(LS_EMIT_PIN, OUTPUT);
      pinMode(LS_EMIT_PIN, HIGH);
      for (int which = 0 ; which < NB_LS_PINS ; which++ ) {

        pinMode(ls_pins[which], OUTPUT);
        digitalWrite(ls_pins[which], HIGH);

        delayMicroseconds(10);
      }
    }

    void enableIRLeds() {

      for ( int which = 0 ; which < NB_LS_PINS ; which++ ) {

        pinMode(ls_pins[which], INPUT);
      }

    }

    void parallelLineSensorRead() {



      unsigned long sensor_read[NB_LS_PINS];
      int remaining = NB_LS_PINS;
      unsigned long start_time;
      unsigned long end_time_ls[NB_LS_PINS];
      unsigned long elapsed_time;
      unsigned long timeout = 5000;

      float total_activation = 0;

      start_time = micros();

      for ( int which = 0 ; which < NB_LS_PINS ; which++ ) {
        end_time_ls[which] = 0;
      }

      while ( remaining > 0 ) {



        for ( int which = 0 ; which < NB_LS_PINS ; which++ ) {

          if ( digitalRead( ls_pins[which]) == LOW) {

            if (end_time_ls[which] == 0) {

              end_time_ls[which] = micros();

              remaining -= 1;


            }

          } //if all sensors read

        } //for each sensor

      }// while not all done

      //Save the reading for each sensor
      for (int which = 0 ; which < NB_LS_PINS ; which ++) {
        unsigned long elapsed_time;
        elapsed_time = end_time_ls[which] - start_time;
        ls_reading[which] = (float)elapsed_time;

        
      }

      for(int which = 0 ; which < 3 ; which++){
          total_activation += ls_reading[which];
        }
      
      normalizedValues(total_activation);
    }
    void normalizedValues(float total_activation) {
      for (int which = 0 ; which < 3 ; which++) {

        ls_normalized_reading[which] = (ls_reading[which]) / total_activation;

      }

    }

    float getLineError() {

      weight_left = ls_normalized_reading[0] + (ls_normalized_reading[1] * 0.5);
      weight_right = ls_normalized_reading[2] + (ls_normalized_reading[1] * 0.5);
      e_line = weight_left - weight_right;
      return e_line;
    }
};



#endif
