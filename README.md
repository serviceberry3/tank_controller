Controller code for SZDoit robot tank chassis over WiFi, using ESP8266-01 module. Code in onboard-ctl/esp-websock/esp-websock.ino should be flashed onto the ESP-01; it will host a webpage at it's IP that clients can use to open up a WebSocket (using arduinoWebSockets) for communication with the module. You can use nmap to find the ESP's IP.  

Code in onboard-ctl/duino-code/duino_code.ino should be flashed onto an Arduino Uno which is hooked up to an L298 or similar for driving the motors.  

You need to change ```ssid[]``` and ```pass[]``` variables in esp-websock.ino to match your Wi-Fi network.

# Flashing the ESP #
Flahing the ESP through the Arduino Uno is tricky. Follow these steps:
* In the Arduino IDE, select "Generic ESP-01 Module" as the board.
* Plug the Uno into the computer and selet appropriate port in the IDE.
* Swap the TX and RX jumper cables on the Uno.
* Connect RESET to GND on the Uno.
* Connect ESP GPIO0 pin to Arduino GND.
* Hit "Upload" in the IDE.
* You should see "Connecting..." or something similar. At this point, you need to pull the RESET pin of the ESP-01 to Ardunio GND for a second.
* Now you should see the code uploading.
* After you see "Hard reset...", pull the ESP's RESET pin to GND again to start running the code.
* Undo all of the connections/switches you made above.

# Known issues #
* It's often difficult to get the ESP to connect to Wifi for some reason. While it tries to connect, you'll here a clicking noise and see the blue LED flash occasionally. In esp-websock.ino, it's only given a certain period of time to connect, otherwise it goes into an infinite idling loop. If you can't get it to connect on the first try, just keep restarting it and it should connect after a few tries, at which point the controller webpage will be served.