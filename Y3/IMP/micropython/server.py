import network
import usocket as socket
import temp
import machine


HTTP = b"""\
HTTP/1.0 200 OK
Content-Type: application/json

%s
"""

class Server:

    def __init__(self):
        self.ap = network.WLAN(network.AP_IF)
        self.ap.config(essid='My AP', channel=11)

        self.thermometer = temp.Thermometer()

        self.ap.active(True)

        addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]

        self.socket = socket.socket()
        self.socket.bind(addr)

    def __del__(self):
        self.socket.close()

    def communicate(self):
        self.socket.listen(5)

        while True:
            cl, addr = self.socket.accept()
            print('client connected from', addr)
            cl_file = cl.makefile('rwb', 0)
            while True:
                line = cl_file.readline()
                if not line or line == b'\r\n':
                    break


            response = b'{ "temperature" : %f }\n' % self.thermometer.getTemp()
            cl.send(HTTP % response)
            cl.close()

                

