EE175-SeniorDesign-ASSP
=======================

An autonomous system on which any necessary sensor can be mounted to monitor atmospheric data in an area.
 1. an Autonomous Ground Vehicle (AGV) will travel to waypoints set by the user to collect data.
 2. a base-station
will be a docking station for the vehicle to recharge with power collected from solar cells.

Developers
----------
* [Brett Fontaine](mailto:bfont001@ucr.edu)
* [Gustavo Blanco](mailto:gblan002@ucr.edu)
* [Russell Perry](mailto:rperr002@ucr.edu)

Code Structure
--------------
* atmega_one
contains the code that will be loaded onto the atmega32 that will handle the GPS Bluetooth communication, and path planning
* atmega_two
contains the code that will be loaded onthe the atmega32 that will handle the motor control and sensor data
