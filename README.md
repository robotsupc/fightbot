# Fightbot

Fightbot is a 3D printed Mario Kart clone for less than 10$

Built by [Robots UPC](http://www.robotsupc.com), the UPC's Robotics student association.


# How it works
The body is completely 3D printed. The parts include holders and adapters for all electronics.
To use the arduino wirelessly, we need to use batteries. 4AA batteries provide 6V when fully charged, so we need to 
step this down to the 5V the arduino needs. We use the LM2596 DC step down module for this.

To **control it wirelessly**, we need to use a WiFi-capable Arduino, or use a RC receiver module.
The easiest way to do this is using a ESP8266 based controller, which **includes WiFi**, so we use the D1 mini.

The D1 mini uses the HG7881 motor driver, because it cannot supply the required current itself.

### The interesting part
To communicate to the arduino, it sets up an AP, called "RobotsUPC Fightbot X", where X is {1,2,3,4}.
When a client connects, we use a captive portal like setup, which will capture any URL and redirect it to the arduino itself.
For example you can visit example.com and you will be shown the robot's page.
This page, instead of the usual login page, shows a joystick that allows you to control the robot.
Since ajax and the likes are way too slow, we use WebSockets to communicate to the Arduino, obtaining excellent latency results.



# Requirements
Bill of materials:
* 1x [D1 mini Arduino](https://www.aliexpress.com/item/D1-mini-Mini-NodeMcu-4M-bytes-Lua-WIFI-Internet-of-Things-development-board-based-ESP8266-by/32635160765.html?)
* 1x [LM2596 buck down](https://www.aliexpress.com/item/15924-Free-shipping-DC-DC-Step-Down-Converter-Module-LM2596-DC-4-0-40-to-1/32354635261.html)
* 1x [HG7881 motor driver](https://www.aliexpress.com/item/5pcs-lot-HG7881-HG7881CP-two-road-motor-driven-motor-driver-module-for-arduino-2-channel-motor/1919127484.html)
* 4x AA batteries
* 1x 4AA battery holder
* Some jumper wires
* About 20 M3 screws & nuts

You'll allso need:
* Screwdriver
* Soldering iron
* A knife
* A 3D printer

# Building
Coming soon.

# Results
We have presented the robot at the university's expo, obtaining amazing interest from other students.
