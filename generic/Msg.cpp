#include "Msg.h"

namespace freezyoff{
namespace mqtt{
	
	StrBufferMsgImpl& StrBufferMsgImpl::operator=(const char* str){
		if (strcmp(str, (const char*) _msg) == 0) return *this;
		delete[] _msg;
		_max_size = strlen(str);
		_msg = new uint8_t[_max_size];
		_pos = _len = 0;
		write(str);
		return *this;
	}
		
	StrBufferMsgImpl& StrBufferMsgImpl::operator+(const char* str){
		size_t olen = _len;
		uint8_t* obytes = _msg;
		
		_max_size = size() + strlen(str);
		_msg = new uint8_t[_max_size];
		_pos = _len = 0;
		
		//write old data
		if (obytes) {
			write(obytes, olen);
			delete[] obytes;
		}
		
		//write current str data
		write(str);
		return *this;
	}
			
	StrBufferMsgImpl& StrBufferMsgImpl::operator+=(const char* str){ return operator+(str); }
	
	int  StrBufferMsgImpl::available(){ return streamRemaining()>0; }
	int  StrBufferMsgImpl::read(){ return (char)_msg[_pos++]; }
	int  StrBufferMsgImpl::peek(){ return (char)_msg[_pos]; }
	bool StrBufferMsgImpl::seek(uint32_t pos){ _pos = (size_t) pos; }
	
	size_t StrBufferMsgImpl::write(uint8_t b){ 
		if (_pos < _max_size) {
			_msg[_pos++] = b; 
			_len++;
			return 1;
		}
		return 0;
	}
	
	size_t StrBufferMsgImpl::write(uint8_t* b, size_t l) { 
		size_t wlen = 0;
		for(size_t i=0; i<l; i++) wlen += write(b[i]);
		return wlen;
	}
		
	size_t StrBufferMsgImpl::write(const char* str) { return write((uint8_t*) str, strlen(str)); }

	void StrBufferMsgImpl::flush(){ /*do nothing*/ }
	
	size_t StrBufferMsgImpl::position(){ return _pos; }
	size_t StrBufferMsgImpl::size(){ return _len; }
	ssize_t StrBufferMsgImpl::streamRemaining() { return size() - _pos; }
	
};

namespace mqtt{
	
	int    Msg::available(){ return _ctx->available(); }
	int    Msg::read() { return _ctx->read(); }
	bool   Msg::seek(size_t pos){ return _ctx->seek(pos); }
	int    Msg::peek(){ return _ctx->peek(); }
	
	size_t Msg::write(uint8_t b) { _ctx->write(b); }
	size_t Msg::write(uint8_t* b, size_t l) { _ctx->write(b, l); }
	size_t Msg::write(const char* str) { _ctx->write(str); }
	void   Msg::flush() { _ctx->flush(); }
		
	size_t Msg::size(){ return _ctx->size(); }
	size_t Msg::length(){ return _ctx->size(); }
	size_t Msg::position(){ return _ctx->position(); }
	
	ssize_t Msg::streamRemaining() { return _ctx->streamRemaining(); }
};
};