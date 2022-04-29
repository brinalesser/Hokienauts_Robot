using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

#if UNITY_EDITOR
using System.IO.Ports;
#elif NETFX_CORE
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
    // Start is called before the first frame update

    Text status;
    bool taking_picture = false;
    private string inputString;

#if UNITY_EDITOR
    private SerialPort port;
#elif NETFX_CORE
    Windows.Devices.Bluetooth.Rfcomm.RfcommDeviceService _service;
    Windows.Networking.Sockets.StreamSocket _socket;
#endif

    byte[] buf;

    
    public void ConnectButtonPressHandler(Text status)
    {

#if UNITY_EDITOR
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

                buf = new byte[1024];

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

#elif NETFX_CORE
    // Enumerate devices with the object push service
    var services =
        await Windows.Devices.Enumeration.DeviceInformation.FindAllAsync(
            RfcommDeviceService.GetDeviceSelector(
                RfcommServiceId.ObexObjectPush));

    if (services.Count > 0)
    {
        // Initialize the target Bluetooth BR device
        var service = await RfcommDeviceService.FromIdAsync(services[0].Id);

        bool isCompatibleVersion = await IsCompatibleVersionAsync(service);

        // Check that the service meets this App's minimum requirement
        if (isCompatibleVersion)
        {
            _service = service;

            // Create a socket and connect to the target
            _socket = new StreamSocket();
            await _socket.ConnectAsync(
                _service.ConnectionHostName,
                _service.ConnectionServiceName,
                SocketProtectionLevel
                    .PlainSocket);
        }
    }
#endif
    }

#if NETFX_CORE
    // This App relies on CRC32 checking available in version 2.0 of the service.
    const uint SERVICE_VERSION_ATTRIBUTE_ID = 0x0300;
    const byte SERVICE_VERSION_ATTRIBUTE_TYPE = 0x0A;   // UINT32
    const uint MINIMUM_SERVICE_VERSION = 200;
    async Task<bool> IsCompatibleVersionAsync(RfcommDeviceService service)
    {
        var attributes = await service.GetSdpRawAttributesAsync(
            BluetoothCacheMode.Uncached);
        var attribute = attributes[SERVICE_VERSION_ATTRIBUTE_ID];
        var reader = DataReader.FromBuffer(attribute);

        // The first byte contains the attribute's type
        byte attributeType = reader.ReadByte();
        if (attributeType == SERVICE_VERSION_ATTRIBUTE_TYPE)
        {
            // The remainder is the data
            uint version = reader.ReadUInt32();
            return version >= MINIMUM_SERVICE_VERSION;
        }
        else return false;
    }
#endif

    private void Update()
    {
        inputString = string.Empty;
#if UNITY_EDITOR
        if (taking_picture)
        {
            //get start of jpg stream
            int count = 0;
            int b = readData();
            while (b != 2 && count < 100)
            {
                b = readData();
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
                b = readData();
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

#endif
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

    public int readData()
    {
#if UNITY_EDITOR
        return port.ReadByte();
#elif NETFX_CORE
        var ibuf = new Windows.Storage.Streams.Buffer(1);
        _socket.InputStream.ReadAsync(ibuf, 1, InputStreamOptions.None);
        return 0;
#endif

    }

    public void writeData(string str)
    {
#if UNITY_EDITOR
        port.Write(str);
#elif NETFX_CORE
        _socket.OutputStream.WriteAsync(str);
#endif
    }

    public bool isConnected()
    {
#if UNITY_EDITOR
        return port != null;
#elif NETFX_CORE
        return true;
#endif
    }
}
