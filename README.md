# NTP Time Date on the Large 7-segments Display #


## _Introduction_ ##

This article describes a seven segment, 2.3 inch large display, that shows the time and date. Hardware is ready to show temperature, just cooding need to be modified.

 The exact time and date are obtained from the Network Time Protocol via WiFi. 
For a smaller display, about 0.8 inch schematic diagram is very simple. 
Problems are with large segments, containing more LEDs in series. In this case, the required voltage is higher than 5V. It is necessary to use some additional components and a special power supply.

The main component is an ESP32 D1 mini, a microprocessor and Maxim ICs: MAX7219 and two MAX333 components. The entire circuit is connected using the recommended connection from the Maxim Datasheet. The power supply is a DC-DC converter 5V to dual voltage +5V, GND, -5V.

The construction is simple, all electronic parts, including displays, are mounted on one PCB board.

## _Parts:_ ##


1.	**U1**    Microprocessor ESP32 D1 mini			1
2.	**U2** Seven-segments driver MAX7219			1
3.	**U3, U4** Analog switch, quad, SPDT, MAX333 (394)	2
4.	**LED1-LED6** seven-segment display 2.3”, FJ23101	6
5.	**Q1-Q6** Transistor MOSFET, N-channel, IRF540N	6
6.	**PS1** DC-DC step up converter, 12V to -5V/GND/+5V	1
7.	**SW1** Toggle switch, SPST or DPST  			1  
8.	**R1, R3-R8** Resistor 470k           			7	
9.	**R2** Resistor 220ohm 		         		1
10.	**R9** Resistor 470					1
11.	**R10** LDR Resistor 20k				1
12.	**R11** Resistor 10k					1
13.	**CON1** DC Power jack 				1
14.	**LED7** Led diode 3mm red				1
15.	**D1** Zener diode 5,6V				1
16.	**C1** Capacitor M1					1
17.	**C2** Capacitor 10uF/25V				1
18.	**C3, C4** Capacitor 1000uF/25V			2
19.	U6 optional DS18B20 Temperature sensor		1
20.	Pinhead connectors and IC sockets


## _Circuit description_ ##

Heart of the project is IC MAX7219 - **U2** . It is driver for seven segments numeric LED display and is able to energize 8 digits by time multiplex. Display segments “a” to “f”, are connected directly to outputs pins 14-17 and pins 20, 21, 23, totally 7 pins. Pin 22 for decimal point need lower voltage, to prevent points from higher brightness. Zener diode D1 reduces the voltage for decimal point. 

Individual digits are activated through common cathode, outputs DIGIT 0 – 5. Just 6 digits are used to display time in format:  **_Hours. Minute. Seconds_**. Required voltage is provided by switches **U3 and U4**. Each IC MAX394 contains four single-pole and double-throw switches, which according to input signal, switch voltage on COM pin between NO and NC pins. This way is voltage from COM pin apply to gate of transistors and transistors switches display digits ON and OFF with voltage 10V. All transistors are IRF540N, N Channel Power MOSFET with ultralow ON resistance. In ON state, resistance between Source and Drain is 0.04 ohm.


3 wire SPI serial interface connect the IC MAX7219 to microprocessor. Serial input for data is DATA **_pin 1_**, for CLK with max. 10MHz. **_Pin 13_** and LOAD **_pin 12_** loads incoming data.


ISET pin 18 allow to adjust peak segment current by connected resistor R2. Calculation is described in Datasheet.


## _Construction_ ##

The size of PCB determinates by size of complete display – 6 digits. There is enough space for PTH technology and components.

On upper side lays six displays, mounted in sockets made of pinhead connectors. There is located LDR resistor and Error LED. All other parts are mounted on back side.
On back side is placed switch such a way, that is accessible from front size. Switch was planned to be DPDT. I wanted to reduce size of switch, so finally I used smaller SPST, which is enough for safety reason.  Beside switch is power connector for plug adapter (main power220V/110V AC to 12V DC). It is a standard adapter 12V for current min. 0.5A.


Microprocessor and ICs are mounted in sockets. MCU is removable for programming. Power supply is switching PS, converter **_type DD39AJPA_**. Output voltage is adjustable. Board of PS is attached to PCB by 2 input and 3 output wires. Soldering points are designed for mentioned type of converter. On the market there are many similar types available. Output current must be about 0.5A min, more reliable are PS with 1A and higher current. 
Prototype was mounted inside aluminium frame via four screws.








