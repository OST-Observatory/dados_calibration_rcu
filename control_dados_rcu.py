### DADOS RCU Communication Tool v1.0 ###
# This application allows to communicate with the DADOS RCU via a serial port
####################################

# import necessary packages
from serial import Serial
from serial.tools.list_ports import comports
import time
###########################

# user intro
print(r"### DADOS RCU Communication Tool v1.0 ###")
print("Do NOT close this application as long as the DADOS RCU is in use. Exiting this application WILL cause a shut down of all calibration lamps of the calibration unit for safety reasons!")

# routine, to find the appropriate COM port
print()
print("The following COM ports are available to this computer:")
print("========================")
all_ports = comports()
for port in all_ports: # print all available com ports
    print(port)
print("========================")
print()

# the proper COM port's name is "USB-SERIAL CH340". Find the description in the list of all COM ports that have been found
proper_COM_port_name = "USB-SERIAL CH340"

print("Trying now to find the proper COM port to connect to the DADOS RCU")
comports_found = []
counters_found = []
counter = 0
for port in all_ports:
    if port.description.find(proper_COM_port_name) != -1: # if you found a comport with the proper description ...
        comports_found.append(port.name)
        counters_found.append(counter)
    counter += 1

if len(comports_found) == 0: # apparently, the DADOS RCU is not among the ist of all COM ports that have been identified
    print("No port found to which the DADOS RCU is connected to. Check for loose cables!")

elif len(comports_found) > 1:
    print("Found multiple appropriate COM ports that MIGHT have a microcontroller connected, similar to the model which is also used for the DADOS RCU. Please choose from the list of available, proper COM ports and type the resperctive COM port number (NUMBER ONLY!):")
    for i in counters_found:
        print(all_ports[i])
    COMport = "COM" + input("\t>>>   ") # if more than one COM port has been found, the action is on the user to specify the proper COM port number

else:
    COMport = comports_found[0]
    print("Since only found a single COM port has been found that seems to be suitable, we assume that this is the correct one.\nWe establish communication ...")

# open the COM port for communication
arduino = Serial(COMport, baudrate=9600, timeout=.1)
print("[SUCCESS] Connected to ", COMport, "!") # Yay! We found THE proper COM port!

exit_cases = ["exit", "Exit", "quit", "Quit"] # list of possible inputs to exit this application
print("Write the commands you want to transmit in the following lines and send them by hitting enter.\nExit this application either by writing ")

message = "" # variable to stroe the user input
while message not in exit_cases: # 
    if arduino.inWaiting(): # if there is something in the read buffer, read it ...
        print(arduino.readline().decode("ascii")) # ... and decode the array of bytes to a human-readable string
    
    message = input("Write the command to send\t>>>   ") # user needs to write the command in order to transmit something
    arduino.write((message + ";").encode("ascii")) # transmit the command to the arduino and encode the string to serial transmittable bytes
    time.sleep(0.1) # allow some time before the next loop inbteration stars. This is important to let some bytes arrive at the read buffer. Some commands trigger a response from the arduino. The arrival of these messages then is somewhat delayed because of the tramission time.

arduino.close() # close the COM port after being finished with our business ...