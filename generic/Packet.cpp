#include "Packet.h"

namespace generic{
namespace mqtt{

	uint16_t Packet::packetId(){ srand(time(NULL)); return rand() % 65535 + 1; }
	
	//Encode Remaining generic
	uint8_t Packet::encodeRemainingLength(uint8_t* dest, uint32_t length){
		//max allowed 'Remaining generic'
		if (length > MQTT_MAX_REMAINING_LENGTH) return 0;
		uint8_t digit = 0;
		uint8_t len = 0;
		do{
			
			//check if pos index > MQTT_MAX_BYTES_REMAINING_LENGTH
			if (len > MQTT_MAX_BYTES_REMAINING_LENGTH) return false;
			
			digit = length % 128;
			length = length / 128;
			if (length > 0) digit |= 128;
			dest[++len - 1] = digit;
			
		}while(length > 0);
		return len <= MQTT_MAX_BYTES_REMAINING_LENGTH? len : 0;
	}
	
	//decode 'Remaining generic' start from current cursor pointer of IOStream.
	//@return remaining length value in uint32_t
	uint32_t Packet::decodeRemainingLength(Client* _c){
		uint8_t  multiplier = 1;
		int 	 len = 0;
		uint8_t  encoded = 0;
		uint32_t dest = 0;
		uint8_t  src = 0;
		do{
			
			//check if pos index >= MQTT_MAX_BYTES_REMAINING_LENGTH
			if (len >= MQTT_MAX_BYTES_REMAINING_LENGTH) {
				return 0;
			}
			
			//read from IOStream
			if (!_c->readByte(&src)) return 0;
			++len;
			
			encoded = src;
			dest += ( encoded & 127) * multiplier;
			multiplier *= 128;
			
			//check if malformed encoding
			if (multiplier > 128*128*128) return 0;
			
		}while (( encoded & 128) != 0);
		return dest;
	}
	
	ClientState Packet::sendConnect(Client* _c, packet::Connect* _p){
		
		//assume Stream open
		uint8_t varHeader[15] = {0x00,0x04,'M','Q','T','T',MQTT_VERSION};
		int varHeaderLen = 7;
		uint32_t payloadLen = 0;

		//Connect Flag Byte
		uint8_t cfbits = 0x00;
		if (_p->topic) 		  cfbits |= 0x04 | (_p->qos<<3) | (_p->retain<<5);
		if (_p->cleanSession) cfbits |= 0x02;
		if (_p->user) 	  	  cfbits |= 0x80;
		if (_p->pwd) 		  cfbits |= (0x80>>1);
		varHeader[varHeaderLen++] = cfbits;
		
		//Keep Alive Byte
		varHeader[varHeaderLen++] = _p->keepAlive >> 8;
		varHeader[varHeaderLen++] = _p->keepAlive & 0xFF;
		
		//Payload generic
		if (_p->clientId) payloadLen += 2 + strlen(_p->clientId); 
		else 		 	  return ClientState::bad_identifier;
		
		if (_p->topic) payloadLen += 2 + strlen(_p->topic);
		if (_p->user)  payloadLen += 2 + strlen(_p->user);
		if (_p->pwd)   payloadLen += 2 + strlen(_p->pwd);
		
		//connect msg need leading 2 bytes, befor msg bytes
		if (_p->msg || _p->msgLen) payloadLen += 2 + _p->msgLen;
		
		//build fixed header
		uint8_t fixHeaderBytes[5] = {MQTT_CONNECT};
		uint8_t fixHeaderLen = 1 + encodeRemainingLength(fixHeaderBytes+1, varHeaderLen + payloadLen);
		
		//if max Fixed Header 5 byte length, bad encoding
		if (fixHeaderLen > 5) return ClientState::bad_protocol;
		
		//validate write byte length
		uint32_t wrb = fixHeaderLen + varHeaderLen + payloadLen;
		wrb -= _c->writeBytes(fixHeaderBytes, fixHeaderLen);	//Fixed Header Control
		wrb -= _c->writeBytes(varHeader, varHeaderLen);			//variable header
		if (_p->clientId) wrb -= _c->writeEncodedStr(_p->clientId);
		if (_p->topic)	  wrb -= _c->writeEncodedStr(_p->topic);
		
		if (_p->msgLen > 0) {
			if (_p->msg){//if uint8_t* array
				wrb -= _c->writeByte(_p->msgLen >> 8);
				wrb -= _c->writeByte(_p->msgLen & 0xFF);
				wrb -= _c->writeBytes(_p->msg, _p->msgLen);
			}
			else if (_p->msgStream){ //if Msg
				wrb -= _c->writeEncodedStr(_p->msgStream);
			}
		}
		
		if (_p->user) wrb -= _c->writeEncodedStr(_p->user);
		if (_p->pwd)  wrb -= _c->writeEncodedStr(_p->pwd);
		
		//wrb should be 0, if not writeByte failed
		//wait CONNACK packet
		if (wrb) return ClientState::socket_error;
		
		return parseConnAck(_c, _p);
	}

	ClientState Packet::parseConnAck(Client* _c, packet::Connect* _p){
		//1st byte -> Fixed Header Control Packet
		uint8_t cbyte = 0;
		
		if (!_c->readByte(&cbyte)) 		return ClientState::socket_error;
		else if (cbyte != MQTT_CONNACK) return ClientState::bad_protocol;
		
		//2nd byte -> Fixed Header Remaining generic == 2
		uint32_t remaininggeneric = decodeRemainingLength(_c);
		if (remaininggeneric != 2){ //malformed remaining length
			return ClientState::bad_protocol;
		}
		
		uint8_t rbyte[remaininggeneric];
		if (!_c->readBytes(rbyte, remaininggeneric)){
			return ClientState::socket_error;
		}
		
		if (_p)_p->sessionCode = rbyte[0];
		
		return (ClientState) rbyte[1];
		
	}
	
	ClientState Packet::sendDisconnect(Client* _c){
		uint8_t buff[2] = {MQTT_DISCONNECT, 0};
		_c->writeBytes(buff, 2);
		return ClientState::disconnected;
	}
	
	ClientState Packet::sendPingRequest(Client* _c){
		uint8_t buff[2] = {MQTT_PINGREQ, 0};
		if (_c->writeBytes(buff, 2)) return ClientState::connection_lost;
		return ClientState::connected;
	}
	
	ClientState Packet::parsePingRequest(Client* _c){
		uint8_t buff[2];
		if (!_c->readBytes(buff, 2)) 	  return ClientState::connection_lost;
		else if (buff[0] != MQTT_PINGREQ) return ClientState::bad_protocol;
		return ClientState::connected;
	}
	
	ClientState Packet::sendPingResponse(Client* _c){
		uint8_t buff[2] = {MQTT_PINGRESP, 0};
		if (_c->writeBytes(buff, 2)) return ClientState::connection_lost;
		return ClientState::connected;
	}
	
	ClientState Packet::parsePingResponse(Client* _c){
		uint8_t buff[2];
		if (!_c->readBytes(buff, 2)) 	   return ClientState::connection_lost;
		else if (buff[0] != MQTT_PINGRESP) return ClientState::bad_protocol;
		return ClientState::connected;
	}
	
	ClientState Packet::sendSubscribe(Client* _c, uint16_t* pid, const char** reqTopics, uint8_t* reqQos, uint16_t reqLen){
		//Variable Header
		uint32_t varHeaderLen = 2; //only 2 bytes for packet id
		
		//Payload, 
		uint32_t payloadLen = 0;
		for(uint16_t i=0; i<reqLen; i++){
			size_t tlen = strlen(reqTopics[i]);
			payloadLen += (tlen? tlen : 0) + 3;	//add 2 bytes for topic length & 1 bytes for qos
		}
		
		//Fixed Header
		uint8_t fixHeaderBytes[5] = {0};
		fixHeaderBytes[0] = MQTT_SUBSCRIBE | (1<<1);
		
		uint8_t fixHeaderLen = 1 + encodeRemainingLength(fixHeaderBytes+1, varHeaderLen + payloadLen);
		if (fixHeaderLen == 0) return ClientState::bad_protocol;
		
		//validate write byte length
		uint32_t wrb = fixHeaderLen + varHeaderLen + payloadLen;
		wrb -= _c->writeBytes(fixHeaderBytes, fixHeaderLen); //fixed header
		
		//generate msg id if not provided, and set to pointer src
		//subscribe should have pid
		if (*pid <= 0) *pid = packetId();
		wrb -= _c->writeByte((*pid >> 8));	//msb
		wrb -= _c->writeByte((*pid & 0xFF));	//lsb
			
		//topic & qos
		for(uint16_t i=0; i<reqLen; i++){
			wrb -= _c->writeEncodedStr(reqTopics[i]);
			wrb -= _c->writeByte(reqQos[i]);
		}
		
		//wrb should be 0, if not writeByte failed
		if (wrb) return ClientState::connection_lost;
		
		return ClientState::connected;
	}
	
	ClientState Packet::parseSubAck(Client* _c, uint16_t* pid, std::vector<uint8_t>* qos){
		//read Fixed Header Control Flag
		uint8_t cByte = 0;
		if (!_c->readByte(&cByte)) 	   return ClientState::connection_lost;
		else if (cByte != MQTT_SUBACK) return ClientState::bad_protocol;
		
		
		//read Fixed Header remaining length
		uint32_t remaininggeneric = decodeRemainingLength(_c);
		if (remaininggeneric == 0) return ClientState::bad_protocol;
		
		//extract Variable Header -> Packet Id
		//we need 2 bytes for Packet Id & match pid given
		uint8_t pidbytes[2] = {0};
		if (!_c->readBytes(pidbytes, 2)) return ClientState::connection_lost;
		*pid = pidbytes[0] << 8 | pidbytes[1];
		remaininggeneric -= 2;
		
		//extract Payload
		for(uint32_t i=0; i<remaininggeneric; i++) {
			uint8_t bb = 0;
			if (!_c->readByte(&bb)) return ClientState::connection_lost;
			qos->push_back(bb);
		}
		
		return ClientState::connected;
	}
	
	ClientState Packet::sendUnsubscribe(Client* _c, uint16_t* pid, const char** reqTopics, uint16_t reqLen){
		//Variable Header
		uint32_t varHeaderLen = 2; //only 2 bytes for packet id
		
		//Payload, 
		uint32_t payloadLen = 0;
		for(uint16_t i=0; i<reqLen; i++){
			size_t tlen = strlen(reqTopics[i]);
			payloadLen += (tlen? tlen : 0) + 2;	//add 2 bytes for topic length
		}
		
		//Fixed Header
		uint8_t fixHeaderBytes[5] = {MQTT_UNSUBSCRIBE | (1<<1), 0, 0, 0, 0};
		uint8_t fixHeaderLen = 1 + encodeRemainingLength(fixHeaderBytes+1, varHeaderLen + payloadLen);
		if (fixHeaderLen == 0) return ClientState::bad_protocol;
		
		//validate write byte length
		uint32_t wrb = fixHeaderLen + varHeaderLen + payloadLen;
		wrb -= _c->writeBytes(fixHeaderBytes, fixHeaderLen); //fixed header
		
		//generate msg id if not provided, and set to pointer src
		if (*pid <= 0) *pid = packetId();
		wrb -= _c->writeByte((*pid >> 8));	//msb
		wrb -= _c->writeByte((*pid & 0xFF));//lsb
		
		//Paylod Topics
		for(uint16_t i=0; i<reqLen; i++) wrb -= _c->writeEncodedStr(reqTopics[i]);
		
		//wrb should be 0, if not writeByte failed
		if (wrb) return ClientState::connection_lost;
		return ClientState::connected;
	}
	
	ClientState Packet::parseUnsubAck(Client* _c, uint16_t* pid){
		//UNSUBACK only have 4 bytes packet
		//read Fixed Header Control Flag
		uint8_t cByte = 0;
		if (!_c->readByte(&cByte)) 		 return ClientState::connection_lost;
		else if (cByte != MQTT_UNSUBACK) return ClientState::bad_protocol;
		
		//read Fixed Header remaining length
		uint32_t remaininggeneric = decodeRemainingLength(_c);
		if (remaininggeneric == 0) return ClientState::bad_protocol;
		
		//we need 2 bytes for Packet Id & match pid given
		uint8_t pidbytes[2] = {0};
		if (!_c->readBytes(pidbytes, 2)) return ClientState::connection_lost;
		*pid = pidbytes[0] << 8 | pidbytes[1];
		
		
		return remaininggeneric-2 == 0? ClientState::connected : ClientState::bad_protocol;
	}
	
	ClientState Packet::sendPublish(Client* _c, 
										uint16_t* pid, 
										const char* topic, 
										uint8_t qos, 
										bool retain, 
										bool duplicate, 
										uint8_t* msgBytes, 
										size_t msglen, 
										Msg* msgStream)
	{
		//calculate Variable Header
		uint32_t varHeaderLen = 0; //2 bytes for msg/packet id
		
		//no topic provided, don't publish
		if (!topic) return ClientState::bad_protocol;
		
		//topic bytes
		varHeaderLen += strlen(topic) + 2;
		
		//variable header = packet identifier
		//only available when qos > 0
		varHeaderLen += (qos > 0)? 2 : 0;
		
		//calculate Payload, 
		//PUBLISH don't need to add 2 bytes for length
		uint32_t payloadLen = 0;
		if (msgBytes) 		payloadLen = msglen;
		else if (msgStream) payloadLen = msgStream->size();
		else 				return ClientState::bad_protocol;
		
		//build Fixed Header
		int fixHeaderPos = 0;
		uint8_t fixHeaderBytes[5] = {0};
		fixHeaderBytes[0] = MQTT_PUBLISH | 
							(retain? 1 : 0) |
							(qos > 0? (1<<1) : 0) |
							(duplicate? (1 << 3) : 0);	
		
		uint8_t fixHeaderLen = encodeRemainingLength(fixHeaderBytes+1, varHeaderLen + payloadLen);
		if ((fixHeaderLen++) == 0) return ClientState::bad_protocol;
		
		//validate write byte length
		uint32_t wrb = fixHeaderLen + varHeaderLen + payloadLen;
		wrb -= _c->writeBytes(fixHeaderBytes, fixHeaderLen); //fixed header
		wrb -= _c->writeEncodedStr(topic); //variable header = topic
		
		//variable header = packet identifier
		//only available when qos > 0
		if (qos > 0){
			if (*pid <= 0) *pid = packetId();	//generate msg id if not provided, and set to pointer src
			wrb -= _c->writeByte((*pid >> 8));	//msb
			wrb -= _c->writeByte((*pid & 0xFF));//lsb
		}
		
		if (msgBytes && msglen > 0) {
			wrb -= _c->writeBytes(msgBytes, msglen); 
		}
		else if (msgStream){
			size_t opos = msgStream->position();		 //move Msg cursor pointer to beginning,
			msgStream->seek(0);
			while (msgStream->available()) {	
				wrb -= _c->writeByte(msgStream->read()); //write to iostream
			}
			msgStream->seek(opos);						 //put back cursor pointer later
		}
		else{
			return ClientState::bad_protocol;
		}
		
		//wrb should be 0, if not writeByte failed
		if (wrb) return ClientState::connection_lost;
		return ClientState::connected;
	}
	
	ClientState Packet::parsePublish(Client* _c, uint16_t* pid, std::string* topic, Msg* msg){
		uint8_t cByte = 0;
		if (!_c->readByte(&cByte)) 		 		  return ClientState::connection_lost;
		else if ((cByte>>4) != (MQTT_PUBLISH>>4)) return ClientState::bad_protocol;
		
		bool 	duplicate = (cByte & 0b1000) >> 3;
		uint8_t qos    	  = (cByte & 0b110 ) >> 1;
		bool 	retain    = (cByte & 0b1   );
		
		uint32_t packetgeneric = decodeRemainingLength(_c);
		
		//topic length
		uint8_t tl[2] = {0,0};
		if (!_c->readBytes(tl, 2)) return ClientState::connection_lost;
		uint16_t topicLen = (tl[0] << 8 | tl[1]);
		packetgeneric -= ((uint32_t)topicLen);
		packetgeneric -= 2;
		
		//read topic 
		uint8_t topicBytes[topicLen + 1];
		memset(topicBytes, 0, topicLen + 1);
		if (!_c->readBytes(topicBytes, topicLen)) return ClientState::connection_lost;
		*topic = std::string((char*) topicBytes);
		
		//pid
		if (qos > 0){
			_c->readBytes(tl, 2);
			*pid = (tl[0] << 8 | tl[1]);
			packetgeneric -= 2;
		}
		
		//payload
		msg->seek(0);
		Serial.print(F("packetgeneric: "));Serial.println(packetgeneric);
		while (packetgeneric > 0){
			size_t r = (packetgeneric > 1000)? packetgeneric - 1000 : packetgeneric;
			if (!_c->readBytes(msg, r)) return ClientState::connection_lost;	
			msg->flush();
			packetgeneric -= r;
		}
		return ClientState::connected;
	}
};
};