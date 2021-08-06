#include "Client.h"

#include "Msg.cpp"
#include "Packet.cpp"

namespace freezyoff{
namespace mqtt{
	
	void  	 Client::setSetSocketTimeout(uint16_t t){ _ioStream->setSetSocketTimeout(t); }
	uint16_t Client::socketTimeout(){ return _ioStream->socketTimeout(); }
		
	void 		Client::setServer(const char* host, uint32_t port){ _host = host; _port = port; }
	const char* Client::host(){ return _host; }
	uint16_t 	Client::port(){ return _port; }
	
	void 		Client::setState(ClientState s){ _state = s; }
	ClientState Client::state(){ return _state; }
	
	void 			 Client::setConnectOpt(packet::Connect& p){ _connOpt = p; }
	packet::Connect* Client::connectOpt(){ return &_connOpt; }
		
	bool Client::connect(const char* clientId, bool cleanSession){
		packet::Connect _opt; 
		_opt.clientId = clientId;
		_opt.cleanSession = cleanSession;
		return connect(_opt);
	}
	
	bool Client::connect(const char* clientId, const char* user, const char* pwd, bool cleanSession, uint16_t keepAlive){
		packet::Connect _opt; 
		_opt.clientId = clientId;
		_opt.user = user;
		_opt.pwd = pwd;
		_opt.cleanSession = cleanSession;
		_opt.keepAlive = keepAlive;
		return connect(_opt);
	}
	
	bool Client::connect(packet::Connect& p){ 
		setConnectOpt(p);
		
		if (!_ioStream->connected()){
			_ioStream->setSetSocketTimeout(2);
			if (!host() || port() == 0) {
				return false;
			}
			if (!_ioStream->connect(host(), port())) {
				return false;
			}
		}
		
		_state = Packet::sendConnect(this, &p);
	
		const bool result = (_state == ClientState::connected);
		if (connected()){
			for(ClientListener* l: _lst) {
				l->onConnected(result);
			}
		}
		return result;
	}
	
	void Client::disconnect(){
		_ioStream->disconnect();
	}
	
	
	bool Client::publish(const char* topic, uint8_t* msg, size_t msglen, uint8_t qos, bool retain, bool duplicate){
		uint16_t pid;
		return publish(&pid,  topic, msg, msglen, qos, retain, duplicate);
	}
	
	bool Client::publish(const char* topic, const char* msg, uint8_t qos, bool retain, bool duplicate){
		uint16_t pid;
		return publish(&pid,  topic, (uint8_t*) msg, strlen(msg), qos, retain, duplicate);
	}
	
	bool Client::publish(const char* topic, Msg* msg, uint8_t qos, bool retain, bool duplicate){
		uint16_t pid;
		return publish(&pid,  topic, msg, qos, retain, duplicate);
	}
	
	bool Client::publish(uint16_t* pid, const char* topic, Msg* msg, uint8_t qos, bool retain, bool duplicate){
		return Packet::sendPublish(this, pid, topic, qos, retain, duplicate, NULL, msg->size(), msg);
	}
	
	bool Client::publish(uint16_t* pid, const char* topic, uint8_t* msg, size_t msglen, uint8_t qos, bool retain, bool duplicate){
		_state =  Packet::sendPublish(this, pid, topic, qos, retain, duplicate, msg, msglen, NULL);
		if (_state != ClientState::connected){
			disconnect();
			return false;
		}
		
		//if qos == 0, notify listener
		if (qos == 0) {
			for(ClientListener* l: _lst) {
				l->onPublish(*pid);
			}
		}
		
		return true;
	}
	
	bool Client::subscribe(const char* topic, uint8_t qos){
		uint16_t pid; 
		return subscribe(&pid, topic, qos);
	}
	
	bool Client::subscribe(uint16_t* pid, const char* topic, uint8_t qos){ 
		const char* topics[1] = {topic};
		uint8_t qoss[1] = {qos};
		return subscribe(pid, topics, qoss, 1);
	}
	
	bool Client::subscribe(uint16_t* pid, const char** topics, uint8_t qos, uint16_t len){
		uint8_t qoss[len];
		for(uint16_t i=0; i<len; i++) qoss[i] = qos;
		return subscribe(pid, topics, qoss, len);
	}
	
	bool Client::subscribe(uint16_t* pid, const char** topics, uint8_t* qos, uint16_t len){
		return Packet::sendSubscribe(this, pid, topics, qos, len) == ClientState::connected;
	}
	
	bool Client::unsubscribe(const char* topic){
		uint16_t pid;
		const char* topics[1] = {topic};
		return unsubscribe(&pid, topics, 1);
	}
	
	bool Client::unsubscribe(uint16_t* pid, const char** topics, uint16_t len){
		return Packet::sendUnsubscribe(this, pid, topics, len);
	}
		
	bool Client::connected(){ 
		return state() == ClientState::connected; 
	}
	
	bool Client::readByte(uint8_t* dest){ 
		if (_ioStream->readByte(dest)){
			time(&_lastPing); 
			return true;
		}
		return false;
	}
	
	bool Client::readBytes(uint8_t* dest, size_t l){ 
		if (_ioStream->readBytes(dest, l)){
			time(&_lastPing); 
			return true;
		}
		return false;
	}
	
	bool Client::readBytes(Msg* dest, size_t l){ 
		if (_ioStream->readBytes(dest, l)){
			time(&_lastPing); 
			return true;
		}
		return false;
	}
	
	size_t Client::writeByte(uint8_t b){ time(&_lastPing); return _ioStream->writeByte(b); }
	size_t Client::writeBytes(uint8_t* b, size_t l){ time(&_lastPing); return _ioStream->writeBytes(b, l); }
	size_t Client::writeBytes(const char* c){ time(&_lastPing); return _ioStream->writeBytes(c); }
	
	size_t Client::writeEncodedStr(const char* str){
		const char* ptr = str;
		size_t strl = 0;
		while(*ptr){ strl++; ptr++; }
		return _ioStream->write(strl >> 8) + _ioStream->write(strl & 0xFF) + _ioStream->write(str);
	}
	
	size_t Client::writeEncodedStr(Msg* ptr){
		if(!ptr) return 0;
		
		//encode string
		size_t slen = ptr->size();
		size_t w  = _ioStream->write(slen >> 8) + _ioStream->write(slen & 0xFF);
			   
		//move pointer to beginning (0)
		//and put back old position after write
		size_t old_pos = ptr->position();	
		ptr->seek(0);
		while (ptr->available()) w += _ioStream->write(ptr->read());
		ptr->seek(old_pos);	
		return w;
	}
	
	void Client::addListener(ClientListener* l){
		_lst.push_back(l);
	}
	
	bool Client::removeListener(ClientListener* l){
		int idx = 0;
		for(ClientListener* ptr: _lst){
			if (ptr == l){
				_lst.erase(_lst.begin() + idx);
				return true;
			}
			idx++;
		}
		return false;
	}
	
	bool Client::run(){
		//check if io socket still connected
		if (!connected()) return false;
		
		//check ping timeout
		if (time(NULL) - _lastPing > _connOpt.keepAlive){
			_pingOutstanding = true;
			if (Packet::sendPingRequest(this) != ClientState::connected){
				disconnect();
				return false;
			}
		}
		
		int idx = 0;
		while( _ioStream->available() ){
			
			uint8_t CTRL = _ioStream->peek();
			
			if (CTRL == Packet::MQTT_PINGREQ){
				_state = Packet::sendPingResponse(this);
				if (_state != ClientState::connected){	
					disconnect();
					return false;
				}
				return true;
			}
			else if (CTRL == Packet::MQTT_PINGRESP){
				_state = Packet::parsePingResponse(this);
				if (_state != ClientState::connected){
					disconnect();
					return false;
				}
				_pingOutstanding = false;
			}
			else if (CTRL == Packet::MQTT_SUBACK){
				uint16_t pid;
				std::vector<uint8_t> qos;
				_state = Packet::parseSubAck(this, &pid, &qos);
				if (_state != ClientState::connected){
					disconnect();
					return false;
				}
				for(ClientListener* l: _lst) l->onSubscribe(pid, &qos);
			}
			else if (CTRL == Packet::MQTT_UNSUBACK){
				uint16_t pid;
				_state = Packet::parseUnsubAck(this, &pid);
				if (_state != ClientState::connected){
					disconnect();
				}
				for(ClientListener* l: _lst) l->onUnsubscribe(pid);
			}
			else if (CTRL == Packet::MQTT_PUBLISH){
				uint16_t pid;
				std::string topic;
				if (_buffer->type() == MSG_FILE_STREAM) ((FileStreamMsgImpl*) _buffer->implPtr())->open();
				Packet::parsePublish(this, &pid, &topic, _buffer);
				for(ClientListener* l: _lst) {
					_buffer->seek(0); //set cursor to beginning
					l->onMessage(pid, topic.c_str(), _buffer);
				}
				if (_buffer->type() == MSG_FILE_STREAM){
					((FileStreamMsgImpl*) _buffer->implPtr())->close();
				}
			}
			else{
				readByte(&CTRL);
				break;
			}
		}
		
		return true;
	}
};
};