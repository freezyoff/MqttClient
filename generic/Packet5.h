#ifndef _mqtt_packet_h_
#define _mqtt_packet_h_

#include "Stream.h"
#include "Msg.h"

#define MQTT_VERSION 5
#define MQTT_MAX_BYTES_REMAINING_LENGTH 4
#define MQTT_MAX_HEADER_LENGTH 5;
#define MQTT_MAX_TOPIC_LENGTH 65536
#define MQTT_MAX_REMAINING_LENGTH 2000

#ifndef MQTT_KEEP_ALIVE
#define MQTT_KEEP_ALIVE 15
#endif


#define MQTT_CTRL_CONNECT 		1 << 4 // Client request to connect to Server
#define MQTT_CTRL_CONNACK	 	2 << 4 // Client request to connect to Server
#define MQTT_CTRL_PUBLISH     	3 << 4 // Publish message
#define MQTT_CTRL_PUBACK      	4 << 4 // Publish Acknowledgment
#define MQTT_CTRL_PUBREC      	5 << 4 // Publish Received (assured delivery part 1)
#define MQTT_CTRL_PUBREL      	6 << 4 // Publish Release (assured delivery part 2)
#define MQTT_CTRL_PUBCOMP     	7 << 4 // Publish Complete (assured delivery part 3)
#define MQTT_CTRL_SUBSCRIBE   	8 << 4 // Client Subscribe request
#define MQTT_CTRL_SUBACK      	9 << 4 // Subscribe Acknowledgment
#define MQTT_CTRL_UNSUBSCRIBE  10 << 4 // Client Unsubscribe request
#define MQTT_CTRL_UNSUBACK     11 << 4 // Unsubscribe Acknowledgment
#define MQTT_CTRL_PINGREQ      12 << 4 // PING Request
#define MQTT_CTRL_PINGRESP     13 << 4 // PING Response
#define MQTT_CTRL_DISCONNECT   14 << 4 // MqttClient is Disconnecting
#define MQTT_CTRL_AUTH  	   15 << 4 // Authentication exchange

#define MQTT_CTRL_FLAG 		1 << 1
#define MQTT_FLAG_DUPLICATE 1 << 3
#define MQTT_FLAG_QOS 		1 << 1
#define MQTT_FLAG_RETAIN 	1 << 0

//The Reason Codes used for Malformed Packet and Protocol Errors are:
#define MQTT_ERROR_UNSPECIFIED_ERROR 0x80
#define MQTT_ERROR_MALFORMED_PACKET 					  0x81
#define MQTT_ERROR_PROTOCOL_ERROR   					  0x82
#define MQTT_ERROR_IMPLEMENTATION_SPECIFIC_ERROR 		  0x83
#define MQTT_ERROR_UNSUPPORTED_PROTOCOL_VERSION 		  0x84 
#define MQTT_ERROR_CLIENT_IDENTIFIER_NOT_VALID			  0x85
#define MQTT_ERROR_BAD_USER_NAME_OR_PASSWORD 			  0x86  
#define MQTT_ERROR_NOT_AUTHORIZED 						  0x87 
#define MQTT_ERROR_SERVER_UNAVAILABLE 					  0x88
#define MQTT_ERROR_SERVER_BUSY 							  0x89
#define MQTT_ERROR_BANNED 								  0x8A 
#define MQTT_ERROR_SERVER_SHUTTING_DOWN 				  0x8B 
#define MQTT_ERROR_BAD_AUTHENTICATION_METHOD 			  0x8C 
#define MQTT_ERROR_KEEP_ALIVE_TIMEOUT 					  0x8D  
#define MQTT_ERROR_SESSION_TAKEN_OVER 					  0x8E
#define MQTT_ERROR_TOPIC_FILTER_INVALID 				  0x8F
#define MQTT_ERROR_TOPIC_NAME_INVALID 					  0x90
#define MQTT_ERROR_PACKET_IDENTIFIER_IN_USE 			  0x91
#define MQTT_ERROR_PACKET_IDENTIFIER_NOT_FOUND 			  0x92
#define MQTT_ERROR_RECEIVE_MAXIMUM_EXCEEDED 			  0x93
#define MQTT_ERROR_TOPIC_ALIAS_INVALID 					  0x94
#define MQTT_ERROR_PACKET_TOO_LARGE 					  0x95
#define MQTT_ERROR_MESSAGE_RATE_TOO_HIGH 				  0x96
#define MQTT_ERROR_QUOTA_EXCEEDED 						  0x97
#define MQTT_ERROR_ADMINISTRATIVE_ACTION 				  0x98
#define MQTT_ERROR_PAYLOAD_FORMAT_INVALID 				  0x99
#define MQTT_ERROR_RETAIN_NOT_SUPPORTED 				  0x9A
#define MQTT_ERROR_QOS_NOT_SUPPORTED 					  0x9B
#define MQTT_ERROR_USE_ANOTHER_SERVER 					  0x9C
#define MQTT_ERROR_SERVER_MOVED  						  0x9D
#define MQTT_ERROR_SHARED_SUBSCRIPTIONS_NOT_SUPPORTED 	  0x9E
#define MQTT_ERROR_CONNECTION_RATE_EXCEEDED 			  0x9F
#define MQTT_ERROR_MAXIMUM_CONNECT_TIME					  0xA0
#define MQTT_ERROR_SUBSCRIPTION_IDENTIFIERS_NOT_SUPPORTED 0xA1
#define MQTT_ERROR_WILDCARD_SUBSCRIPTIONS_NOT_SUPPORTED   0xA2


#define MQTT_PROP_SESSION_EXPIRY_INTERVAL 		0x11 //4 Bytes, once
#define MQTT_PROP_REQUEST_PROBLEM_INFORMATION	0x17 //1 Byte, 0/1, once
#define MQTT_PROP_REQUEST_RESPONSE_INFORMATION	0x19 //1 Byte, 0/1, once
#define MQTT_PROP_RECEIVE_MAXIMUM 		  		0x21 //2 Bytes, once
#define MQTT_PROP_TOPIC_ALIAS_MAXIMUM 	  		0x22 //2 Bytes, once
#define MQTT_PROP_USER_PROPERTY					0x26 //UTF-8 String Pair
#define MQTT_PROP_MAXIMUM_PACKET_SIZE 	  		0x27 //4 Bytes, once
namespace freezyoff{
namespace mqtt{
	
	struct Client;
	
	namespace packet{
	
		struct Connect {
			
			const char* clientId = NULL;			//client id
			const char* user = NULL;
			const char* pwd = NULL;
			const char* topic = NULL; 				//will topic
			uint8_t* 	msg = NULL; 				//will msg str
			size_t 		msgLen = 0; 				//will msg length
			Msg* 		msgStream = NULL;			//will msg stream
			uint8_t 	qos = 0; 					//will qos
			bool 		retain = 0;					//will retain
			bool 		cleanSession = 1;			//should use clean session
			uint16_t 	keepAlive = MQTT_KEEP_ALIVE;
			
			uint32_t 	prop_session_expiry;
			
			//server response session, 
			//if (clean session) = 0
			//if !(clean session) = server value
			uint8_t sessionCode = 0;
			
		};
		
	};
	
	struct Packet{
		

		//send connect packet (blocking main thread)
		//@return true if send successfully; false otherwise
		static ClientState sendConnect(Client*, packet::Connect*);
		static ClientState parseConnAck(Client*, packet::Connect*);
		
		static ClientState sendPingRequest(Client* _c);
		static ClientState parsePingRequest(Client* _c);
		
		static ClientState sendPingResponse(Client* _c);
		static ClientState parsePingResponse(Client* _c);
		
		static ClientState sendDisconnect(Client* _c);
		
		static ClientState sendSubscribe(Client*, uint16_t* pid, const char** reqTopics, uint8_t* reqQos, uint16_t reqLen);
		static ClientState parseSubAck(Client*, uint16_t* pid, std::vector<uint8_t>* qos);
		
		static ClientState sendUnsubscribe(Client* _c, uint16_t* pid, const char** topics, uint16_t len);
		static ClientState parseUnsubAck(Client* _c, uint16_t* pid);
		
		static ClientState sendPublish(Client* _c, uint16_t* pid, const char* topic, uint8_t qos, bool retain, bool duplicate, uint8_t* msg, size_t msgLen, Msg* msgStream);
		static ClientState parsePublish(Client* _c, uint16_t* pid, std::string* topic, Msg* msg);
		
		protected:
		static uint8_t  encodeRemainingLength(uint8_t* dest, uint32_t length);
		static uint32_t decodeRemainingLength(Client* _c);
		static uint16_t packetId();
		
	};
	
};
};

#endif