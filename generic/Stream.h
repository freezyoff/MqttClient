#ifndef _mqtt_stream_h_
#define _mqtt_stream_h_

namespace generic{
namespace mqtt{
	
	//stack dependant
	struct Stream{
		
		virtual int available() = 0;
		virtual int read() = 0;
		virtual int peek() = 0;
		
		virtual size_t write(uint8_t) = 0;
		virtual size_t write(uint8_t*,size_t) = 0;
		virtual size_t write(const char*) = 0;
		virtual void   flush()=0;
		
		virtual ssize_t streamRemaining() = 0;
		
	};
	
};
};

#endif