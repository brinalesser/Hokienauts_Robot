# BtEchoServer.py

from btpycom import *

def onStateChanged(state, msg):
    if state == "LISTENING":
        print("Server is listening")
    elif state == "CONNECTED":
        print("Connection established to", msg)
    elif state == "MESSAGE":
        print("Got message", msg)
        server.sendMessage(msg)

serviceName = "EchoServer"
server = BTServer(serviceName, stateChanged = onStateChanged)
