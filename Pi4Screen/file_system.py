import serial
ser = serial.Serial('/dev/serial0', 115200, timeout=1)
print("Listening on /dev/serial0 @115200 ...")
while True:
    line = ser.readline().decode(errors='ignore').strip()
    if line:
        print(f"KEY: {line}")

