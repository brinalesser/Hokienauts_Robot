<<<<<<< HEAD
import move
import time
import serial

speed_set = 100
rad = 0.5
buf_size = 1024
ser = serial.Serial ("/dev/ttyS0", 115200)
        
def robot_ctrl(command_input):
    global direction_command, turn_command
    if 'F' == command_input:
        direction_command = 'forward'
        move.move(speed_set, 'forward', 'no', rad)
    
    elif 'B' == command_input:
        direction_command = 'backward'
        move.move(speed_set, 'backward', 'no', rad)

    elif 'L' == command_input:
        turn_command = 'left'
        move.move(speed_set, 'no', 'left', rad)

    elif 'R' == command_input:
        turn_command = 'right'
        move.move(speed_set, 'no', 'right', rad)

    elif 'S' in command_input:
        turn_command = 'no'
        direction_command = 'no'
        move.move(speed_set, 'no', 'no', rad)

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
    elif(data == 'Z'):
        return false
    else:
        data = "did not recognize command"
        send_msg(data)
    
    return true
 
def send_msg(data):
    ser.write(data)
    print("Sent ["+data+"]")

#init motor control
move.setup()

#test motion
#move.move(speed_set, 'forward', 'no', rad)
#time.sleep(0.5)
#move.move(speed_set, 'backward', 'no', rad)
#time.sleep(0.5)
#move.move(speed_set, 'no', 'no', rad)

send_msg("Hello from robot")

while(receive_msg):
    continue

move.destroy()
