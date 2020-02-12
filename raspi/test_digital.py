import array
import serial
import time

# connect to arduino
ser=serial.Serial("/dev/ttyS0",9600)
time.sleep(2)

# set up digital pin 8 as input
val = bytearray(b'\xf0\x08\x00')
print("Setting pin 8 to input")
ser.write(val)

# request value on pin 8
val = bytearray(b'\x10\x08')
print("Reading pin 8")
ser.write(val)

while True:
    # check for full read packet
    if ser.in_waiting >= 3:
        print("Received packet (printing size of 3):")
        print(list(ser.read(3)))
        #print(ser.read(1))

        # wait before next request
        time.sleep(1)

        # request value on pin 8
        val = bytearray(b'\x10\x08')
        print("Reading pin 8")
        ser.write(val)

