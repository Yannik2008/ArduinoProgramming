Cable Wire Test

This is an Arduino program based on C++.
It assumes the use of the Arduino Mega 2560.
It can be run using the PlatformIO Monitoring Tool.

Goal:
It tests the integrety of cables with theoretically unlimited wires.
The different wires have to be connected following this pattern (A0 --> A8 | A1 --> A9 | etc.).
In the current Version 1.3 you have to declare in the code in the variabel "wires" how many wires you have.
The current code will run every 5 sec and prints in the terminal using the Serial output.

This version uses the digital outputpins of port a (cuurent version: 1.3)

Developer: @Wendel2008
