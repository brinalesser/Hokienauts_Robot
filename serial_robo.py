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
    data = ser.read()
    sleep(0.03)
    data_left = ser.inWaiting()             #check for remaining byte
    data += ser.read(data_left)
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
=======
import move
import time
import serial

speed_set = 100
rad = 0.5
buf_size = 1024
ser = serial.Serial ("/dev/ttyUSB0", 115200)
ser.reset_input_buffer()
        
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

def receive_msg(data):
    
    direction_command = 'no'
    turn_command = 'no'
    
    if(data == 'F' or data == 'B' or data == 'R' or data == 'L' or data == 'S'):
        robot_ctrl(data)
    elif(data == 'Z'):
        return false
    else:
        data = "did not recognize command"
        send_msg(data)
    
    return true
 
def send_msg(data):
    data += '\n'
    ser.write(data.encode(encoding='UTF-8'))
    print("Sent ["+data+"]")

#init motor control
move.setup()

#test motion
move.move(speed_set, 'forward', 'no', rad)
time.sleep(0.5)
move.move(speed_set, 'backward', 'no', rad)
time.sleep(0.5)
move.move(speed_set, 'no', 'no', rad)

send_msg("Hello from robot")

exit_flag = false
while(!exit_flag):
    if(ser.in_waiting > 0):
        data = ser.readline().decode('UTF-8').rstrip()
        print("Received ["+data+"]")
        exit_flag = receive_msg(data)

move.destroy()

>>>>>>> 0b2d71b3fa4f6c04e05cd7a1ace0f845dfb4eb0c
