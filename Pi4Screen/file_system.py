#Imports
import serial

#Set the serial communication - baud agreed upon beforehand
ser = serial.Serial('/dev/serial0', 115200, timeout=1)

#Start the program and print the values
print("Starting the Console for the IMP project...")

#Infinitely listen to the D1R32 processing outputs
while True:
    line = ser.readline().decode(errors='ignore').strip()
    if line:
        print(f"KEY: {line}")

