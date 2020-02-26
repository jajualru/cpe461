import array
import serial
import time
import packets # TODO: import directly to remove unnecessary references
import struct
from mega import Mega

# connects to an Arduino Mega for controlling a robot
class MegaSerial:
    # constants
    BAUD_RATE = 9600

    # serial object
    ser = None

    # virtual Mega tracking object
    localMega = None

    # byte array for reading packets
    packet = bytearray(packets.MAX_PACKET_LENGTH)
    packetLength = 0

    # ping response tracker
    pingResponse = False

    # pin initialization check
    pinInitSafe = None

    # connects to arduino
    def setupConnection(self):
        self.ser=serial.Serial("/dev/ttyS0", self.BAUD_RATE)
        time.sleep(0.3) # wait to be safe

        # reset mega
        self.sendPacket(packets.PACKET["RESET"], None)
        time.sleep(1) # wait to be safe

        # wait for startup ack
        watchdog = time.time() + 3
        while(self.localMega is not None):
            if time.time() > watchdog:
                print("Error: Connection timeout")
                exit()
        time.sleep(0.3) # wait to be safe

    # processes all available serial data
    def processSerial(self):
        # check if connection has been set up
        if self.ser is None:
            print("Error: Serial connection not initialized")
            return

        # read data as long as it is available
        while self.ser.in_waiting > 0:
            # read next byte
            self.packet[self.packetLength] = ord(self.ser.read(1))
            self.packetLength += 1

            # switch on packet type
            if self.packet[0] == packets.PACKET["ERROR"]:
                self.processError()
                continue # TODO: is this continue necessary?

            elif self.packet[0] == packets.PACKET["DIGITAL_DATA"]:
                # TODO: refactor to separate function
                if self.packetLength < 3:
                    continue
                # print data
                print("Pin " + str(self.packet[1]) + ": " + str(self.packet[2]))

                # update pin structure
                self.localMega.setPinData(self.packet[1], self.packet[2])

                # reset after complete packet
                self.packetLength = 0

            elif self.packet[0] == packets.PACKET["PING"]:
                # mark ping response as received
                self.pingResponse = True

                # reset after complete packet
                self.packetLength = 0

            elif self.packet[0] == packets.PACKET["PIN_ACK"]:
                # update pin initialization state
                self.pinInitSafe = True

                # reset after complete packet
                self.packetLength = 0

            elif self.packet[0] == packets.PACKET["STARTUP"]:
                # check if mega is already initialized
                if self.localMega is not None:
                    print("Error: Mega reset")
                    exit()

                # initialize local mega
                self.localMega = Mega()

                # reset after complete packet
                self.packetLength = 0
               
            # TODO: add other packet types here

            else:
                # report bad packet and throw out data
                print("Error: Unrecognized op code")
                self.packetLength = 0

    # reports an error
    def processError(self):
        # check for error message contents
        if self.packetLength < 2:
            return
        # check errors of length 2
        elif self.packetLength == 2:
            if self.packet[1] == packets.ERROR["INVALID_PACKET"]:
                print("Error: Arduino did not recognize packet")
                # reset after complete packet
                self.packetLength = 0
        # check errors of length 3
        elif self.packetLength == 3:
            if self.packet[1] == packets.ERROR["INVALID_PIN"]:
                print("Error: Invalid pin " + str(self.packet[2]))
                self.pinInitSafe = False
                # reset after complete packet
                self.packetLength = 0

        # TODO: add other errors here
        
        # unrecognized error
        else:
            print("Error: Unrecognized error message")

    # sends a packet to the Arduino
    def sendPacket(self, opCode, msg):
        self.ser.write(struct.pack('B', opCode))
        if not msg is None:
            self.ser.write(msg)

    # sets up a pin for digital reading
    def setPinDigitalRead(self, pin):
        # reset pin init state
        self.pinInitSafe = None

        # send request to Mega
        data = bytearray()
        data.append(pin)
        data.append(0)
        self.sendPacket(packets.PACKET["PIN_INIT"], data)

        # wait for ack or error
        while(self.pinInitSafe is None):
            self.processSerial()

        # check for error
        if not self.pinInitSafe:
            exit()

        # update local Mega
        self.localMega.initializePin(pin, 0)

    # sends a ping to the Arduino and returns the response time in decimal seconds or -1 on error
    def ping(self):
        # check if connection has been set up
        if self.ser is None:
            print("Error: Serial connection not initialized")
            return

        # reset ping tracker and start timer
        self.pingResponse = False
        start = time.time()
        end = time.time()

        # send ping
        self.sendPacket(packets.PACKET["PING"], None)

        # block until response or 1 second elapsed
        while((not self.pingResponse) and (end < start + 1)):
            # block until data available for more precise timing
            while(self.ser.in_waiting == 0):
               None
            self.processSerial()
            end = time.time()

        # check for timeout
        if end >= start + 1:
            return -1

        # return elapsed time
        return end - start

