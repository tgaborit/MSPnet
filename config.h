#ifndef CONFIG_H
#define CONFIG_H

///////////////////////////////
////////DEVICE ID//////////////
///////////////////////////////

#define DEVICE_ID '1'


///////////////////////////////
///////WIFI CONFIGURATION//////
///////////////////////////////

#define WIFINETWORK  "HUAWEI P10 lite"
#define WIFIPASSWORD "12345678"


///////////////////////////////
////////BROKER MQTT////////////
///////////////////////////////

#define MQTTHOST     "ec2-3-8-210-204.eu-west-2.compute.amazonaws.com"
#define MQTTPORT     "1883"


///////////////////////////////
////////HARDWARE SETUP/////////
///////////////////////////////

// uncomment if a switch is used on P1.3
#define SWITCH_0

// uncomment if an external switch is used, and fill the port and pin
//#define EXT_SWITCH_1
#ifdef EXT_SWITCH_1
  #define EXT_PORT_1 1          // port (set to 1 or 2)
  #define EXT_PIN_1 0x10        // pin (mask of the pin position between pin 0 and 7)
#endif

// uncomment if another external switch is used, and fill the port and pin
//#define EXT_SWITCH_2
#ifdef EXT_SWITCH_2
  #define EXT_PORT_2 1          // port (set to 1 or 2)
  #define EXT_PIN_2 0x20        // pin (mask of the pin position between pin 0 and 7)
#endif
  
// uncomment if a potentiometer is used, and fill the pin (port is automatically 1)
//#define POTENTIO
#ifdef POTENTIO
  #define POTENTIO_PIN 1        // pin (pin position between 0 and 7)
#endif
  
//uncomment if a buzzer is used, and fill the port and pin
//#define BUZZ
#ifdef BUZZ
  #define BUZZ_PORT 1           // port (set to 1 or 2)
  #define BUZZ_PIN 0x40         // pin (mask of the pin position between pin 0 and 7)
#endif

#endif
