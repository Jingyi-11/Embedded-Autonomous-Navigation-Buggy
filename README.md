
# Autonomous Line-Following Buggy

An embedded systems project that designs and builds an autonomous buggy capable of following a track using optical sensors and motor control algorithms.

This project integrates **mechanical design, electronics, control algorithms, and embedded software** to create a fully functional autonomous vehicle.

---

# Overview

The objective of this project is to design and implement a robotic buggy that can autonomously navigate a track containing turns, slopes, and a 180° rotation.

The system uses **infrared reflectance sensors** to detect the track and adjust the motor speeds accordingly. A microcontroller processes the sensor readings and controls the motors through PWM signals to maintain alignment with the path.

The project involved the complete workflow of an embedded system, including:

- System architecture design
- Mechanical chassis design
- Sensor PCB design
- Embedded software development
- Control algorithm implementation
- System testing and optimisation

---

# Final System

### Buggy Design



The final buggy integrates the microcontroller, motor driver, sensor PCB, and power system into a compact two-level chassis.

---

# System Architecture

<img src="images/software_design.png" width="600">

The system consists of four main subsystems:

- Mechanical structure
- Sensor electronics
- Motor control system
- Embedded software

Each subsystem was designed and iteratively improved during the project.

---

# Mechanical Design

<img src="images/chassis_design.png" width="500">

The chassis structure was redesigned during development to improve stability and component placement.

Key improvements:

- Replaced dual caster wheels with a **single supported caster wheel**
- Increased chassis size to accommodate electronics
- Improved mounting of the battery pack
- Reduced vibration affecting steering performance

The redesigned chassis significantly improved reliability and maneuverability.

---

# Electronics

## Sensor PCB

<img src="images/pcb_schematic_sensor.png" width="400">
<img src="images/pcb_layout_sensor.png" width="400">

The custom PCB contains **five infrared reflectance sensors** arranged in a semicircle to detect the line.

<img src="images/TCRT5000.jpg" width="300">

Features:

- TCRT5000 line sensors
- Darlington array switching for sensor control
- Optimised spacing for detecting track curvature
- Interface with the microcontroller for real-time readings

This configuration allows the buggy to detect deviations from the track and adjust its direction accordingly.

---

# Control System

The control system determines motor speed based on sensor readings.

Initially, a **PID controller** was considered due to its smooth response and precise error correction. However, due to time constraints and tuning complexity, the system was implemented using a **bang-bang control strategy**.

Bang-bang control adjusts motor PWM duty cycles depending on which sensor detects the line.

<img src="images/pwm_control.jpg" width="500">

This method provides:

- fast response
- simple implementation
- reliable performance for the track environment

---

# Software Implementation

<img src="images/software_design.png" width="600">

The embedded software was implemented using **Keil Mbed (C++)**.

Main software components:

### Hardware Initialisation

- GPIO configuration
- motor PWM setup
- sensor input configuration

### Sensor Processing

Sensor values are converted into digital readings to determine whether the buggy is on or off the track.

### Speed Control

Wheel encoders are used to calculate wheel speed and adjust the base motor speed.

### Direction Control

Motor speeds are modified based on which sensors detect the track.

### Stopping Logic

When all sensors detect the background (track end), a timer determines when the buggy should stop.

### Bluetooth Command

A Bluetooth command triggers the **180° rotation maneuver**.

---

# Testing and Results

<img src="images/buggy_design.png" width="500">

Extensive testing was performed to improve the buggy's performance.

Testing stages included:

- Motor characterisation
- Sensor calibration
- Line-following experiments
- Speed control testing
- Track performance validation

The buggy was eventually able to:

- follow the track autonomously
- navigate turns
- climb and descend slopes
- complete the full course

Despite some oscillations and speed instability, the final system successfully achieved the main project goal.

The final project demonstrated a working autonomous navigation system integrating hardware and software components. :contentReference[oaicite:0]{index=0}

---

# Project Structure
