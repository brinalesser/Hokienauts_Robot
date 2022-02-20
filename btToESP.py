from bluetooth import *

def send_msg():
    print("\nInput message to send:\n")
    data = input()
    sock.send(data)
        
def receive_msg():
    data = sock.recv(buf_size)
    if data:
        print("Received: " + data)
            
#MAC address of ESP32
addr = "40:91:51:9B:E3:AE"
service_matches = find_service( address = addr )

buf_size = 1024;

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
receive_msg()

sock.close()
