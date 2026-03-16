#include "mbed.h"
#include "C12832.h"
#include "QEI.h"

// Enable pin to start motors and control system code
DigitalOut Enable(PA_15);

// Encoders for each motor
QEI encoderLeft(PC_10, PC_12, NC, 256);
QEI encoderRight(PB_14, PB_13, NC, 256);

// Interrupt service routine for button press
void buttonISR() {
    Enable = !Enable;  // Toggle enable state of the motors
}

double CVL, CVR;
float minspeed = 0.3, maxspeed = 0.9;     //speed limits for speed control
float diff = 0, avgdr = 0.6, avgspeed=0.6;
double RPSL, RPSR;  

void velocity(){
     // Variables to 
    RPSL = 20.0*encoderLeft.getPulses()/512;
    CVL = (RPSL*0.245);  //revs*circumfrence circ = diameter*pi=0.078*pi
    encoderLeft.reset();
     
    RPSR = 20.0*encoderRight.getPulses()/512;
    CVR = (RPSR*0.245);  
    encoderRight.reset();

    if(avgspeed < minspeed){              
		        avgdr -= 0.05;
			}

    if (avgspeed > maxspeed){
               avgdr += 0.1;
            }
}

// Function to determine area color based on sensor voltage
char getAreaColor(float voltage) {
    const float whiteThreshold = 1.4; // Threshold voltage for white
    const float blackThresholdLow = 0.0, blackThresholdHigh = 1.4; // Range for black
    if (voltage > whiteThreshold) return '1'; // White
    else if (voltage > blackThresholdLow && voltage < blackThresholdHigh) return '0'; // Black
    else return 'X'; // Undefined
}


int main() {
    // Define motor output pins
    PwmOut MotorL(PC_8), MotorR(PC_6);
    // Define digital outputs for motor control
    DigitalOut Direction1(PA_14), Direction2(PA_13);
    DigitalOut Bipolar1(PB_15), Bipolar2(PB_2);
    DigitalOut Switch1(PC_11), PD_Switch3(PD_2);

    // Analog inputs for sensors
    AnalogIn sensor1(PC_2);
    AnalogIn sensor2(PC_4);
    AnalogIn sensor3(PB_1);
    AnalogIn sensor4(PC_5);
    AnalogIn sensor5(PC_3);

    //bluetooth
    Serial hm10(PA_11, PA_12); //UART6 TX,RX
    char c;

    Ticker speed; //for getting meters/second

    // Setup the button interrupt for motor enable toggle
    InterruptIn button(PC_13);
    button.rise(&buttonISR);
    speed.attach(&velocity,0.05);    //read pulses and calculate speed every 0.2s
  

    hm10.baud(9600);    //initialise bluetooth

    // Initial settings for motor control
    Enable.write(0);
    Direction1 = 1;
    Direction2 = 0;
    Bipolar1 = 0;
    Bipolar2 = 0;
    Switch1 = 1;
    PD_Switch3 = 1;

    float lastMotorL = 0.6, lastMotorR = 0.6;  // Initial motor speed setting
    char SensorState = 'X', lastSensorState = 'X'; // Variable to track the last sensor state that was white

    int trackend = 2000;   //variable for stopping, may need change.
    int ble = 0;

    char sensorColors[5];

    // PWM frequency
    MotorL.period_ms(10);
    MotorR.period_ms(10);

    // Initial motor state
    MotorL.write(0.6);
    MotorR.write(0.6);
    Timer my_timer;

    while (1) {
        
        float sensorValues[5] = {
            sensor1.read() * 3.3f,  // Convert analog to voltage
            sensor2.read() * 3.3f,
            sensor3.read() * 3.3f,
            sensor4.read() * 3.3f,
            sensor5.read() * 3.3f
        };
        
        for (int i = 0; i < 5; i++) {
            sensorColors[i] = getAreaColor(sensorValues[i]);
        }

        if (Enable) {

            avgspeed = (CVL+CVR)/2;
            
            lastSensorState=SensorState;

            if (sensorColors[2] == '1') {
                diff = 0;
                 SensorState = '2';    
            } else if (sensorColors[1] == '1') {
                diff = 0.025;
                SensorState = '1';
            } else if (sensorColors[3] == '1') {
                diff = -0.025;
                SensorState = '3';
            } else if (sensorColors[0] == '1') {
               diff = 0.08;
               SensorState = '0';
            } else if (sensorColors[4] == '1') {
                diff = -0.08;
                SensorState = '4';
            }
       
            else {
                if(lastSensorState == '2'){          //stopping code; every iteration 
                       // trackend--;  
                       my_timer.start();
                                      //if sensors are off for (trackend) iterations, buggy stops
                        if(my_timer.read()>=0.40){
                            MotorR.write(1);
                            MotorL.write(1);
                            my_timer.reset();
                            return 0;   
                        }                     
                     }
                else{
                    //trackend=2000;  
                    my_timer.reset();         //reset trackend var when there was recently a sensor read
                }
            }

            if (lastSensorState==SensorState ){
                diff=1.1*diff;
                }

                lastMotorL = avgdr - diff;
                lastMotorR = avgdr + diff;
                MotorL.write(lastMotorL);
                MotorR.write(lastMotorR);

            if(hm10.readable()){
                c = hm10.getc(); //read a single character
        
                if(c == 'A'&& ble==0){
                    Direction2 = 1;
                    MotorL.write(0.5);          //speed and time for 180 turn
                    MotorR.write(0.5);
                    wait(0.8);
                    Direction2 = 0;
                    diff=0;
                    avgdr=0.6;
                    ble=1;  
                }             // rotate 180 degrees
            }
        }
    }
}



