#ifndef _mqtt_client_h_
#define _mqtt_client_h_

#include "Msg.h"

namespace freezyoff{
namespace mqtt{
	
	typedef enum{
		socket_error=		-5,
		connection_timeout=	-4,
		connection_lost=	-3,
		connection_failed=	-2,
		disconnected=		-1,
		connected=			 0,
		bad_protocol=		 1,		//bad MQTT protocol
		bad_identifier=		 2,		//bad client id
		bad_credential=		 3,		//bad username & password
		server_unavailable=	 4,		//server unavailable
		unauthorized=		 5		//client not authorized
	} ClientState;
	
};
};

#include "Packet.h"

namespace freezyoff{	
namespace mqtt{
	
	struct ClientStream : Stream {
		
		virtual void   setSetSocketTimeout(uint16_t t) = 0;
		virtual uint16_t socketTimeout()  = 0;
		
		virtual bool connect(const char* host, uint16_t port)= 0;
		virtual void disconnect() = 0;
		virtual bool connected() = 0;
		
		//read byte and store to destination.
		//@param dest - return bytes
		//@param ms - elapse time in millisecond
		virtual bool readByte(uint8_t* dest) = 0;
		virtual bool readBytes(uint8_t* dest, size_t length) = 0;
		virtual bool readBytes(Msg* dest, size_t length) = 0;
		
		virtual size_t writeByte(uint8_t b) = 0;
		virtual size_t writeBytes(uint8_t* b, size_t l) = 0;
		virtual size_t writeBytes(const char* c) = 0;
		
	};
	
	struct ClientListener{
		virtual void onConnected(bool connected) = 0;
		virtual void onSubscribe(uint16_t pid, std::vector<uint8_t>* ackQos) = 0;
		virtual void onUnsubscribe(uint16_t pid) = 0;
		virtual void onPublish(uint16_t pid) = 0;
		virtual void onMessage(uint16_t pid, const char* topic, Msg* msg) = 0;
	};

	struct MsgAllocator{
		virtual Msg* allocate(uint16_t pid, size_t size) = 0;
		virtual void deallocate() = 0;
	};
	
	struct Client {
		Client(ClientStream* io, Msg* buffer) : _ioStream(io), _buffer(buffer) {}
		~Client(){ 
			if (_ioStream) delete _ioStream; 
			if (_buffer) delete _buffer;
		}
		
		virtual void     setSetSocketTimeout(uint16_t t);
		virtual uint16_t socketTimeout();
		
		virtual void 		setServer(const char* host, uint32_t port);
		virtual const char* host();
		virtual uint16_t 	port();
		
		virtual void 			 setConnectOpt(packet::Connect& p);
		virtual packet::Connect* connectOpt();
		
		virtual bool connect(packet::Connect& p);
		virtual bool connect(const char* clientId, bool cleanSession=1);
		virtual bool connect(const char* clientId, const char* user, const char* pwd, bool cleanSession = 1, uint16_t keepAlive = MQTT_KEEP_ALIVE);
		virtual void disconnect();
		virtual bool connected();
		
		// virtual bool publish(const char* topic, uint8_t qos, bool retain);
		virtual bool publish(const char* topic, uint8_t* msg, size_t msglen, uint8_t qos=0, bool retain=0, bool duplicate=0);
		virtual bool publish(const char* topic, const char* msg, uint8_t qos=0, bool retain=0, bool duplicate=0);
		virtual bool publish(const char* topic, Msg* msg, uint8_t qos=0, bool retain=0, bool duplicate=0);
		virtual bool publish(uint16_t* pid, const char* topic, Msg* msg, uint8_t qos, bool retain, bool duplicate);
		virtual bool publish(uint16_t* pid, const char* topic, uint8_t* msg, size_t msglen, uint8_t qos, bool retain, bool duplicate);
		
		virtual bool subscribe(const char* topic, uint8_t qos = 0);
		virtual bool subscribe(uint16_t* pid, const char* topic, uint8_t qos = 0);
		virtual bool subscribe(uint16_t* pid, const char** topics, uint8_t qos, uint16_t len);
		virtual bool subscribe(uint16_t* pid, const char** topics, uint8_t* qos, uint16_t len);
		
		virtual bool unsubscribe(const char* topic);
		virtual bool unsubscribe(uint16_t* pid, const char** topic, uint16_t len);
		
		virtual void    	setState(ClientState s);
		virtual ClientState state();
		
		void addListener(ClientListener* l);
		bool removeListener(ClientListener* l);
		
		bool loop(){ return run(); }
		bool run();
		
		protected:
		ClientStream*   _ioStream;
		Msg*			_buffer;
		
		ClientState     _state;
		const char*     _clientId;
		const char*     _user;
		const char*     _pwd;
		const char*     _host;
		uint16_t 	    _port;
		
		packet::Connect _connOpt;
		
		time_t _lastPing;
		bool   _pingOutstanding = false;
		
		std::vector<ClientListener*> _lst;
		
		bool readByte(uint8_t* dest);
		bool readBytes(uint8_t* dest, size_t l);
		bool readBytes(Msg* dest, size_t l);
		
		size_t writeByte(uint8_t b);
		size_t writeBytes(uint8_t* b, size_t l);
		size_t writeBytes(const char* c);
		
		//encode string to mqtt packet bytes & send.
		size_t writeEncodedStr(const char*);
		size_t writeEncodedStr(Msg*);
		
		static uint8_t  encodePacketLength(uint8_t* dest, uint32_t length);
		static uint32_t decodePacketLength(Client* _c);
		
		friend class Packet;
	};
	
};
};

#include "Packet.h"
#endif