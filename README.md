# Spray Can Plotter

A large DIY plotter for spray cans. Powered by cordless drills and an Arduino as a brain. This readme aims to explain how the plotter works. 

## TECHNICAL DESCRIPTION OF THE PLOTTER'S MECHANICS

The spray can plotter has a printing area of 2.20m * 2.00m. It is built as a classic XY gantry mechanism. The X-axis consists of 2 aluminum tubes. Two short pieces of PVC electric conduit glide on the pipes and hold the spray can mount. The can mount is pulled back and forth by a clothesline running over two pulleys, one of which is driven by a cordless screwdriver. The nozzle of the spray can is operated by a lever from an RC servo motor.

The entire X-axis, including the spray head, is suspended on the left and right via two cords that run over two pulleys at the of top frame to the bottom of the plotter, where they are wound on a long aluminum tube. Winding and unwinding the two cords by a second cordless screwdriver (Y-motor) moves the entire X-axis up and down. As the cords wind and unwind at the same speed on both sides, the X-axis does not become wedged. For further stabilization, the X-axis slides along two vertical aluminum tubes; Here, too, short sections of PVC pipe serve as linear bearings.

## ELECTRONICS

The two cordless screwdrivers are DC gear motors with a rated voltage of 18V. These are each switched via an H-bridge, a standard electronic circuit that looks like an "H" in the circuit diagram. This allows the polarity of the current applied to the motors to be reversed, so that the cordless screwdrivers can be turned in both directions. Variable speed control works with pulse width modulation. This means the motors are permanently switched on and off very quickly, whereby the length of the "on" phase varies in relation to the "off" phase.

The cordless screwdriver motors have no position feedback. In order to be able to move to specific coordinates, a black and white encoder strip is attached to each axis. The strip is read by two photo transistors per axis. Those optical sensors are used to count the relative position of the spray head. In order to set the absolute origin of the coordinate system, the spray head calibrates itself by moving to the upper left corner until it hits end switches.

The two H-bridges (one for each axis) are controlled by an Arduino-Nano microcontroller board. The microcontroller communicates with the outside world via a serial interface and also via I/O pins. In the case of the plotter controller, the input pins read the photo transistors of the encoders, as well as the limit switches of the two axes. The program uses this information to continuously calculate the actual position of the spray head. The spray head is moved by voltage pulses on the output pins, which in turn are connected to the control inputs of the H-bridges.

## CODE

The program running on the controller works relatively simple: the controller receives a data packet via the serial interface. This contains, for example, target coordinates to which the printhead should move. The controller then compares the current position of the spray head with the target position, and as long as both positions differ, the cordless motors move the spray head in the direction of the target position. Once it has been reached, the plotter controller signals via the serial interface return channel that it is ready to receive new coordinates.

For interactive real-time control, the data package can also contain target speeds for the X and Y motors. These are forwarded directly to the corresponding output pins. In this case, the controller still counts the position of the spray head and ensures that it only moves within a defined area.

## DATA SOURCES

Various data sources can now be connected to the plotter via the serial interface of the plotter, such as a small SD card reader with display. On the SD card are text files with coordinates, which are then transferred line by line via the serial interface. Of course, a laptop can also be connected directly,  e.g. in order to be able to remotely control the plotter in real time worldwide via the Internet.

## REALTIME CONTROL VIA INTERNET

The real time control of the plotter via the Internet works via the WebSocket protocol. With this protocol, data can be transferred from the web browser to a web server without having to reload the website. The server can also transfer data back, which is then processed by a JavaScript in the browser. For the plotter control, all participants transmit directionional information for the spray head approx. 50 times per second and the server replies to all participants with a list of all directions received. This simple echo program is written in Node.js.

The data transfer between the web browser and the plotter then takes place using the Web Serial API. This programming interface allows the web browser to access the serial port of the computer and throught that, the plotter. The JavaScript in the web browser receives target directions from every participant. It calculates a democratic average and sends the spray head to this direction, similar to operating an [Ouija board](
https://en.wikipedia.org/wiki/Ouija). 



