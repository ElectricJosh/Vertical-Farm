# Vertical-Farm
Vertical Farm project with DesignSpark, Wurth Elektronik, OKdo, and RS Pro.
See DesignSpark for the full writeup.

The vertical farm automation system is made up of three parts:
1. ROCK CM3 IO board running Debian Linux, that acts as the central controller and programmed in Python and a tiny amount of BASH startup script.
2. Wurth Elektronik Sensor FeatherWing, that monitors and send environmental data over USB, programmed with Visual Studio Code and the Platformio extension. See directory for details.
3. Arduino Uno with relay shield, that controls the pump and fans when prompted over USB, programmed in the usual Arduino IDE.
