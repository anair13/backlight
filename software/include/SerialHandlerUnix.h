#ifndef SERIALHANDLER_UNIX_H
#define SERIALHANDLER_UNIX_H

#include "boost/asio.hpp"

class SerialHandler
{
private:
    const unsigned int BUFFER_SIZE = 128;
    unsigned char* buf;
    unsigned int bufIndex;

    void writeToBuffer(unsigned char c) {
        buf[bufIndex] = c;
        bufIndex++;
        if (bufIndex >= BUFFER_SIZE) {
		    boost::asio::write(serial, boost::asio::buffer(buf, sizeof(buf)));
            bufIndex = 0;
        }
    }
public:

    ~SerialHandler() {
        delete buf;
    }

	SerialHandler(std::string port, unsigned int baudRate) : io(), serial(io, port)
	{
		serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
        buf = new unsigned char[BUFFER_SIZE];
        bufIndex = 0;
	}

	void writeSerial(unsigned char r, unsigned char g, unsigned char b) {
        writeToBuffer(0xFF);
        writeToBuffer(r);
        writeToBuffer(g);
        writeToBuffer(b);
    }

private:
	boost::asio::io_service io;
	boost::asio::serial_port serial;
};

#endif
