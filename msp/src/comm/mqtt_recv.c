#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../comm/mqtt_recv.h"

void mqtt_recv_publish(uint8_t* data, char* topic, char* payload) {
  
  /* type, flags: 0
   * remaining length: 1
   * topic length: 2
   * topic: 4
   * property length:  topic index + topic_length
   * (assuming no property)
   * payload: topic index + topic_length + 1
  */

  uint8_t remaining_length = data[1];
  
  /* fetch length of topic field (data[2..3]) */
  uint16_t topic_length = (data[2] << 8) | data[3];
  
  /* copy topic */
//  memcpy(topic, &(data[4]), topic_length);
  
//  topic = &(data[4]);
  topic = data + 4;
  topic[topic_length] = '\0';
  
  /* variable_header_lenght = topic_length_length (2) + topic_length + 
   * property_length_length (1, assuming no property)*/
  uint8_t variable_header_length = 2 + topic_length + 1;
  uint16_t payload_length = remaining_length - variable_header_length ;
  
  /* copy payload */
//  memcpy(payload, &(data[4+topic_length+1]), payload_length);
  
//  payload = &(data[4+topic_length+1]);
  payload = data + 4+topic_length+1;
  payload[payload_length] = '\0';
}



//int mqt_recv_connack(uint8_t* data) {
//  
//  /* type, flags: 0
//   * remaining length: 1
//   * connack flag: 2
//   * return code: 3
//  */
//  
//  /* if return code is 0*/
//  if(data[3] == 0x00) {
//    /* connection was accepted */
//    return 1;
//  }
//  /* connection was not accepted */
//  return 0;
//}




//mqtt_type_t mqtt_get_type(uint8_t* data) {
//  return (mqtt_type_t)data[0] >> 4;
//}






