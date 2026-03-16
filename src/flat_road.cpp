#include "mbed.h"
#include "C12832.h"
#include "QEI.h"

// Define motor output pins
PwmOut MotorL(PC_8), MotorR(PC_6);
// Define digital outputs for motor control
DigitalOut Enable(PA_15);
DigitalOut Direction1(PA_14), Direction2(PA_13);
DigitalOut Bipolar1(PB_15), Bipolar2(PB_2);
DigitalOut Switch1(PC_11), PD_Switch3(PD_2);

// Encoders for each motor
QEI encoderLeft(PC_10, PC_12, NC, 256);
QEI encoderRight(PB_14, PB_13, NC, 256);

// Interrupt service routine for button press
void buttonISR() {
    Enable = !Enable;  // Toggle enable state of the motors
}

// Class definition for PID controller
class PIDController {
private:
    float kp, ki, kd; // PID coefficients
    float setpoint; // Target value for control
    float integral, prevError; // State for integral and previous error
    float dt; // Time step for derivative calculation

public:
    PIDController(float kp, float ki, float kd, float dt) 
    : kp(kp), ki(ki), kd(kd), setpoint(0), integral(0), prevError(0), dt(dt) {}

    void setSetpoint(float sp) {
        setpoint = sp;
    }

    float getSetpoint() {
        return setpoint;
    }
    
    float calculate(float input) {
        float error = setpoint - input;
        integral += error * dt;
        float derivative = (error - prevError) / dt;
        prevError = error;
        return kp * error + ki * integral + kd * derivative;
    }
};

// Analog inputs for sensors
AnalogIn sensor1(PC_2);
AnalogIn sensor2(PC_4);
AnalogIn sensor3(PB_1);
AnalogIn sensor4(PC_5);
AnalogIn sensor5(PC_3);

// LCD Display initialization
C12832 lcd(D11, D13, D12, D7, D10);

// Motor PWM outputs initialized again (redundant)
PwmOut motorLeft(PC_8), motorRight(PC_6);

// Instantiate PID controllers with specific coefficients
PIDController pidLeft(1.0, 0.1, 0.05, 0.1);
PIDController pidRight(1.0, 0.1, 0.05, 0.1);

// Function to determine area color based on sensor voltage
char getAreaColor(float voltage) {
    const float whiteThreshold = 1.5; // Threshold voltage for white
    const float blackThresholdLow = 0.0, blackThresholdHigh = 1.5; // Range for black
    if (voltage > whiteThreshold) return '1'; // White
    else if (voltage > blackThresholdLow && voltage < blackThresholdHigh) return '0'; // Black
    else return 'X'; // Undefined
}

float lastMotorL = 0.6;  // Initial motor speed setting
float lastMotorR = 0.6;
char lastSensorState = 'X'; // Variable to track the last sensor state that was white

int main() {
    // Setup the button interrupt for motor enable toggle
    InterruptIn button(D4);
    button.rise(&buttonISR);

    // Initial settings for motor control
    Enable.write(0);
    Direction1 = 1;
    Direction2 = 0;
    Bipolar1 = 0;
    Bipolar2 = 0;
    Switch1 = 1;
    PD_Switch3 = 1;

    // PWM frequency
    MotorL.period_ms(10);
    MotorR.period_ms(10);

    // Initial motor state
    MotorL.write(1);
    MotorR.write(1);

    // Set PID controller target speed (encoder pulses per second)
    float targetSpeed = 300;
    pidLeft.setSetpoint(targetSpeed);
    pidRight.setSetpoint(targetSpeed);
    bool endingDetected = false;

    while (1) {
        if (Enable) {
            float sensorValues[5] = {
                sensor1.read() * 3.3f,  // Convert analog to voltage
                sensor2.read() * 3.3f,
                sensor3.read() * 3.3f,
                sensor4.read() * 3.3f,
                sensor5.read() * 3.3f
            };

            char sensorColors[5];
            bool allSensorsOff = true;
            for (int i = 0; i < 5; i++) {
                sensorColors[i] = getAreaColor(sensorValues[i]);
                if (sensorColors[i] != '0') {
                    allSensorsOff = false;
                }
            }

            lcd.cls();
            lcd.locate(0, 0);
            lcd.printf("S1:%c S2:%c S3:%c S4:%c S5:%c", sensorColors[0], sensorColors[1], sensorColors[2], sensorColors[3], sensorColors[4]);

            bool anyWhiteDetected = false;
            for (int i = 0; i < 5; i++) {
                if (sensorColors[i] == '1') {
                    anyWhiteDetected = true;
                    break;
                }
            }

            if (!anyWhiteDetected) {
                MotorL.write(lastMotorL);
                MotorR.write(lastMotorR);
            } else {
                if (sensorColors[2] == '1') {
                    lastMotorL = 0.65;
                    lastMotorR = 0.65;
                } else if (sensorColors[1] == '1') {
                    lastMotorL = 0.6;
                    lastMotorR = 0.8;
                } else if (sensorColors[3] == '1') {
                    lastMotorL = 0.8;
                    lastMotorR = 0.6;
                } else if (sensorColors[0] == '1') {
                    lastMotorL = 0.55;
                    lastMotorR = 0.9;
                } else if (sensorColors[4] == '1') {
                    lastMotorL = 0.9;
                    lastMotorR = 0.55;
                }

                // Handle specific sensor conditions for turning
                if (allSensorsOff) {
                    if (lastSensorState == '1') {
                        lastMotorL = 0.8;
                        lastMotorR = 0.6;
                    } else if (lastSensorState == '3') {
                        lastMotorL = 0.6;
                        lastMotorR = 0.8;
                    } else if (lastSensorState == '2') {  // stop the buggy
                        endingDetected = true; 
                    }
                }

                // Update the lastSensorState based on the current sensor values
                if (sensorColors[1] == '1') lastSensorState = '1';
                else if (sensorColors[3] == '1') lastSensorState = '3';
                else if (sensorColors[2] == '1') lastSensorState = '2';
                else if (allSensorsOff) lastSensorState = 'X'; // Reset if all sensors are off

                // Drive the motors with the updated speeds
                MotorL.write(lastMotorL);
                MotorR.write(lastMotorR);
            }

            // Example condition to start slowing down
            if (endingDetected) {
                while (pidLeft.getSetpoint() > 0) {
                    float stepDown = 5;  // Decrease speed by 5 pulses per second per loop iteration
                    float newSetpoint = pidLeft.getSetpoint() - stepDown;
                    pidLeft.setSetpoint(newSetpoint);
                    pidRight.setSetpoint(newSetpoint);

                    // Calculate PID output
                    float leftSpeed = pidLeft.calculate(encoderLeft.getPulses());
                    float rightSpeed = pidRight.calculate(encoderRight.getPulses());

                    // Adjust motor speeds
                    MotorL.write(leftSpeed);
                    MotorR.write(rightSpeed);

                    wait(0.1); // Delay to slow down the rate of decrease
                }
            }
        }
    }
}