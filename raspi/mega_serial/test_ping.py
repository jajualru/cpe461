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

    # send a ping and print the result
    pingTime = mega.ping()

    print("Ping time: " + str(pingTime))

if __name__ == "__main__":
   main()

