import robot_move
import robot_camera
import robot_led

import time
import serial
import cv2
import numpy as np

speed_set = 100
rad = 0.5
buf_size = 1024
ser = serial.Serial ("/dev/ttyUSB0", 115200)

def robot_ctrl(command_input):
    global direction_command, turn_command
    if 'F' == command_input:
        direction_command = 'forward'
        robot_move.move(speed_set, 'forward', 'no', rad)

    elif 'B' == command_input:
        direction_command = 'backward'
        robot_move.move(speed_set, 'backward', 'no', rad)

    elif 'L' == command_input:
        turn_command = 'left'
        robot_move.move(speed_set, 'no', 'left', rad)

    elif 'R' == command_input:
        turn_command = 'right'
        robot_move.move(speed_set, 'no', 'right', rad)

    elif 'S' in command_input:
        turn_command = 'no'
        direction_command = 'no'
        robot_move.move(speed_set, 'no', 'no', rad)

def robot_pic():
    image_name = robot_camera.take_picture()
    with open(image_name, "rb") as image:
        f = image.read()
        b = bytearray(f)
        print(b)
        ser.write(b)
        ser.write('\n'.encode('UTF-8'))

def receive_msg():

    direction_command = 'no'
    turn_command = 'no'

    #receive
    while(ser.in_waiting <= 0):
        continue
    data = ser.readline().decode('UTF-8').rstrip()
    print("Received ["+data+"]")

    if(data == 'F' or data == 'B' or data == 'R' or data == 'L' or data == 'S'):
        robot_ctrl(data)
    elif(data == 'P'):
        robot_pic()
    elif(data == 'Z'):
        return False
    else:
        data = "did not recognize command"
        send_msg(data)

    return True

def send_msg(data):
    ser.write(data.encode('UTF-8'))
    #print("Sent ["+data+"]")

if __name__ == '__main__':
    #init motor control
    robot_move.setup()

    #test motion
    #robot_move.move(speed_set, 'forward', 'no', rad)
    #time.sleep(0.5)
    #robot_move.move(speed_set, 'backward', 'no', rad)
    #time.sleep(0.5)
    #robot_move.move(speed_set, 'no', 'no', rad)

    send_msg("Hello from robot")

    while(receive_msg()):
        continue

    robot_move.destroy()