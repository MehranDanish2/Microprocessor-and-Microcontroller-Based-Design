# Microprocessor-and-Microcontroller-Based-Design
📟 Frequency Measurement Device using PIC18F452
Microprocessor and Microcontroller Based Design Project – NUST CEME

🔧 Overview
This project implements a Frequency Measurement Device using the PIC18F452 microcontroller. It is designed to measure the frequency of an external input signal and display the result on an LCD module in real-time. The system is developed as part of the Microprocessor and Microcontroller-Based Design course.

📌 Features
Measures frequency of square wave or periodic digital signals

Displays frequency on a 16x2 LCD

Accurate timing using Timer1 and interrupt-based capture

User-friendly and low-power design

Can be extended to measure other parameters (e.g., period, RPM)

⚙ Hardware Components
PIC18F452 Microcontroller

16x2 Character LCD Display

Crystal Oscillator (typically 20MHz)

Input signal source (function generator or sensor output)

Power Supply (5V regulated)

Push button (optional reset)

Breadboard and jumper wires for prototyping  🧠 Working Principle
Timer1 is configured as a counter for incoming pulses.

A gate time (e.g., 1 second) is defined using another timer (e.g., Timer0).

After the gate time, the pulse count is captured and converted into frequency.

The result is displayed on the LCD.

This method provides reasonably accurate frequency measurements for most low-to-mid-frequency digital signals.

🖥 Software & Tools Used
MPLAB X IDE
MikroC
XC8 Compiler

Proteus (for simulation)

PICKit3 (for programming hardware)
