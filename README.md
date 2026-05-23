#Rotating Ultrasonic Radar Detector

##📌 Overview 
This project uses an ESP32 to create an automated toll gate that detects objects going through, vertifies the object using an RFID card scanner, and controls the servo driven gate

##🚀 Features

Ultrasonic sensor detects an object within 10cm before entering gate, and the system registers its presence
64 x 128 SSD1306 OLED displays messages when system is ready, when to tap card, etc.
System displays "Access Granted" message and a green LED goes off, otherwise the red LED stays on or blinks if "Access Denied" is displayed
Servo moves in a 0-90 degrees rotation to open gate if "Access Granted", otherwise it remains closed

##🛠️ Hardware Used

ESP32 (NodeMCU)
HC-SR04 Ultrasonic Sensor
SG90 Micro Servo
SSD1306 OLED Display (I2C)
2 LED's
RFID Reader (MFRC522)

##🔌Wiring

*HC-SR04 Ultrasonic Sensor
VCC-->VIN
GND-->GND
TRIG-->D12
ECHO-->D13

*SG90 Micro Servo
VCC(red)-->VIN
GND(brown)-->GND
PWM (yellow)-->D14

*RFID Reader (MFRC522)
VCC-->3.3V
GND-->GND
MISO-->D19
MOSI-->D23
SCK-->D18
SDA-->D5
RST-->D27

*SSD1306 OLED Display (I2C)

VCC-->3.3V
GND-->GND
SCL-->D22
SDA-->D21


## 🔧 Challenges
Managing the high number of pins required by SPI (RFID), I2C (OLED), PWM (Servo), and digital I/O required careful mapping to avoid conflicting with the ESP32 strapping pins.

## 📝 Small Note
Make sure your RFID reader is powered strictly by the 3.3V pin on the ESP32. Connecting it to 5V (VIN) can permanently damage the MFRC522 chip.
