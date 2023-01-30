// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _KINEMATICS_H
#define _KINEMATICS_H

// Class to track robot position.
class Kinematics_c {
  public:
    float leftDist, rightDist;
    float thetha, oldThetha, alpha, oldAlpha, t, X, Y, oldX, oldY, finalDist;
    int th;
    // Constructor, must exist.
    Kinematics_c() {

    }

    // Use this function to update
    // your kinematics
    void update(volatile long leftEncoderCount, volatile long rightEncoderCount) {
      leftDist = leftEncoderCount * 0.28;
      rightDist = rightEncoderCount * 0.28;

      thetha = (((leftDist/(2*43)) - (rightDist/(2*43))) * 57.2958);
      oldX = (leftDist/2) + (rightDist/2);
      oldY = 0;

      if(thetha > 360 || thetha < -360){
        th = thetha;
        thetha = th % 360;
        alpha = thetha;
        }
        else{
          alpha = thetha;
          }
          X = oldX * cos(alpha/57.2958);
          Y = oldY * sin(alpha/57.2958);
          alpha = thetha;
      
    }

};



#endif
