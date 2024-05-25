all: cp flash monitor

cp:
	arduino-cli compile --fqbn esp32:esp32:nodemcu-32s figure 

flash:
	arduino-cli upload --port /dev/ttyUSB0 --fqbn esp32:esp32:nodemcu-32s figure

ch: 
	sudo chmod a+rw /dev/ttyUSB0

monitor:
	arduino-cli monitor -p /dev/ttyUSB0 --fqbn esp32:esp32:nodemcu-32s -c baudrate=9600
