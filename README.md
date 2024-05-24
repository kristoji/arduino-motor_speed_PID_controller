# Closed Loop Control of a DC Motor in Arduino

This project implements a closed loop control of a DC motor using an Arduino. The motor is driven by an H bridge and the velocity is measured using an encoder. The control algorithm is a simple PID controller that adjusts the voltage applied to the motor so that the measured velocity matches the desired one.  
The arduino server talks to a client running on a PC that sends the desired velocity and receives the measured one. The client can also plot the measured and desired velocity in real time. The communication between the client and the server is done using the serial port and it is handled via interrupts.

The project is part of the course [Operative Systems](https://gitlab.com/grisetti/sistemi_operativi_2023_24) held by Prof. Grisetti at the University of Rome, La Sapienza.

## Hardware Configuration

For this project I used the following hardware components:
- Arduino Mega 2560
- DC Motor with encoder
- H bridge [MR001-004.1](https://www.gotronic.fr/pj-983.pdf)
- Power supply 12V
<!-- - JOYSTICK??? -->

<!-- TODO cabling imges -->

## Usage

<!-- TODO build and loading -->

## Results

<!-- TODO imgs of final result  -->