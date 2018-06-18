import time
import machine
import onewire, ds18x20

class Thermometer:
    def __init__(self,pin=5):
        # Uses pin 5 for thermometer
        self.device = machine.Pin(pin)

        # Creates OneWire object
        self.deviceOneWire = ds18x20.DS18X20(onewire.OneWire(self.device))

        # Scans for OneWire objects on the bus
        self.roms = self.deviceOneWire.scan()
    
    def getTemp(self):
        self.deviceOneWire.convert_temp()
        if(len(self.roms) > 0):
            return self.deviceOneWire.read_temp(self.roms[0])

