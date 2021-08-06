#include "Mqtt.h"

namespace arduino_esp8266 {
namespace mqtt{
	
	FileStreamMsg::FileStreamMsg(const char* fname) : _name(fname) 
	{
		LittleFS.remove(_name);	//clean start
	}
		
	FileStreamMsg::~FileStreamMsg(){ 
		LittleFS.remove(_name); 
	}
		
	//open file for writing & reading. Should truncate file when opening.
	bool FileStreamMsg::open() { 
		if (!_f) _f = LittleFS.open(_name, "w+");
		if (_f)  return true; 
		return false; 
	}
	
	void FileStreamMsg::close() { _f.close(); }
	bool FileStreamMsg::remove() { return LittleFS.remove(_name); }
		
	int  FileStreamMsg::available() { return _f.available(); }
	int  FileStreamMsg::read() { return _f.read(); }
	int  FileStreamMsg::peek() { return _f.peek(); }
		
	size_t FileStreamMsg::write(uint8_t b) { return _f.write(b); }
	size_t FileStreamMsg::write(uint8_t* b, size_t l) { return _f.write(b, l); }
	size_t FileStreamMsg::write(const char* str) { _f.write(str); }
		
	void FileStreamMsg::flush() { _f.flush(); }
		
	bool    FileStreamMsg::seek(size_t pos) { return _f.seek(pos); }
	size_t  FileStreamMsg::position() { return _f.position(); }
	size_t  FileStreamMsg::size() { _f.size(); }
	ssize_t FileStreamMsg::streamRemaining() { _f.size() - _f.position(); }
		
};

namespace mqtt{
		
	bool SecureClientStream::connect(const char* host, uint16_t port) {
		disconnect();
		_client = WiFiClientSecure();
		_client.setCACert(_tls_der, _tls_len);
		return _client.connect(host, port);
	}
		
	void SecureClientStream::disconnect() { _client.stop(); }
	bool SecureClientStream::connected(){ return _client.connected(); }
		
	int SecureClientStream::available()  { return _client.available(); }
	ssize_t SecureClientStream::streamRemaining()  { 
		//NOTE: if posible use _client.streamRemaining(); 
		return SIZE_MAX;
	}
		
	int SecureClientStream::read()  { return _client.read(); }
	int SecureClientStream::peek()  { return _client.peek(); }
	
	size_t SecureClientStream::write(uint8_t b)  { return _client.write(&b, 1); }
	size_t SecureClientStream::write(uint8_t* b, size_t l)  { return _client.write(b, l); }
	size_t SecureClientStream::write(const char* s)  { return _client.write(s); }
	void   SecureClientStream::flush() { _client.flush(); }
		
	void SecureClientStream::setCACert(unsigned char* der, unsigned int length){
		_tls_der = der;
		_tls_len = length;
	}

	void SecureClientStream::setSetSocketTimeout(uint16_t t) { 
		_socket_timeout = t; 
		_client.setTimeout(t*1000UL); 
	}
	uint16_t SecureClientStream::socketTimeout() { return _socket_timeout; }
		
	bool SecureClientStream::readByte(uint8_t* dest) {
		time_t now = time(NULL);
		while(!_client.available()) {
			yield();
			if(time(NULL) - now > socketTimeout()) {
				return false;
			}
		}
		*dest = read();
		return true;
	}
		
	bool SecureClientStream::readBytes(uint8_t* dest, size_t length) {
		int pos = 0;
		while (pos < length){
			if (!readByte(&dest[pos++])) return false;
		}
		return true;
	}
		
	bool SecureClientStream::readBytes(MqttMsg* dest, size_t length) {
		int pos = 0;
		while (pos < length){
			uint8_t cbyte = 0;
			if (!readByte(&cbyte)) 		return false;
			if (dest->write(cbyte) != 1)return false;
			pos++;
		}
		return true;
	}
		
	size_t SecureClientStream::writeByte(uint8_t b) { return _client.write(&b, 1); }
	size_t SecureClientStream::writeBytes(uint8_t* b, size_t l) { return _client.write(b, l); }
	size_t SecureClientStream::writeBytes(const char* c) { return _client.write(c); }
	
};

namespace mqtt{
	
	MqttSecureClient::MqttSecureClient() : 
		BaseClient( 
			new SecureClientStream(), 
			new MqttMsg( new FileStreamMsg("/mqtt.buff") ) 
		)
	{	
		addListener(this);
	}
	
	MqttSecureClient::~MqttSecureClient(){ removeListener(this); }
	
	void MqttSecureClient::setCACert(unsigned char* der, unsigned int length){
		((SecureClientStream*) _ioStream)->setCACert(der, length);
	}
	
	void MqttSecureClient::onConnected(bool connected) { 
		if (_callback_connect) _callback_connect(connected); 
	}
	
	void MqttSecureClient::setOnConnectedCallback(MQTT_CALLBACK_CONNECT(clb)){ 
		_callback_connect = clb; 
	}
	
	void MqttSecureClient::onSubscribe(uint16_t pid, std::vector<uint8_t>* ackQos) { 
		if(_callback_subscribe) _callback_subscribe(pid, ackQos); 
	}
	
	void MqttSecureClient::setOnSubscribeCallback(MQTT_CALLBACK_SUBSCRIBE(clb)){ 
		_callback_subscribe = clb; 
	}
	
	void MqttSecureClient::onUnsubscribe(uint16_t pid) { 
		if (_callback_unsubscribe) _callback_unsubscribe(pid); 
	}
	
	void MqttSecureClient::setOnUnsubscribeCallback(MQTT_CALLBACK_UNSUBSCRIBE(clb)){ 
		_callback_unsubscribe = clb; 
	}
	
	void MqttSecureClient::onPublish(uint16_t pid)  { 
		if (_callback_publish) _callback_publish(pid); 
	}
	
	void MqttSecureClient::setOnPublishCallback(MQTT_CALLBACK_PUBLISH(clb)){
		_callback_publish = clb;
	}
	
	void MqttSecureClient::onMessage(uint16_t pid, const char* topic, MqttMsg* msg) {
		if (_callback_message) _callback_message(pid, topic, msg);
	}
	
	void MqttSecureClient::setOnMessageCallback(MQTT_CALLBACK_MESSAGE(clb)){
		_callback_message = clb;
	}
	
};
};