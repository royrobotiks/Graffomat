# Spray Can Plotter

I made a large DIY plotter for spray cans. It is powered by cordless drills and has an Arduino Nano as a brain. This README explains how the plotter works and here you can also find all the source codes for this plotter.

<img alt="DIY spray can plotter" src="https://niklasroy.com/diary/8/Niklas_Roy_Diary_Lores_818.jpg"> 

## TECHNICAL DESCRIPTION OF THE PLOTTER'S MECHANICS

The spray can plotter has a printing area of 2.20m * 2.00m. It is built as a classic XY gantry mechanism. The X-axis consists of 2 aluminum tubes. Two short pieces of PVC electric conduit glide on the pipes and hold the spray can mount. The can mount is pulled back and forth by a clothesline running over two pulleys, one of which is driven by a cordless screwdriver. The nozzle of the spray can is operated by a lever from an RC servo motor.

<img alt="cordless drill and pulley" src="https://niklasroy.com/diary/3/Niklas_Roy_Diary_Lores_378.jpg" width="49%" height="49%"> <img alt="first version of the spray can mount" src="https://niklasroy.com/diary/3/Niklas_Roy_Diary_Lores_397.jpg" width="49%" height="49%"> 

The entire X-axis, including the spray head, is suspended on the left and right side of the plotter frame via two cords that run over two pulleys at the of top frame to the bottom of the plotter, where they are wound on a long aluminum tube. Winding and unwinding the two cords by a second cordless screwdriver (Y-motor) moves the entire X-axis up and down. As the cords wind and unwind at the same speed on both sides, the X-axis does not become wedged. For further stabilization, the X-axis slides along two vertical aluminum tubes; Here, too, short sections of PVC pipe serve as linear bearings.

<img alt="Y-axis motor" src="https://niklasroy.com/diary/4/Niklas_Roy_Diary_Lores_415.jpg" width="32%" height="32%"> <img alt="Y-axis suspended on a pulley" src="https://niklasroy.com/diary/4/Niklas_Roy_Diary_Lores_414.jpg" width="32%" height="32%"> <img alt="right side of the Y-axis" src="https://niklasroy.com/diary/3/Niklas_Roy_Diary_Lores_390.jpg" width="32%" height="32%"> 

## ELECTRONICS

The two cordless screwdrivers are DC gear motors with a rated voltage of 18V. These are each switched via an [H-bridge](https://en.wikipedia.org/wiki/H-bridge), a standard electronic circuit that looks like an "H" in the circuit diagram. This allows the polarity of the current applied to the motors to be reversed, so that the cordless screwdrivers can be turned in both directions. Variable speed control works with [pulse-width modulation](https://en.wikipedia.org/wiki/Pulse-width_modulation). This means the motors are permanently switched on and off very quickly, whereby the duration of the "on" phase varies in relation to the "off" phase.

The cordless screwdriver motors have no position feedback. In order to be able to move to specific coordinates, a [black and white encoder strip](https://en.wikipedia.org/wiki/Incremental_encoder) is attached to each axis. The strip is read by two photo transistors per axis. Those optical sensors are used to count the relative position of the spray head. In order to set the absolute origin of the coordinate system, the spray head calibrates itself by moving to the upper left corner until it hits end switches when the plotter is turned on.

<img alt="DIY linear optical encoder" src="https://niklasroy.com/diary/4/Niklas_Roy_Diary_Lores_404.jpg" width="32%" height="32%"> <img alt="Encoder schematics" src="https://niklasroy.com/diary/3/Niklas_Roy_Diary_Lores_395.jpg" width="32%" height="32%"> <img alt="Encoder schematics" src="https://niklasroy.com/diary/3/Niklas_Roy_Diary_Lores_391.jpg" width="32%" height="32%"> 

The two H-bridges (one for each axis) are controlled by an Arduino-Nano microcontroller board. The microcontroller communicates with the outside world via a serial interface and also via I/O pins. In the case of the plotter controller, the plotter receives commands via the serial port, while the I/O pins are connected with the plotter hardware. Via its input pins, controller reads the Voltage on the photo transistors of the encoders, as well as the states of the limit switches. The program uses this information to continuously calculate the actual position of the spray head. The spray head is moved by Voltage pulses on the output pins, which in turn are connected to the control inputs of the H-bridge modules.

## FIRMWARE

The program which runs on the controller works relatively simple: the controller receives a data packet via the serial interface. This contains, for example, target coordinates to which the printhead should move. The controller then compares the current position of the spray head with the target position, and as long as both positions differ, the motors move the spray head in the direction of the target position. Once the target has been reached, the plotter controller signals via the serial interface return channel that it is ready to receive a new command with new coordinates.

For interactive real-time control, the data package can also contain target speeds instead of target coordinates for the X and Y motors. The speed information is forwarded directly to the corresponding output pins. In this case, the controller still keeps track of the position of the spray head and ensures that it only moves within a defined area.

<img alt="" src="https://niklasroy.com/diary/8/Niklas_Roy_Diary_Lores_812.jpg" width="49%" height="49%">  <img alt="" src="https://niklasroy.com/diary/8/Niklas_Roy_Diary_Lores_815.jpg" width="49%" height="49%"> 

## SERIAL DATA SOURCES

Various data sources can be connected to the plotter via its serial interface, such as a small SD card reader with display. Stored on the SD card are text files with coordinates, which are then transferred line by line via the serial interface. Of course, a laptop can also be connected directly,  e.g. in order to be able to remotely control the plotter worldwide in real-time via the Internet.

<img alt="" src="https://niklasroy.com/diary/4/Niklas_Roy_Diary_Lores_462.jpg" width="49%" height="49%">  <img alt="" src="https://niklasroy.com/diary/6/Niklas_Roy_Diary_Lores_664.jpg" width="49%" height="49%"> 

## REALTIME CONTROL VIA INTERNET

The real time control of the plotter via the Internet works via the [WebSocket protocol](https://en.wikipedia.org/wiki/WebSocket). With this protocol, data can be transferred from the web browser to a web server without having to reload the website. The server can also transfer data back, which is then processed by a JavaScript in the browser. For the plotter control, all participants transmit directionional information for the spray head approx. 50 times per second and the server replies to all participants with a list of all directions received. This simple echo program is written in Node.js.

The data transfer between the web browser and the plotter then takes place using the [Web Serial API](https://developer.mozilla.org/en-US/docs/Web/API/Web_Serial_API). This programming interface allows the web browser to access the serial port of the computer and throught that, the plotter. The JavaScript in the web browser receives target directions from every participant. It calculates a democratic average and sends the spray head to this direction, similar to operating an [Ouija board](
https://en.wikipedia.org/wiki/Ouija). 

## ELECTRONIC PARTS LIST

- 1x 12V 18Ah lead-acid battery
- 2x 18V cordless drills
- 2x BTS7960B 43A high power H-bridge driver modules
- 1x Arduino Nano
- 1x XL4016 step down converter module DC-DC 5-40V
- 1x RC servo
- 4x BPW42 photo transistors
- 4x 220 Ohm resistor
- 4x 1K2 Ohm resistor
- 2x 1000 uF capacitor
- 4x arcade buttons (end switches and interface box)
- 2x potis
- 1x switch

## PLOTTER IN ACTION / TWITTER THREADS
-  [Hello World](https://twitter.com/royrobotiks/status/1382610619300478977)
-  [CMYK Test & Spontankollaboration](https://twitter.com/royrobotiks/status/1388173453719654405)
-  [#ALGO vs OSK](https://twitter.com/royrobotiks/status/1393245371514621956)
-  [DM von @pixtxa](https://twitter.com/royrobotiks/status/1394211939866357760)
-  [DIY needle cap](https://twitter.com/royrobotiks/status/1397196201942978571)

