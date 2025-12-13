#Imports
import serial

# Define the special commands
START_CMD = "start_cmd"
RST_CMD = "rst_cmd"
OVERFLOW_CMD = "overflow_cmd"

PROMPT_PREFIX = "IMP@file_system:"

def check_special_input(line: str) -> tuple[bool, bool]:
    """Check if the input is any of the special commands
       and react accordingly.

        Returns a tuple.
        First bool is if a special value was detected, second if to 
        shut the program or no.
    """
    if (line == RST_CMD):
        print("The restart was pressed! Shutting down the program")
        return True, True
    if (line == OVERFLOW_CMD):
        print("Overflow on client was detected! The program is terminating")
        return True, True
    return False, False

def read_frame(ser: serial.Serial) -> str | None:
    """Read one input until termination of !
    """
    # Read one ESP->Pi frame terminated by '!'
    data = ser.read_until(b'!')
    if not data:
        return None
    return data.decode(errors="ignore").rstrip("!\r\n")

def wait_for_result(ser: serial.Serial) -> str:
    """Scan the input until something received and then do a quick analysis.

        Returns False if not to shut down the program, True otherwise.
    """
    while True:
        # Read the message from the D1R32
        msg = read_frame(ser)
        if msg is None:
            continue
        # Check if special message send
        special, shutdown = check_special_input(msg)
        if special:
            if shutdown:
                raise SystemExit(0)
            continue
        # Check if we are receiving starting prompt
        if msg.startswith(PROMPT_PREFIX):
            return msg
        # Otherwise just print the receiving data
        if msg:
            print(msg)


def main():
    #Set the serial communication - baud agreed upon beforehand
    ser = serial.Serial('/dev/serial0', 115200, timeout=1)

    #Wait for the D1R32 to send the initiation sequence
    try:
        # Reset the buffer and print starting prompt
        ser.reset_input_buffer()
        print("Waiting for D1R32 to start the communication...")

        # Wait for the starting sequence from D1R32
        first = None
        while first is None:
            first = read_frame(ser)

        # Inform the user and D1R32 of the running program here
        ser.write((START_CMD + "!").encode("utf-8"))
        ser.flush()
        print("Communication established ;). Booting the filesystem now...")

        # Decode
        if first and first.startswith(PROMPT_PREFIX):
            prompt = first
        else:
            if first:
                print(first)
            prompt = wait_for_result(ser)

        # Final interactive loop
        while True:
            # Try and read the input prompt
            try:
                cmd = input(prompt)
            except EOFError:
                break
            # Send the input to D1R32 -> Add th terminator and formate
            ser.write((cmd.strip() + "!").encode("utf-8"))
            ser.flush()
            # Wait for the results from the D1R32
            prompt = wait_for_result(ser)

    except KeyboardInterrupt:
        # Ctrl-C
        pass
    except EOFError:
        # Ctrl-D
        pass
    finally:
        print("\nClosing serial…")
        ser.close()


if __name__ == "__main__":
    main()