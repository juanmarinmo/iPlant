![iPlant logo](https://raw.githubusercontent.com/juanmarinmo/iPlant/master/assets/proyecto.png)


# iPlant

Development of an IoT solution to keep track of the plants needs and to discover their contribution in the improvement of air quality in one's home.

## Required materials

1. ESP32 Programmable board (Microcontroller/Master)
2. Raspberry PI (Server)
3. FPGA (Slave)
4. Sensor array
	- DHT11 (Temperature)
	- YL100 (Humidity)
	- MQ135 (CO2)
5. Arduino UNO (Slave)
6. Level shifter circuit (ESP32 - Arduino UNO serial connection)

## Main To-Do list

- [x] Study the problem
- [x] Propose different solutions 
- [x] Evaluate alternatives
- [x] Select the most viable one
- [x] Run DOFA analysis
- [x] Determine each iPlant core member's strengths
- [x] According to the results of the previous task, assign each member a task.

## iPlant implementation main branches

- Server and database
- Microcontroller 
- FPGA 
- Mobile app 

## General map

![iPlant map](https://raw.githubusercontent.com/juanmarinmo/iPlant/master/assets/map.jpg)

To check the connections between each module please read "wiring.txt".

## Server and database

Core members:

- Juan Sebastián Monje
- Wilmar Quiroga

Still working...

## Microcontroller 

Core members: 

- Miller Cubillos
- Juan Manuel Marin 

### Specific To-Do list

- [x] Set up workspaces (Arduino IDE and Platform IO)
- [x] Take measures from sensors YL100 and DHT11 using ESP32
- [x] Implement FreeRTOS in ESP32 
- [x] Calibrate MQ135 sensor 
- [x] Take measures from MQ135 sensor using Arduino UNO
- [x] Design level shifter circuit for serial communication (Arduino UNO - ESP32)
- [x] Implement MQTT broker in ESP32
- [x] Write a HIGH output in one of the ESP32 GPIO pins connected to FPGA for a threshold humidity reading
- [x] Design level shifter PCB layout


**MQ135 calibration** 

The sensor was connected to a 5V source for 24 hours, and afterwards CO2 readings were taken for 1 hour (1 second per reading) and registered in "mq135_log.txt".


Once registered the readings, they were therefore averaged so as to continue with the performance of a proper characterization to translate voltage into CO2 PPM readings. 


The full procedure is available on "mq135_correlation.txt".


**MQ135 readings**

They were performed on Arduino UNO. Although the remaining sensors were planned to be read using this board as well, the MQ135 was the only one that truly demands a 5V board to properly work. The code is available in MQ135 folder. (PlatformIO IDE was used to make this code). 


**Level shifter design**

![Level shifter circuit](https://raw.githubusercontent.com/juanmarinmo/iPlant/master/assets/circ_levsh-1.png)


**MQTT broker and remaining readings using FreeRTOS**

The main code which not only takes readings from sensors YL100 and DHT11 but sends the data collected to the database using a MQTT broker is available in the folder Master_MQTT. (Arduino IDE was used to make this code). 

Additionally, the implementation of the activation of a HIGH state in one of the GPIO pins of the ESP32 was also performed on this file. 


**Level Shifter PCB layout**

![Level shifter PCB](https://raw.githubusercontent.com/juanmarinmo/iPlant/master/assets/level_shifter-1.png)







## FPGA 

Core members:

- Maria Fernanda Triviño
- Gilbert Andrés Angulo

Still working...

## Mobile app

Developer:

- Juan Sebastián Quecán

Still working...








