# verisure-armstate-mqtt

ESP8266 node that sends a single MQTT message when powered up.  

While this can be useful in various scenarios, the driving idea was that I wanted to be notified via a MQTT message immediately when my Verisure alarm system was disarmed/turned off.  
It would be perfectly possible to poll Verisure's API to get the current arm state of the alarm, but as there are rate limits in place such polling would mean delays, and the notification would not be immediate.  

By using a combination of Verisure hardware and their "smart home" concept, which (among other things) allows for power plugs to be turned on or off when the alarm is disarmed, notifications with only a 2-3 second delay are possible.

![alt text](https://github.com/mountaindude/verisure-armstate-mqtt/raw/master/img/20180922_132155.jpg "The final result, ready to be inserted into 220 V wall outlet.")
![alt text](https://github.com/mountaindude/verisure-armstate-mqtt/raw/master/img/20180922_132259.jpg "Verisure smart plug and USB power adapter to the left, ESP8266 box to the right.")
![alt text](https://github.com/mountaindude/verisure-armstate-mqtt/raw/master/img/20180922_132314.jpg "A standard, unmodified Verisure smart plug is used.")
![alt text](https://github.com/mountaindude/verisure-armstate-mqtt/raw/master/img/20180922_132438.jpg "All components used.")
![alt text](https://github.com/mountaindude/verisure-armstate-mqtt/raw/master/img/20180922_132519.jpg "Close-up of soldering USB cables onto CH340 chip and D1 pins.")



## Concept

1. A person comes home and disarms (turns off) the Verisure alarm system.
2. A Verisure "smart plug" (=radio controlled power outlet) has been configured to turn on when the alarm system is disarmed. This configuration is done using Verisure's standard online tools.
3. A small ESP8266 device is plugged into the smart plug, and boots when power becomes available.
4. In order to boot as quickly as possible, the ESP8266 device uses a static IP address rather than DHCP.
5. The device sends a single MQTT message "ALARM_OFF" when powered up. The message is typically sent within 2-3 seconds, which means that for example home automation devices (lights etc) can take actions immediately when the alarm is disarmed. You can easily change (in main.cpp) the MQTT topic used. 
6. When the alarm system is later armed/turned on, the smart plug will turn off. When its power is cut, the ESP8266 device will of course loose contact with the MQTT broker, but as the device registered a LWT (Last Will and Testament) when connecting to the broker, the LWT message will be sent by the broker to the MQTT topic of choice. Note that there is a (up to) 15 second delay before this happens, but this doesn't really matter when it comes to being notified of the alarm being turned on/armed.
  
I dug into my random-parts-box and found a small plastic box in which I hotglued an Wemos D1 Mini Lite, with a cut-off USB cable soldered directly onto the pins of the CH340 USB-to-serial chip. 
Some additional hotglue acts as stress relief for the somewhat small/delicate USB cables.  

Having a short USB cable (10-15 cm) makes it possible to get a pretty compact unit without loose cables all around it. The combo (Verisure smart plug, USB adapter, ESP8266 box) is then plugged in as a whole into a wall outlet. As long as the unit is within range from the alarm system's central unit, it doesn't matter what power outlet is used. I used some random 220V to USB adapter I had lying around, but any old USB cell phone charger should work fine.







## Needed configuration

1. The Verisure smart plug should be configured to turn on when the alarm system is disarmed, and turn off when the alarm is armed.
2. Change main.cpp as needed for your environment
    1. ssid and password for wifi
    2. Static IP address that the device should use
    3. IP address and port of MQTT broker
    4. What MQTT topic messages should be posted to
3. The device sends a heartbeat message ("Ping...") over the serial port every 5 seconds


## Development setup

Platform IO ("PIO") was used to develop the ESP8266 code.  

The code uses the PubSubClient library - make sure to install it before compiling.

Some PIO related files might be missing in the git repository, if that is the case just create a new PIO project on your local computer, then copy the src files and platformio.ini from the repository to your new project. 
Next time you compile PIO will rebuild all dependencies.  


## Hardware

Both Wemos [D1 mini](https://wiki.wemos.cc/products:d1:d1_mini) and [D1 mini lite](https://wiki.wemos.cc/products:d1:d1_mini_lite) have been successfully used with the code in this repository.  
But as the code is very minimalistic, pretty much any ESP8266 board should work.




