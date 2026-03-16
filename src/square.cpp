#include "mbed.h"

/*
code for the buggy to move in a square
change variables rt and d as needed

THE BUGGY DOESN'T ACTUALLY MAKE A SQUARE but it's close enough
and the code works as a test for movement
*/

PwmOut MotorL(PC_8), MotorR(PC_6);
DigitalOut Enable(PC_3);
DigitalOut Direction1(PA_14), Direction2(PA_13);
DigitalOut Bipolar1(PB_1), Bipolar2(PB_2);

void buttonISR() {
    if (Enable == 1) {
        Enable.write(0);
    }
    else {Enable.write(1);}
}

int main() {
    int t = 10;
    float dc1 = 0.5, dc2 = 0.5;                 // PWM period and duty cycle
    float wdist = 0.195;
    float rt = 0.9, d = 1.0;            // time required for buggy to move 1m and rotate 90deg
    // CHANGE THESE ^^^^^^^

    InterruptIn button(D4);

    button.rise(&buttonISR);

    Enable.write(0);
    Direction1 = 1;
    Direction2 = 0;
    Bipolar1 = 0;
    Bipolar2 = 0;

    MotorL.period_ms(t);
    MotorR.period_ms(t);
    MotorL.write(1);
    MotorR.write(1);                // initial state of motors: zero duty cycle, enable off

    while(1){
        if (Enable == 1){                   // start square sequence with button press
            MotorL.write(dc1);
            MotorR.write(dc2);               // initialise motor PWM pulses
            wait(d);
            for(int i=0;i<3;i++){           // square movement
                MotorR.write(1);            // move 1m, rotate 90deg, loop until square is done
                wait(rt);
                MotorR.write(dc2);
                wait(d);
            }
            MotorR.write(1);
            MotorL.write(1);
            wait(rt);
            MotorL.write(dc1);
            wait(2*rt);
            MotorR.write(dc2);               // rotate 180 degrees
            wait(d);

            for(int i=0;i<3;i++){           // square in opposite direction
                MotorL.write(1);
                wait(rt);
                MotorL.write(dc1);
                wait(d);
            }

            Enable.write(0);                // stop both motors, end of square
        }
        
    }
}
