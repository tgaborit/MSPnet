# EE579-IFTT

This project consists in the implementation of a **"if this, then that"** functionality. Devices based on a MSP430 microcontroller are able to send inputs and receive outputs to any similar device connected to the system based on a web-based server. The logic defining the inputs and outputs is configurable by the user. For example, to blink a LED on a device for 10 seconds when a switch is toggled on another device.

## Tree view

* `/documents` : the documents used for the project management
* `/msp` : the IAR project containing the header and source files relating to the client part of the system, i. e. the device based on a MSP430
* `/server` : the source files related to the server part of the system
* `config.h` : configuration file related to the client comprising parameters such as the WiFi Configuration, the MQTT Broker address or the board setup.

## Built With

* [IAR EW for MSP430](https://www.iar.com/products/architectures/iar-embedded-workbench-for-msp430/) - The IDE used for developing for the MSP430
* [Node-RED](https://nodered.org/) - Programming tool used for the server
* [Eclipse Mosquitto](https://mosquitto.org/) - MQTT broker used for the server
* [Amazon Web Services](https://aws.amazon.com/) - Cloud service used for having a Virtual Private Server

## Authors

* **Emmanuel Durel** 
* **Rémi Fargeon**
* **Titouan Gaborit**
* **Baptiste Méhat**
