#ifndef _mqtt_msg_h_
#define _mqtt_msg_h_

#include "Stream.h"

#define MSG_STR_BUFFER 1
#define MSG_FILE_STREAM 2

namespace freezyoff{
namespace mqtt{
	
	struct MsgImpl : Stream{
		virtual int    type() = 0;
		virtual bool   seek(size_t pos) = 0;	//move stream cursor pointer offset from beginning (SeekSet)
		virtual size_t position() = 0;			//cursor pointer position
		virtual size_t size() = 0;				//size of message in bytes
	};
	
	struct StrBufferMsgImpl : MsgImpl {
		
		StrBufferMsgImpl(size_t size) : _max_size(size), _msg(new uint8_t[size]) { _pos = _len = 0; }
		StrBufferMsgImpl(uint8_t* bytes, size_t size) : StrBufferMsgImpl(size) { _pos = _len = 0; write(bytes, size); }
		StrBufferMsgImpl(const char* str) : StrBufferMsgImpl(strlen(str)) { _pos = _len = 0; write(str); }	
		StrBufferMsgImpl() : StrBufferMsgImpl(1){}
		
		~StrBufferMsgImpl(){ if (_msg) delete[] _msg; }
		
		StrBufferMsgImpl& operator= (const char* str);
		StrBufferMsgImpl& operator+ (const char* str);
		StrBufferMsgImpl& operator+=(const char* str);
		
		int  available() override;
		int  read() override;
		int  peek() override;
		
		size_t write(uint8_t b) override;
		size_t write(uint8_t* b, size_t l) override;
		size_t write(const char* str) override;
		
		void    flush() override;
		
		bool    seek(size_t pos) override;
		size_t  position() override;
		size_t  size() override;
		ssize_t streamRemaining() override;
		
		int type(){ return MSG_STR_BUFFER; }
		
		protected:
			uint8_t* _msg = NULL;
			size_t _pos = 0;
			size_t _len = 0;
			size_t _max_size = 0;
	};
	
	struct FileStreamMsgImpl : MsgImpl {
		int type(){ return MSG_FILE_STREAM; }
		
		virtual bool open() = 0;	//open file for writing & reading. Should truncate file when opening.
		virtual void close() = 0;	//close and save file
		virtual bool remove() = 0;	//remove file
	};
	
	struct Packet;
	struct Client;
	
	struct Msg : MsgImpl{
		Msg(MsgImpl* ctx) : _ctx(ctx){}
		~Msg(){ if (_ctx) delete _ctx; }
		
		operator bool() const{ return _ctx != NULL; }
		
		bool operator==(Msg* p) const { return _ctx ==  p->_ctx; }
		
		int    available() override;
		int    read() override;	
		int    peek() override;
		bool   seek(size_t pos) override;
		
		size_t write(uint8_t b) override;
		size_t write(uint8_t* b, size_t l) override;
		size_t write(const char* str) override;
		void   flush() override;
		
		size_t  size() override;
		size_t  length();
		size_t  position() override;
		ssize_t streamRemaining() override;
		
		int type() override{ return _ctx->type(); }
		
		protected:
		MsgImpl* _ctx = NULL;
		MsgImpl* implPtr(){ return _ctx; }
		
		friend class Client;
		friend class Packet;
		
	};
	
};
};
#endif