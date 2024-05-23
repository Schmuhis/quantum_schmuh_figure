cp:
	arduino-cli compile --fqbn esp32:esp32:nodemcu-32s wifi 
	arduino-cli upload --port /dev/ttyUSB0 --fqbn esp32:esp32:nodemcu-32s wifi

ch: 
	sudo chmod a+rw /dev/ttyUSB0


