using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

#if UNITY_EDITOR
using System.IO.Ports;
#else
using System;
using System.Threading.Tasks;
using Windows.Devices.Bluetooth.Rfcomm;
using Windows.Networking.Sockets;
using Windows.Storage.Streams;
using Windows.Devices.Bluetooth;
#endif

using System.Text;


public class ButtonBehavior : MonoBehaviour
{
    private bool taking_picture = false;
    private string inputString = "";

#if UNITY_EDITOR
    private SerialPort port;

    public void ConnectButtonPressHandler(Text status)
    {


        if (port == null)
        {
            status.text = "Connecting...";
            try
            {
                port = new SerialPort();
                port.BaudRate = 9600;
                port.PortName = "COM6";
                //port.ReadTimeout = 50;

                port.Open();

                try
                {
                    writeData("Hello from PC\n");
                }
                catch (System.Exception ex)
                {
                    status.text = "Error communicating over Bluetooth serial port";
                    Debug.Log(ex);
                    return;
                }

                status.text = "Connected";
            }
            catch (System.Exception ex)
            {
                status.text = "Error opening Bluetooth serial port";
                Debug.Log(ex);
                return;
            }
        }

    }

    public int readData()
    {
        return port.ReadByte();
    }

    public void writeData(string str)
    {
        port.Write(str);
    }

    public bool isConnected()
    {
        return port != null;
    }

#else
    Windows.Devices.Bluetooth.Rfcomm.RfcommDeviceService _service;
    Windows.Networking.Sockets.StreamSocket _socket;

    async public void ConnectButtonPressHandler(Text status)
    {
        // Create a socket and connect to the target
        _socket = new StreamSocket();
        await _socket.ConnectAsync("40:91:51:9B:E3:AC", "HokienautsRobot", SocketProtectionLevel.PlainSocket);
        status.text = "Connected";
    }

    async public Task<int> readData()
    {
        int readByte = 0;
        using (var dataReader = new Windows.Storage.Streams.DataReader(_socket.InputStream))
        {
            dataReader.UnicodeEncoding = Windows.Storage.Streams.UnicodeEncoding.Utf8;
            dataReader.ByteOrder = Windows.Storage.Streams.ByteOrder.LittleEndian;

            await dataReader.LoadAsync(1);

            readByte = dataReader.ReadInt32();
        }
        return readByte;
    }

    async public void writeData(string str)
    {
        // Create the data writer object backed by the in-memory stream.
        using (var dataWriter = new Windows.Storage.Streams.DataWriter(_socket.OutputStream))
        {
            dataWriter.UnicodeEncoding = Windows.Storage.Streams.UnicodeEncoding.Utf8;
            dataWriter.ByteOrder = Windows.Storage.Streams.ByteOrder.LittleEndian;
            dataWriter.WriteString(str);

            // Send the contents of the writer to the backing stream.
            await dataWriter.StoreAsync();

            // For the in-memory stream implementation we are using, the flushAsync call 
            // is superfluous,but other types of streams may require it.
            await dataWriter.FlushAsync();

            // In order to prolong the lifetime of the stream, detach it from the 
            // DataWriter so that it will not be closed when Dispose() is called on 
            // dataWriter. Were we to fail to detach the stream, the call to 
            // dataWriter.Dispose() would close the underlying stream, preventing 
            // its subsequent use by the DataReader below.
            dataWriter.DetachStream();
        }
    }

    public bool isConnected()
    {
        return _socket != null;
    }

#endif

#if UNITY_EDITOR
    private void Update()
#else
    async private void Update()
#endif
    {
        inputString = string.Empty;
        if (taking_picture)
        {
            //get start of jpg stream
            int count = 0;
#if UNITY_EDITOR
            int b = readData();
#else
            int b = await readData();
#endif
            while (b != 2 && count < 100)
            {
#if UNITY_EDITOR
                b = readData();
#else
                b = await readData();
#endif
                count++;
            }
            if (!(b == 2))
            {
                Debug.Log("Did not receive STX");
                return;
            }

            //get bytes until end of jpg stream
            List<byte> byteList = new List<byte>();
            int done = 0;
            while (done != 3)
            {
#if UNITY_EDITOR
                b = readData();
#else
                b = await readData();
#endif
                if (done == 0)
                {
                    if (b == 3)
                    {
                        done = 1;
                    }
                    else
                    {
                        byteList.Add((byte)b);
                    }
                }
                else if (done == 1)
                {
                    if (b == 3)
                    {
                        done = 2;
                    }
                    else
                    {
                        done = 0;
                        byteList.Add(3);
                        byteList.Add((byte)b);
                    }
                }
                else
                {
                    if (b == 4)
                    {
                        done = 3;
                    }
                    else
                    {
                        done = 0;
                        byteList.Add(3);
                        byteList.Add(3);
                        byteList.Add((byte)b);
                    }
                }
            }
            ImageBehavior.ChangePicture(byteList.ToArray());
            taking_picture = false;
        }

        if (inputString != string.Empty)
        {
            Debug.Log(inputString);
        }
    }

    public void ForwardButtonPressHandler()
    {
        if (isConnected())
        {
            writeData("F\n");
            Debug.Log("Sent Forward");
        }
    }
    public void BackwardButtonPressHandler()
    {
        if (isConnected())
        {
            writeData("B\n");
            Debug.Log("Sent Backward");
        }
    }
    public void LeftButtonPressHandler()
    {
        if (isConnected())
        {
            writeData("L\n");
            Debug.Log("Sent Left");
        }
    }
    public void RightButtonPressHandler()
    {
        if (isConnected())
        {
            writeData("R\n");
            Debug.Log("Sent Right");
        }
    }
    public void StopButtonPressHandler()
    {
        if (isConnected())
        {
            writeData("S\n");
            Debug.Log("Sent Stop");
        }
    }
    public void EStopButtonPressHandler()
    {
        if (isConnected())
        {
            writeData("Z\n");
            Debug.Log("Sent Emergency Stop");
        }
    }
    public void PictureButtonPressHandler()
    {
        if (isConnected())
        {
            writeData("P\n");
            Debug.Log("Sent Take Picture");
            taking_picture = true;
        }
    }
}
