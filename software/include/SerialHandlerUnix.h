#ifndef SERIALHANDLER_UNIX_H
#define SERIALHANDLER_UNIX_H

#include "boost/asio.hpp"

class SerialHandler
{
public:
	SerialHandler(std::string port, unsigned int baudRate) : io(), serial(io, port)
	{
		serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
	}

	void writeSerial(unsigned char r, unsigned char g, unsigned char b) {
		unsigned char c[4];
		c[0] = 0xFF;
		c[1] = r;
		c[2] = g;
		c[3] = b;
		boost::asio::write(serial, boost::asio::buffer(c, sizeof(c)));
	}

private:
	boost::asio::io_service io;
	boost::asio::serial_port serial;
};

#endif
