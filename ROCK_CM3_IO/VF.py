import socket
import serial
import time

# automation thresholds
temp_threshold = 25.0 # degrees C
humid_threshold = 75.0 # %rel humidity
pump_counter = 0
irrigation_on = 3600 #1 hour
irrigation_off = 10 # 10 seconds
loop_delay = 3 # 3 second program loop delay

# remote login
HOST = ''                 # Symbolic name meaning all available interfaces
PORT = 50007              # Arbitrary non-privileged port
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((HOST, PORT))
print(socket.gethostname()) # use putty to login
print(PORT)
sock.settimeout(loop_delay)

# feather serial setup
ser_feather = serial.Serial(port = '/dev/ttyACM0', baudrate = 115200, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)  # open serial port

# arduino serial setup SERIAL_8N1
ser_arduino = serial.Serial(port = '/dev/ttyACM1', baudrate = 9600, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)  # open serial port

time.sleep(5) # Pause to allow arduino to reset
# find usb ports in /dev and use ls to search 

message = b"FHEG" #test sequence
print(ser_arduino.write(message))    # turn relay on

# program loop
while(True):
	ser_feather.write(b"A") # send any old data as update request
	feather_data = ser_feather.read_until(expected=b"\n", size=None) # \n is the default of this method anyway
	feather_split = feather_data.split(b",")

	feather_temp = float(feather_split[0])
	feather_humid = float(feather_split[1])
	feather_press = float(feather_split[2])

	print(feather_temp) # temp
	print(feather_humid) # humidity
	print(feather_press) # pressure


	# fan automation
	fanbool = False
	if(feather_temp > temp_threshold):
		fanbool = True		
		print("temperature high!")

	if(feather_humid > humid_threshold):
		fanbool = True
		print("humidity high!")

	if(fanbool):
		ser_arduino.write(b"B") # turn fans on
	else:
		ser_arduino.write(b"A") # turn fans off


	# irrigation automation	
	print(pump_counter)
	if(pump_counter > (irrigation_on + irrigation_off)):
		ser_arduino.write(b"C") # turn water pump off
		pump_counter = 0
	elif(pump_counter > irrigation_on):
		ser_arduino.write(b"D") # turn water pump on
		pump_counter += loop_delay
	
	else:
		pump_counter += loop_delay
		

	
	#time.sleep(loop_delay)

	##############################################################################
	# TCP/IP remote monitoring
	
	try:
		sock.listen(1) # timout is equal to the loop_delay
		conn, addr = sock.accept()
		with conn:
			print('Connected by', addr)
			print("maintainence mode, all opperations suspended")
			ser_arduino.write(b"A") # turn fans off
			ser_arduino.write(b"C") # turn water pump off
			while True:
				data = conn.recv(1024) #conn is a socket object, the return value is a bytes object 
				if not data: break

				# text based remote protocol
				print(data)
				remote_control = data.split(b",")
				if(remote_control[0] == b"help"):
					conn.sendall(b"Commands are... help, temp, humid, press, watering_period, watering_time\r\n")
					conn.sendall(b"Use ',' after command to overwride thresholds (optional)\r\n")

				elif(remote_control[0] == b"temp"):
					conn.sendall(bytes("Last temperature reading: " + str(feather_temp) + " degC\r\n", 'utf-8'))
					conn.sendall(bytes("Current fan temperature threshold: " + str(temp_threshold) + " degC\r\n", 'utf-8'))
					if(len(remote_control) > 1):
						temp_threshold = float(remote_control[1])						
						print("Fan temperature threshold updated to " + str(temp_threshold) + " degC")
						conn.sendall(bytes("Fan temperature threshold updated to " + str(temp_threshold) + " degC\r\n", 'utf-8'))

				elif(remote_control[0] == b"humid"):
					conn.sendall(bytes("Last humidity reading: " + str(feather_humid) + "%\r\n", 'utf-8'))
					conn.sendall(bytes("Current fan humidity threshold: " + str(humid_threshold) + " %\r\n", 'utf-8'))
					if(len(remote_control) > 1):
						humid_threshold = float(remote_control[1])						
						print("Fan humidity threshold updated to " + str(humid_threshold) + "%")
						conn.sendall(bytes("Fan humidity threshold updated to " + str(humid_threshold) + "%\r\n", 'utf-8'))

				elif(remote_control[0] == b"press"):
					conn.sendall(bytes("Last pressure reading: " + str(feather_press) + " mBar\r\n", 'utf-8'))





				elif(remote_control[0] == b"watering_period"):
					conn.sendall(bytes("Current watering period: " + str(irrigation_on) + " sec\r\n", 'utf-8'))
					if(len(remote_control) > 1):
						irrigation_on = float(remote_control[1])						
						print("Watering period updated to " + str(irrigation_on) + " sec")
						conn.sendall(bytes("Watering period updated to " + str(irrigation_on) + " sec\r\n", 'utf-8'))

				elif(remote_control[0] == b"watering_time"):
					conn.sendall(bytes("Current watering time: " + str(irrigation_off) + " sec\r\n", 'utf-8'))
					if(len(remote_control) > 1):
						irrigation_off = float(remote_control[1])						
						print("Watering time updated to " + str(irrigation_off) + " sec")
						conn.sendall(bytes("Watering time updated to " + str(irrigation_off) + " sec\r\n", 'utf-8'))
						

				
	except:
		print("no connection")



ser_feather.close()   
ser_arduino.close()  
