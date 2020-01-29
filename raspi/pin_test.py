import RPi.GPIO as GPIO
import time

OUT_PIN = 21

GPIO.setmode(GPIO.BOARD)
GPIO.setup(OUT_PIN, GPIO.OUT)

for i in range(10):
    print("HIGH")
    GPIO.output(OUT_PIN, GPIO.HIGH)
    time.sleep(0.5)
    print("LOW")
    GPIO.output(OUT_PIN, GPIO.LOW)
    time.sleep(0.5)

GPIO.cleanup()

