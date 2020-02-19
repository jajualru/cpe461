import time
from packets import PACKET
from mega_serial import MegaSerial
from mega import Mega

# sets up a connection and sends some commands
# - sets up pin 8 for digital reading
# - sends requests to pin 8 and monitors when data is available
def main():
    # create objects
    mega = MegaSerial()

    # initialize connection to Arduino
    mega.setupConnection()

    # send initial data
    # initialize pin 8 to input
    mega.setPinDigitalRead(8)

    # turn on Arduino LED
    mega.sendPacket(PACKET["LED_HIGH"], None)

    # enter loop
    while(True):
        # read pin status
        print("A " + str(mega.localMega.getPinState(8)))

        # request status of pin 8 and update pin structure
        mega.sendPacket(PACKET["DIGITAL_READ"], bytearray(b'\x08'))
        mega.localMega.setPinNotReady(8)

        # read pin status
        print("B " + str(mega.localMega.getPinState(8)))

        # poll for response
        mega.processSerial()

        # read pin status
        print("C " + str(mega.localMega.getPinState(8)))

        time.sleep(1)

if __name__ == "__main__":
    main()

