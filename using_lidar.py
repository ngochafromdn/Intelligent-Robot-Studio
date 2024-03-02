from math import floor
from rplidar import RPLidar
import serial
import datetime

# Setup the RPLidar
lidar = RPLidar('/dev/ttyUSB1', timeout= 0.01)



# #Set up Arduino
ARDUINO_PORT_NAME = '/dev/ttyUSB0'
arduino = serial.Serial(port = ARDUINO_PORT_NAME, baudrate = 256000, timeout= 0.01)

# used to scale data to fit on the screen
max_distance = 0


def linear_interpolation(angle, data):
    """
    Perform linear interpolation to estimate distance at a specific angle.

    Parameters:
    - angle (float): The target angle for interpolation.
    - data (list): List of tuples containing (quality, angle, distance) data from the lidar scan.

    Returns:
    - float: Estimated distance at the specified angle using linear interpolation.
    """
    # Sort data based on angle
    sorted_data = sorted(data, key=lambda x: x[1])

    if angle == 0:
        angle1, distance1 = sorted_data[0][1], sorted_data[0][2]
        angle2, distance2 = sorted_data[-1][1], sorted_data[-1][2]
        quality1 = sorted_data[0][0]
        quality2 = sorted_data[-1][0]
        if quality1 <14 or quality2 <14:
            return None
        interpolated_distance = distance1 + (angle - angle1) * (distance2 - distance1) / (angle2 - angle1)
        return interpolated_distance
    else:
        # Find the two closest data points for interpolation
        for i in range(len(sorted_data) - 1):
            if int(sorted_data[i][1]) <= angle <= sorted_data[i + 1][1]:
                angle1, distance1 = sorted_data[i][1], sorted_data[i][2]
                angle2, distance2 = sorted_data[i + 1][1], sorted_data[i + 1][2]
                quality1 = sorted_data[i][0]
                quality2 = sorted_data[i+1][0]
                if quality1 <14 or quality2 <14:
                    return None
                # Perform linear interpolation
                interpolated_distance = distance1 + (angle - angle1) * (distance2 - distance1) / (angle2 - angle1)
                return interpolated_distance
    # If the target angle is outside the provided range, return None or handle as needed
    return None



def process_data(data):
    HEAD_DATA = linear_interpolation(0, data)
    LEFT_DATA = linear_interpolation(270, data)
    if (HEAD_DATA != None) and (LEFT_DATA != None):
        print(datetime.datetime.now())
        print('head data',HEAD_DATA, ' left data  ', LEFT_DATA)
        head = (300 > HEAD_DATA) and (HEAD_DATA > 0)
        nohead = 300 <= HEAD_DATA 
        left = LEFT_DATA < 450
        noleft = LEFT_DATA >= 450
        
        if nohead and left:
            # arduino.write(b'21')
            print('No head && Left')
        elif  head and left:
            arduino.write(b'h\n')
            print('Head && Left')
        
        elif nohead and noleft:
            # arduino.write(b'22')
            print('Py: No head && No left')
        elif head and noleft:
            arduino.write(b'h\n')
            print('Py: Head && No left')
        dta1 = arduino.readline()
        print(dta1)

    

try:

    # print(lidar.get_info())
    for scan in lidar.iter_scans():
        for (_, angle, distance) in scan:
            angle =  ([359, floor(angle)])
        process_data(scan)

except KeyboardInterrupt:
    print('I am crying')


lidar.stop()
lidar.disconnect()
