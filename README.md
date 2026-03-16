# Autonomous Line-Following Buggy

An embedded systems project focused on designing and building an autonomous robotic buggy capable of following a track using line sensors and real-time motor control.

The system integrates hardware design, embedded software development, and control algorithms to achieve reliable autonomous navigation.

---

# Project Overview

This project involved the development of a robotic buggy capable of autonomously navigating a track that includes turns, slopes, and a 180° rotation.

The system was designed using multiple subsystems:

- Mechanical structure and chassis design
- Custom sensor PCB for line detection
- Motor drive and speed control
- Embedded software for navigation and control

The final robot successfully completed the full track during testing.

---

# System Architecture

The buggy consists of four major subsystems:

1. Mechanical SystemChassis design, wheel configuration, and component placement.
2. Electronic SystemSensor PCB with five infrared line sensors and motor driver electronics.
3. Control SystemLine following algorithm using bang-bang control for steering and speed regulation.
4. Embedded Software
   Firmware controlling sensors, motors, speed feedback, and Bluetooth communication.

---

# Hardware Design

## Mechanical Design

The chassis was redesigned during the project to improve stability and reliability.

Key improvements:

- Replacement of two castor wheels with a single central castor wheel
- Larger chassis to accommodate all electronics
- Improved battery placement to reduce movement during operation

## Sensor PCB

A custom PCB was designed to support five line sensors arranged in a semicircle.

This configuration allows early detection of track deviations and enables the buggy to correct its direction before leaving the track.

The PCB also includes a Darlington array to switch sensor groups in order to reduce power consumption.

---

# Embedded Software

The firmware was written using C/C++ on an embedded microcontroller platform.

The main components of the software include:

- Hardware initialization
- Sensor data acquisition
- Speed estimation using wheel encoders
- Direction control using sensor readings
- Bluetooth command interface
- Stopping logic at the end of the track

Wheel speed is calculated by counting encoder pulses and estimating velocity using the wheel circumference and sampling period.

---

# Control Algorithm

The original design intended to use a PID controller for precise control.
However, due to time constraints and implementation complexity, the final system adopted a **bang-bang control strategy**.

This method adjusts motor duty cycles depending on which sensor detects the line.

Although less smooth than PID control, this approach proved more reliable under the project time constraints.

---

# System Features

- Autonomous line following
- Real-time speed monitoring
- Sensor-based steering control
- Bluetooth-triggered 180° turn
- Encoder-based speed feedback

---

# Results

The buggy was tested across multiple runs on the final track.

The final system was able to:

- Follow the track successfully
- Climb and descend slopes
- Perform a 180° turn using Bluetooth
- Complete the full course with minimal intervention

---

# Future Improvements

Potential improvements include:

- Implementing full PID control
- Optimizing sensor spacing for smoother tracking
- Refactoring firmware for better modularity
- Improving battery reliability and power management

---

# Project Images

## Final Buggy

![Buggy Top](images/buggy_top_view.png)

![Buggy Side](images/buggy_side_view.png)

![Buggy Front](images/buggy_front_view.png)

## Sensor PCB

![Sensor PCB](images/sensor_pcb.png)
