Cable Wire Test

This is an Arduino program based on C++.
It assumes the use of the Arduino Mega 2560.
It can be run using the PlatformIO Monitoring Tool.

Goal:
It tests the integrety of cables with wires up to 8 (max analogpins of an Arduino Mega 2560).
The different wires have to be connected following this pattern (A0 --> A8 | A1 --> A9 | etc.).
In the current Version Alpha 1.0 you have to declare in the code in the variabel "wires" how many wires you have.
The current code will run every 10 sec and prints in the terminal using the Serial output.

A more stable and improved version will follow (cuurent version: Alpha 1.0)

Developer: @Wendel2008
