#Imports
import serial
import sys

# Define the special commands
start_cmd = "start_cmd"
rst_cmd = "rst_cmd"
overflow_cmd = "overflow_cmd"

def check_special_input(line: str) -> tuple[bool, bool]:
    """Check if the input is any of the special commands
       and react accordingly.

        Returns a tuple.
        First bool is if a special value was detected, second if to 
        shut the program or no.
    """
    if (line == rst_cmd):
        print("The restart was pressed! Shutting down the program")
        return True, True
    if (line == overflow_cmd):
        print("Overflow on client was detected! The program is terminating")
        return True, True
    return False, False

def wait_for_result() -> bool:
    """Scan the input until something received and then do a quick analysis.

        Returns False if not to shut down the program, True otherwise.
    """
    try:
        while True:
            data = ser.read_until(b'!')
            line = data.decode(errors='ignore').rstrip("!\r\n")
            # Check no special signals were sent
            special = check_special_input(line)
            # If so exit with signal to end
            if (special[0]):
                return special[1]
            if line:
                #Return the data exactly
                print(line)
                return False
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
        ser.write((start_cmd + "!").encode("utf-8"))
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
        read_line = ser.readline()
        # Check if something returned
        if not read_line:
            continue
        #if user terminates the communication
        if read_line == "":  # EOF
            break

        #Ensure we end with '!'
        read_line = read_line.decode(errors="replace")

        #Send the data and flush it
        if line.startswith("IMP@file_system:"):
            prompt = line.rstrip("\r\n")
            cmd = input(prompt)
            ser.write((cmd.strip() + "!").encode())
            ser.flush()
        else:
            # Normal output from ESP: print as-is
            print(line, end="")
        #Wait for the response from the D1R32
        res: bool = wait_for_result()
        # Shut down the program if true
        if (res):
            break

except Exception:
    pass
finally:
    #Inform about closing the link and close it
    print("\nClosing serial…")
    ser.close()

