# Motor Controller

A guy had an idea to build a contraption for a specific purpose. The core of the contraption was to control the speed of a stepper motor very accurately. He asked me to help with the software for a ESP32 that would control the motor and other peripheral devices. This is the code for that.

The experience of working with an ESP32 was interesting. It was very weird running a web server on an embedded device.

The web server updates the web page with the current state of the devices using a web socket.

The ESP32 code is written in C/C++ and was developed with PlatformIO.