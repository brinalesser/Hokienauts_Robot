from bluetooth import *
import move
import time

speed_set = 100
rad = 0.5
buf_size = 1024;

#MAC address of ESP32
addr = "40:91:51:9B:E3:AE"

def send_msg():
    print("\nSend a message to initiate motor control:\n")
    data = input()
    sock.send(bytes(data, 'UTF-8'))
    sock.send("\n")
        
def robotCtrl(command_input):
    global direction_command, turn_command
    if 'forward' == command_input:
        direction_command = 'forward'
        move.move(speed_set, 'forward', 'no', rad)
    
    elif 'backward' == command_input:
        direction_command = 'backward'
        move.move(speed_set, 'backward', 'no', rad)

    elif 'DS' in command_input:
        direction_command = 'no'
        if turn_command == 'no':
            move.move(speed_set, 'no', 'no', rad)

    elif 'left' == command_input:
        turn_command = 'left'
        move.move(speed_set, 'no', 'left', rad)

    elif 'right' == command_input:
        turn_command = 'right'
        move.move(speed_set, 'no', 'right', rad)

    elif 'TS' in command_input:
        turn_command = 'no'
        if direction_command == 'no':
            move.move(speed_set, 'no', 'no', rad)
        else:
            move.move(speed_set, direction_command, 'no', rad)

def receive_msg():
    
    direction_command = 'no'
    turn_command = 'no'

    data = sock.recv(buf_size).decode()
    print("Received ["+data+"]")
    
    if(data == 'forward' or data == 'backward' or data == 'right' or data == 'left' or data == 'TS' or data == 'DS'):
        robotCtrl(data)
    else:
        data = "did not recognize command"
        sock.send(bytes(data, 'UTF-8'))
 

#init motor control
move.setup()
move.move(speed_set, 'forward', 'no', rad)
time.sleep(0.5)
move.move(speed_set, 'backward', 'no', rad)
time.sleep(0.5)
move.move(speed_set, 'no', 'no', rad)

#connect to ESP32
service_matches = find_service( address = addr )

if len(service_matches) == 0:
    print("Unable to find the ESP32 Device")
    sys.exit(0)

print("\nService:")
print(service_matches[0])
    
port = service_matches[0]["port"]
name = service_matches[0]["name"]
host = service_matches[0]["host"]

print("Connecting to \"%s\" on host %s and port %s" % (name, host, port))

# Create the client socket
sock=BluetoothSocket(RFCOMM)
sock.connect((host, port))

print("Connected")

send_msg()

while(receive_msg):
    continue

sock.close()
move.destroy()
