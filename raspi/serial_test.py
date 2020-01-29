import serial
import time

# connect to arduino
ser=serial.Serial("/dev/ttyS0",9600)
time.sleep(2)

while True:
    val='r'
    print("Sending " + val)
    ser.write(bytes(val.encode("ascii")))
    print("Sent")

    time.sleep(0.5)

