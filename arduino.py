import serial
from time import sleep
import array

port = serial.Serial('/dev/ttyACM0', 9600)

def send(message):
	port.write(message)

def sendColor(r, g, b):
	bytes = array.array('B', [0xff, r, g, b]).tostring()
	print(repr(bytes))
	send(bytes)

if __name__ == "__main__":
	for i in range(255):
		sendColor(i, 0, 0)
		sleep(.01)
	for i in range(255):
		sendColor(255 - i, i, 0)
		sleep(.01)
	for i in range(255):
		sendColor(0, 255 - i, i)
		sleep(.01)
	sendColor(0,0,0)