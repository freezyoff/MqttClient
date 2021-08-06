#ifndef _mqtt_packet_h_
#define _mqtt_packet_h_

#include "Stream.h"
#include "Msg.h"

#define MQTT_VERSION 4
#define MQTT_MAX_BYTES_REMAINING_LENGTH 4
#define MQTT_MAX_HEADER_LENGTH 5;
#define MQTT_MAX_TOPIC_LENGTH 2000
#define MQTT_MAX_REMAINING_LENGTH 2000

#ifndef MQTT_KEEP_ALIVE
#define MQTT_KEEP_ALIVE 15
#endif

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
			
			//server response session, 
			//if (clean session) = 0
			//if !(clean session) = server value
			uint8_t sessionCode = 0;
			
		};
		
	};
	
	struct Packet{
		
		static const uint8_t MQTT_CONNECT= 		1 << 4; // Client request to connect to Server
		static const uint8_t MQTT_CONNACK=	 	2 << 4; // Client request to connect to Server
		static const uint8_t MQTT_PUBLISH=     	3 << 4; // Publish message
		static const uint8_t MQTT_PUBACK=      	4 << 4; // Publish Acknowledgment
		static const uint8_t MQTT_PUBREC=      	5 << 4; // Publish Received (assured delivery part 1)
		static const uint8_t MQTT_PUBREL=      	6 << 4; // Publish Release (assured delivery part 2)
		static const uint8_t MQTT_PUBCOMP=     	7 << 4; // Publish Complete (assured delivery part 3)
		static const uint8_t MQTT_SUBSCRIBE=   	8 << 4; // Client Subscribe request
		static const uint8_t MQTT_SUBACK=      	9 << 4; // Subscribe Acknowledgment
		static const uint8_t MQTT_UNSUBSCRIBE= 10 << 4; // Client Unsubscribe request
		static const uint8_t MQTT_UNSUBACK=    11 << 4; // Unsubscribe Acknowledgment
		static const uint8_t MQTT_PINGREQ=     12 << 4; // PING Request
		static const uint8_t MQTT_PINGRESP=    13 << 4; // PING Response
		static const uint8_t MQTT_DISCONNECT=  14 << 4; // MqttClient is Disconnecting

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