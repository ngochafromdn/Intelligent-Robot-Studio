import os
import serial
from math import floor
from adafruit_rplidar import RPLidar


# Set up the RPLidar
PORT_NAME = '/dev/ttyUSB0'
lidar = RPLidar(None, PORT_NAME)

# Set up serial connection
arduino_port = '/dev/ttyUSB1'  # Change this to your Arduino port
arduino_baudrate = 9600
ser = serial.Serial(arduino_port, arduino_baudrate)

# Used to scale data to fit on the screen
max_distance = 0

def process_data(data):
    global max_distance
    serial_array = []

    for angle in [0, 30, 60, 90, 150, 180]:
        distance = data[angle]
        if distance > 0:
            max_distance = max([min([5000, distance]), max_distance])
            
            # Append distance to serial array
            serial_array.append(distance)

    return serial_array

scan_data = [0] * 360

try:
    print(lidar.info)
    for scan in lidar.iter_scans():
        for (_, angle, distance) in scan:
            scan_data[min([359, floor(angle)])] = distance
        serial_data = process_data(scan_data)
        # Send serial data to Arduino
        ser.write(','.join(map(str, serial_data)).encode())
        ser.write(b'\n')

except KeyboardInterrupt:
    print('Stopping.')

finally:
    lidar.stop()
    lidar.disconnect()
    ser.close()
