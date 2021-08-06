#ifndef _jacc_arduinoesp8266_mqtt_h_
#define _jacc_arduinoesp8266_mqtt_h_

#include <WiFiClientSecure.h>
#include <LittleFS.h>

#include "../generic/Client.cpp"

namespace arduino_esp8266 {
namespace mqtt{
	
	typedef freezyoff::mqtt::Msg 	  	  MqttMsg;
	typedef freezyoff::mqtt::ClientStream BaseClientStream;
	typedef freezyoff::mqtt::Client  	  BaseClient;
	
	struct FileStreamMsg : freezyoff::mqtt::FileStreamMsgImpl {
		FileStreamMsg(const char* fname);
		
		~FileStreamMsg();
		
		//open file for writing & reading. Should truncate file when opening.
		bool open() override;
		void close() override;
		bool remove() override;
		
		int  available() override;
		int  read() override;
		int  peek() override;
		
		size_t write(uint8_t b) override;
		size_t write(uint8_t* b, size_t l) override;
		size_t write(const char* str) override;
		
		void flush() override;
		
		bool seek(size_t pos) override;
		size_t  position() override;
		size_t  size() override;
		ssize_t streamRemaining() override;
		
		protected:
		File _f;
		const char* _name;
	};
	
	struct SecureClientStream : BaseClientStream{
		
		bool connect(const char* host, uint16_t port) override;
		void disconnect() override;
		bool connected() override;
		
		int 	available() override;
		ssize_t streamRemaining() override;
		
		int read() override;
		int peek() override;
	
		size_t write(uint8_t b) override;
		size_t write(uint8_t* b, size_t l) override;
		size_t write(const char* s) override;
		void   flush() override;
		
		void setCACert(unsigned char* der, unsigned int length);

		void setSetSocketTimeout(uint16_t t) override;
		uint16_t socketTimeout() override;
		
		bool readByte(uint8_t* dest) override;
		bool readBytes(uint8_t* dest, size_t length) override;
		bool readBytes(MqttMsg* dest, size_t length) override;
		
		size_t writeByte(uint8_t b) override;
		size_t writeBytes(uint8_t* b, size_t l) override;
		size_t writeBytes(const char* c) override;
		
		protected:
		uint16_t 		 _socket_timeout;
		unsigned char* 	 _tls_der;
		unsigned int   	 _tls_len;
		WiFiClientSecure _client;
	};
	
	
#if defined(ESP8266) || defined(ESP32)
#include <functional>
#define MQTT_CALLBACK_CONNECT(n) std::function<void(bool connected)> n
#define MQTT_CALLBACK_SUBSCRIBE(n) std::function<void(uint16_t pid, std::vector<uint8_t>* ackQos)> n
#define MQTT_CALLBACK_UNSUBSCRIBE(n) std::function<void(uint16_t pid)> n
#define MQTT_CALLBACK_PUBLISH(n) std::function<void(uint16_t pid)> n
#define MQTT_CALLBACK_MESSAGE(n) std::function<void(uint16_t pid, const char* topic, MqttMsg* msg)> n
#else
#define MQTT_CALLBACK_CONNECT(n) void (*n)(bool connected)
#define MQTT_CALLBACK_SUBSCRIBE(n) void (*n)(uint16_t pid, std::vector<uint8_t>* ackQos)
#define MQTT_CALLBACK_UNSUBSCRIBE(n) void (*n)(uint16_t pid)
#define MQTT_CALLBACK_PUBLISH(n) void (*n)(uint16_t pid)
#define MQTT_CALLBACK_MESSAGE(n) void (*n)(uint16_t pid, const char* topic, MqttMsg* msg)
#endif

	
	struct MqttSecureClient : BaseClient, freezyoff::mqtt::ClientListener {
		MqttSecureClient();
		
		~MqttSecureClient();
		
		void setCACert(unsigned char* der, unsigned int length);
		
		void onConnected(bool connected) override;
		void onSubscribe(uint16_t pid, std::vector<uint8_t>* ackQos) override;
		void onUnsubscribe(uint16_t pid) override;
		void onPublish(uint16_t pid) override;
		void onMessage(uint16_t pid, const char* topic, MqttMsg* msg) override;
		
		void setOnConnectedCallback(MQTT_CALLBACK_CONNECT(clb));
		void setOnSubscribeCallback(MQTT_CALLBACK_SUBSCRIBE(clb));
		void setOnUnsubscribeCallback(MQTT_CALLBACK_UNSUBSCRIBE(clb));
		void setOnPublishCallback(MQTT_CALLBACK_PUBLISH(clb));
		void setOnMessageCallback(MQTT_CALLBACK_MESSAGE(clb));
		
		private:
		MQTT_CALLBACK_CONNECT(_callback_connect);
		MQTT_CALLBACK_SUBSCRIBE(_callback_subscribe);
		MQTT_CALLBACK_UNSUBSCRIBE(_callback_unsubscribe);
		MQTT_CALLBACK_PUBLISH(_callback_publish);
		MQTT_CALLBACK_MESSAGE(_callback_message);
	};
	
};
};

typedef arduino_esp8266::mqtt::MqttSecureClient MqttSecureClient;
typedef freezyoff::mqtt::ClientListener MqttListener;
typedef freezyoff::mqtt::Msg MqttMsg;

#endif