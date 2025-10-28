#Imports
import serial

#Set the serial communication - baud agreed upon beforehand
ser = serial.Serial('/dev/serial0', 115200, timeout=1)

#Wait for the D1R32 to send the initiation sequence
print("Waiting for D1R32 to start the communication...")
while True:
    line = ser.readline().decode(errors='ignore').strip()
    #If there is an input - Anything is accepted
    #But it will be the initial header for the file system
    if line:
        #Send message acknowledging the presence of the PI
        ser.write(b'!')
        #Then flush it, so no data are left out
        ser.flush()
        print("Communication established ;). Booting the filesystem now...")
        #print the first message - header of the filesystem
        print(line)
        #And exit the loop
        break

#Wait for the user to print to the console

