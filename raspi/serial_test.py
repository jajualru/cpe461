import array
import serial
import time

# connect to arduino
ser=serial.Serial("/dev/ttyS0",9600)
time.sleep(2)

while True:
    # blink arduino led
    val = array.array('B', [0x55])
    print("Sending high")
    ser.write(val)
    time.sleep(1)
    
    val = array.array('B', [0xAA])
    print("Sending low")
    ser.write(val)
    time.sleep(1)

    # send both blink commands as double packet
    val = array.array('B', [0xAA, 0x55])
    print("Sending both")
    ser.write(val)
    time.sleep(1)

