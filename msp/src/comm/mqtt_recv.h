#ifndef MQTT_RECV_H
#define MQTT_RECV_H

#define TOPIC_MAX_LENGTH 8
#define PAYLOAD_MAX_LENGTH 90

///* message types 
// * (from https://github.com/deoxxa/mqtt-protocol-c/src/message.h) */
//typedef enum mqtt_type_t {
//  MQTT_TYPE_CONNECT = 1,
//  MQTT_TYPE_CONNACK = 2,
//  MQTT_TYPE_PUBLISH = 3,
//  MQTT_TYPE_PUBACK = 4,
//  MQTT_TYPE_PUBREC = 5,
//  MQTT_TYPE_PUBREL = 6,
//  MQTT_TYPE_PUBCOMP = 7,
//  MQTT_TYPE_SUBSCRIBE = 8,
//  MQTT_TYPE_SUBACK = 9,
//  MQTT_TYPE_UNSUBSCRIBE = 10,
//  MQTT_TYPE_UNSUBACK = 11,
//  MQTT_TYPE_PINGREQ = 12,
//  MQTT_TYPE_PINGRESP = 13,
//  MQTT_TYPE_DISCONNECT = 14,
//} mqtt_type_t;

//* processes conneck messages
// * reutrns 1 is connection succeeded, 0 otherwise*/
//int mqt_recv_connack(uint8_t* data);

/* parses publish messages
 * topic and payload are filled with content of publish message
 *
 * size of topic should be TOPIC_MAX_LENGTH
 * size of payload should be PAYLOAD_MAX_LENGTH */
void mqtt_recv_publish(uint8_t* data, char* topic, char* payload);

///* returns type of mqtt command */
//mqtt_type_t mqtt_get_type(uint8_t* data);

#endif //MQTT_RECV_H
