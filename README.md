# GRAFFOMAT

I made a large plotter for spray cans. It is powered by two old cordless drills and it has an Arduino Nano as a brain. This README explains how the plotter works and of course, you can find all the codes for the machine in this repo. If you are interested in what I've done with the plotter, have a look at [the documentation on my website](https://niklasroy.com/graffomat).

<img alt="DIY spray can plotter" src="https://github.com/royrobotiks/SprayCanPlotter/blob/main/plotter.jpg"> 

## TECHNICAL DESCRIPTION OF THE PLOTTER'S MECHANICS

When making the machine, I based all decisions on a few principles: I wanted the plotter to be cheap, lightweight and I also wanted to build it very fast. Everything is basically just zip‑tied together and besides the electronic parts, I tried to use only stuff which I had anyway in my basement or which I could easily get at the hardware store. 

The plotter has a printing area of 2.20m * 2.00m. It is built as a classic XY gantry mechanism. The X-axis consists of 2 aluminum tubes. Two short pieces of PVC electric conduit glide on the pipes and hold the spray can mount. The can mount is pulled back and forth by a clothesline running over two pulleys, one of which is powered by a cordless drill. The nozzle of the spray can is pushed by an RC servo motor via a lever.

<img alt="cordless drill and pulley" src="https://niklasroy.com/diary/3/Niklas_Roy_Diary_Lores_378.jpg" width="49%" height="49%"> <img alt="first version of the spray can mount" src="https://niklasroy.com/diary/3/Niklas_Roy_Diary_Lores_397.jpg" width="49%" height="49%"> 

The X-axis assembly, including the spray head, is suspended on the left and right side of the plotter frame via two cords that run over two pulleys at the top of the frame. The cords are wound around a long aluminum tube which is located at the bottom of the frame. Winding and unwinding the two cords by a second cordless screwdriver (Y-motor) moves the X-axis up and down. As the cords wind and unwind at the same speed on both sides, the X-axis does not become wedged. For further stabilization, the X-axis slides along two vertical aluminum tubes. Again, two short pieces of PVC pipe serve as linear bearings.

<img alt="1st generation can mount" src="https://niklasroy.com/diary/3/Niklas_Roy_Diary_Lores_373.jpg" width="24%" height="24%"> <img alt="Y-axis motor" src="https://niklasroy.com/diary/4/Niklas_Roy_Diary_Lores_415.jpg" width="24%" height="24%"> <img alt="Y-axis suspended on a pulley" src="https://niklasroy.com/diary/4/Niklas_Roy_Diary_Lores_414.jpg" width="24%" height="24%"> <img alt="right side of the Y-axis" src="https://niklasroy.com/diary/3/Niklas_Roy_Diary_Lores_390.jpg" width="24%" height="24%"> 

## ELECTRONICS

The two cordless drills are DC gear motors with a rated voltage of 18V. Each of them is switched via an [H-bridge](https://en.wikipedia.org/wiki/H-bridge), a standard electronic circuit that looks like an "H" in the schematic. The H-bridges allow the polarity of the current to be reversed, so that the drills can be turned in both directions. Variable speed control works with [pulse-width modulation](https://en.wikipedia.org/wiki/Pulse-width_modulation). This means the motors are switched on and off very quickly, whereby the duration of the "on" phase varies in relation to the duration of the "off" phase.

The cordless drill motors have no position feedback. The cordless drill motors have no position feedback. In order to be able to move the spray can to specific coordinates, a [black and white encoder strip](https://en.wikipedia.org/wiki/Incremental_encoder) is attached to each axis. The strips are made out of adhesive tape and they are read by by two photo transistors per axis. Those optical sensors are used to count the relative position of the spray head. In order to set the absolute origin of the coordinate system, the spray head calibrates itself by moving to the upper left corner until it hits end switches.

<img alt="DIY linear optical encoder" src="https://niklasroy.com/diary/4/Niklas_Roy_Diary_Lores_404.jpg" width="32%" height="32%"> <img alt="Encoder schematics" src="https://niklasroy.com/diary/3/Niklas_Roy_Diary_Lores_395.jpg" width="32%" height="32%"> <img alt="Encoder schematics" src="https://niklasroy.com/diary/3/Niklas_Roy_Diary_Lores_391.jpg" width="32%" height="32%"> 

<img alt="" src="https://www.niklasroy.com/diary/11/Niklas_Roy_Diary_Lores_1138.jpg" width="49%" height="49%"> <img alt="" src="https://www.niklasroy.com/diary/11/Niklas_Roy_Diary_Lores_1137.jpg" width="49%" height="49%">

## FIRMWARE

The program which runs on the controller works relatively simple: it receives a data packet via the Serial interface. This contains, for example, target coordinates for the print head. The controller then compares the current position of the spray head with the destination position and as long as both positions differ, the motors move the spray head towards the target position. Once the destination has been reached, the plotter controller signals that it is ready to receive a new command with new coordinates.

For interactive real-time control, the data package can also contain target speeds instead of target coordinates for the X- and Y-motors. The speed information is forwarded directly to the H-bridges. The controller still keeps track of the position of the spray head and ensures that it does not crash into the ends of the frame.

<img alt="" src="https://niklasroy.com/diary/8/Niklas_Roy_Diary_Lores_812.jpg" width="49%" height="49%">  <img alt="" src="https://niklasroy.com/diary/8/Niklas_Roy_Diary_Lores_815.jpg" width="49%" height="49%"> 

## SERIAL DATA SOURCES

Various data sources can be connected to the plotter via its serial interface, such as a small SD card reader with display. Stored on the SD card are text files with coordinates, which are then transferred line by line via the serial interface. Of course, a laptop can also be connected directly, e.g. in order to be able to remotely control the plotter worldwide in real time via the Internet.

<img alt="" src="https://niklasroy.com/diary/4/Niklas_Roy_Diary_Lores_462.jpg" width="49%" height="49%">  <img alt="" src="https://niklasroy.com/diary/6/Niklas_Roy_Diary_Lores_664.jpg" width="49%" height="49%"> 

## REALTIME CONTROL VIA INTERNET

The real time control of the plotter via the Internet works via the [WebSocket protocol](https://en.wikipedia.org/wiki/WebSocket). With this protocol, data can be transferred from the web browser to a web server without having to reload the website. The server can also return data to the browser, which is then processed by JavaScript. For the plotter remote control, all participants transmit directionional information for the spray head approximately 50 times per second and the server replies to all participants with a list of all directions received. It’s a simple “echo server” which I wrote in Node.js.

<img alt="" src="https://niklasroy.com/diary/7/Niklas_Roy_Diary_Lores_745.jpg" width="32%" height="32%"> <img alt="" src="https://niklasroy.com/diary/7/Niklas_Roy_Diary_Lores_747.jpg" width="32%" height="32%"> <img alt="" src="https://niklasroy.com/diary/7/Niklas_Roy_Diary_Lores_751.jpg" width="32%" height="32%"> 

The data transfer between the web browser and the plotter works with the [Web Serial API](https://developer.mozilla.org/en-US/docs/Web/API/Web_Serial_API). As the name suggests, this API allows the web browser to access the serial port of the computer and through that, to access the plotter. The JavaScript in the web browser calculates a democratic average of the inputs of all participants and sends the spray head to this direction. Everyone who controls the plotter has the same influence. The experience is a bit like the digital equivalent of operating an [Ouija board](
https://en.wikipedia.org/wiki/Ouija). 

## HARDWARE

I built the plotter without drawing any plans or schematics beforehand, therefore I cannot provide circuit diagrams. However, below is a list of parts and modules, which I used. If you plan to build something similar, this list might be useful. 

### ELECTRONIC PARTS LIST
- 1x 12V 18Ah lead-acid battery
- 2x 18V cordless drills
- 2x BTS7960B 43A high power H-bridge driver modules
- 1x Arduino Nano
- 1x XL4016 step down converter module DC-DC 5-40V
- 1x RC Servo
- 4x BPW42 photo transistors
- 4x LED's
- 8x 220 Ohm resistor
- 4x 1K2 Ohm resistor
- 2x 1000 uF capacitor
- 4x arcade buttons (end switches and interface box)
- 2x potis
- 1x switch

## PLOTTER IN ACTION

### DOCUMENTATION ON MY WEBSITE
- [niklasroy.com/graffomat](https://niklasroy.com/graffomat/)

### TWITTER THREADS
-  [Hello World](https://twitter.com/royrobotiks/status/1382610619300478977)
-  [CMYK Test & Spontankollaboration](https://twitter.com/royrobotiks/status/1388173453719654405)
-  [#ALGO vs OSK](https://twitter.com/royrobotiks/status/1393245371514621956)
-  [DM von @pixtxa](https://twitter.com/royrobotiks/status/1394211939866357760)
-  [DIY needle cap](https://twitter.com/royrobotiks/status/1397196201942978571)

### VIDEOS
- [GRAFFOMAT video (Youtube)](https://www.youtube.com/watch?v=Pz5CftU_bJw)
- [GRAFFOMAT video (archive.org)](https://archive.org/details/graffomat)
- [Namave animation (Youtube)](https://www.youtube.com/watch?v=AOwUW_8Lk2o)
- [Namave animation (archive.org)](https://archive.org/details/namave_sound_titles_11)      
- [Train writing by "The Fly"](https://www.youtube.com/watch?v=zSIdvQsu27s)

### ONLINE WORKSHOP
- [Graffolonia material collection & documentation](https://niklasroy.com/graffolonia/) (German)

