#Imports
import serial
import sys

def wait_for_result():
    try:
        while True:
            data = ser.readline()
            if data:
                #Retrun the data exactly
                sys.stdout.write(data.decode(errors='ignore'))
                sys.stdout.flush()
    except Exception as e:
        sys.stdout.write(f"\n[serial_reader stopped: {e}]\n")

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
try:
    while(True):
        #Read a whole line from the console - blocked until Enter
        read_line = sys.stdin.readline()
        #if user terminates the communication
        if read_line == "":  # EOF
            break
        #Ensure we end with '!'
        read_line += "!"
        #Send the data and flush it
        ser.write(read_line.encode())
        ser.flush()
        #Wait for the response from the D1R32

except:
    pass
finally:
    #Inform about closing the link and close it
    print("\nClosing serial…")
    ser.close()

