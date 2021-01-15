# TP MOSH Gas Sensor Application using LoRa Communication
This project is done in conjunction with the Smart Devices module. The objective of this project is to develop an application that acts as a breathalyser, detecting alcohol and send a warning to, for example, a web application. The communication from the sensor node to that web application was done through The Things Network platform via LoRa. We created a graphical interface at Node-RED to visualise the gas sensor value.

## Arduino
For the prototyping on Arduino, we did not use the gas sensor fabricated at the cleanroom but a similar one namely, MQ3. For energy efficiency purpose, we put the mcu to sleep when no alcohol is detected, and only wake up and transmit using LoRa when alcohol is detected (gas sensor value exceeds a certain seuil).

### Materials

Following is a list of the components used:
* Arduino Uno
* RN2483A LoRa module x 1
* LED x 1
* Gas sensor MQ3 x 1

![image](https://github.com/MOSH-Insa-Toulouse/2020_2021_LIEW_ZHOU/blob/main/Images/ArduinoProto/arduino.jpg)

### LoRa Communication through TTN
![image](https://github.com/MOSH-Insa-Toulouse/2020_2021_LIEW_ZHOU/blob/main/Images/ArduinoProto/TTN.PNG)

### Graphical Interface (Node-RED)
![image](https://github.com/MOSH-Insa-Toulouse/2020_2021_LIEW_ZHOU/blob/main/Images/ArduinoProto/nodeRedflow.PNG)
![image](https://github.com/MOSH-Insa-Toulouse/2020_2021_LIEW_ZHOU/blob/main/Images/ArduinoProto/interfaceNodeRed.PNG)
 

## Datasheet
Click [here](https://github.com/MOSH-Insa-Toulouse/2020_2021_LIEW_ZHOU/blob/main/Datasheet.pdf) to see the datasheet we created for the gas sensor we fabricated at the cleanroom training. 

## KiCad - Shield for Arduino

### TODO design an electronic card with Kicad
- [x] Creat Kicad porject(Gas_sensor_shield)
- [x] Design the circuit board
- [x] Verification of electrical tests **ERC**
- [x] Association of component footprints with their case
- [x] Netlist generation
- [x] Design of the electronic card
- [x] Importing the netlist
- [x] Component placement
- [x] Define the design constraints of the electronic board ([stackup](https://www.emsproto.com/fr/standard-multi-layer-pcb-stackup))
- [x] Route the tracks of the electronic board

### Schematic
![image](https://github.com/MOSH-Insa-Toulouse/2020_2021_LIEW_ZHOU/blob/main/Images/Kicad/Schematic.PNG)
### Placement and Routing
![image](https://github.com/MOSH-Insa-Toulouse/2020_2021_LIEW_ZHOU/blob/main/Images/Kicad/Placement.PNG)
![image](https://github.com/MOSH-Insa-Toulouse/2020_2021_LIEW_ZHOU/blob/main/Images/Kicad/Routing.PNG)
### 3D View
![image](https://github.com/MOSH-Insa-Toulouse/2020_2021_LIEW_ZHOU/blob/main/Images/Kicad/3D_Model.PNG)

## LTSpice
### Objective
This gas sensor gives an output of around 100 nA. This is not acceptable for normal microcontrollers used nowadays. Therefore, to obtain the output of the gas sensor, the following circuit is needed as an intermediate between the gas sensor and the ADC of  a microcontroller. This circuit will give a gain of 100 in nominal condition.

![image](https://github.com/MOSH-Insa-Toulouse/2020_2021_LIEW_ZHOU/blob/main/Images/Kicad/Schema.PNG)

## Contribution
### Students 
* Terence Chun Heng LIEW
* Zheng ZHOU
### Tutors
* Jérémie GRISOLIA
* Arnauld BIGANZOLI 
* Benjamin MESTRE
