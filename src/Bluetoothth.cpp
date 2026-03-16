#include "mbed.h" //Includes Mbed library

Serial hm10(PA_11, PA_12); //UART6 TX,RX

PwmOut MotorL(PC_8), MotorR(PC_6);
DigitalOut Enable(PA_15);
DigitalOut Direction1(PA_14), Direction2(PA_13);
DigitalOut Bipolar1(PB_15), Bipolar2(PB_2);

char c; //the character we want to receive

int main() {
    hm10.baud(9600);
    int t = 10;
    float dc1 = 0.5, dc2 = 0.5;                 // PWM period and duty cycle
    float wdist = 0.195;
    float rt = 0.4, d = 1.0;            // time required for buggy to move 1m and rotate 90deg
    // CHANGE THESE ^^^^^^^

    InterruptIn button(D4);

    Enable.write(1);
    Direction1 = 1;
    Direction2 = 1;
    Bipolar1 = 0;
    Bipolar2 = 0;

    MotorL.write(1);
    MotorR.write(1);
    MotorL.period_ms(t);
    MotorR.period_ms(t);                // initial state of motors: zero duty cycle, enable off

  while(1) {
    if(hm10.readable()){
        c = hm10.getc(); //read a single character
        
        if(c == 'A'){
            MotorL.write(dc1);
            MotorR.write(dc2);
            wait(2*rt);
            MotorL.write(1);
            MotorR.write(1);               // rotate 180 degrees
            
        }
    }
  }
}