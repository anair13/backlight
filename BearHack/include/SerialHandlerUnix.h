#ifndef SERIALHANDLER_UNIX_H
#define SERIALHANDLER_UNIX_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>     // string function definitions
#include <fcntl.h>      // File control definitions
#include <termios.h>    // POSIX terminal control definitions

class SerialHandler
{
public:
	SerialHandler(std::string port, unsigned int baudRate)
	{
        /*
        int serial = open(port.c_str(), O_RDWR|O_NOCTTY);

        struct termios tty;
        struct termios tty_old;
        memset(&tty, 0, sizeof tty);

        tty_old = tty;

        cfsetospeed (&tty, (speed_t)baudRate);
        cfsetispeed (&tty, (speed_t)baudRate);

        tty.c_cflag &= ~PARENB;        // Make 8n1
        tty.c_cflag     &=  ~CSTOPB;
        tty.c_cflag     &=  ~CSIZE;
        tty.c_cflag     |=  CS8;

        tty.c_cflag     &=  ~CRTSCTS;       // no flow control
        tty.c_cc[VMIN]      =   1;                  // read doesn't block
        tty.c_cc[VTIME]     =   5;                  // 0.5 seconds read timeout
        tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

        cfmakeraw(&tty);

        tcflush(serial, TCIFLUSH );
        */
        file = fopen(port.c_str(), "w");
    }

	void writeSerial(unsigned char r, unsigned char g, unsigned char b) {
		unsigned char c[4];
		c[0] = 0xFF;
		c[1] = r;
		c[2] = g;
		c[3] = b;
        //write(serial, c, sizeof(c));
        int i = 0;
        for(i = 0 ; i < sizeof(c) ; i++)
        {
            fprintf(file,"%d", c[i]); //Writing to the file
            fprintf(file,"%c",','); //To separate digits
        }
        //fclose(file);
	}

private:
    FILE* file;
};

#endif
