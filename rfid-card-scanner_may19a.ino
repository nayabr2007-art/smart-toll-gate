
//Libraries needed
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//Define variables and constants
#define trig 12
#define echo 13
#define Red_LED 25
#define Green_LED 26
#define servo 14
#define SS_PIN 5
#define RST_PIN 27
#define GATE_CLOSED 120
#define GATE_OPEN 180
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool waitingForCard = false;

byte allowedUID[4] = {0xD9, 0xC6, 0x1B, 0x07};
MFRC522 rfid(SS_PIN, RST_PIN);
long duration; 
int distance; 
Servo gateServo;
int fee=50;
int amount =100;

//HELPER FUNCTIONSSS
//Reading distance from ultrasonic sensor
float READINGdistance()
{
digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Read echo (timeout after 30 ms)
  long duration = pulseIn(echo, HIGH, 30000);
  if (duration <= 0) {
    return -1.0; // we don't want a negative time value
  }

  // Distance in cm, the division by 2 is because the distance is traveled twice, once from trig and back by echo
  float distance = (duration * 0.0343) / 2.0;
  return distance;
}

//detecting card
bool detectCard()
{
    // No card present
    if (!rfid.PICC_IsNewCardPresent())
    {
        return false;
    }

    // Couldn't read card
    if (!rfid.PICC_ReadCardSerial())
    {
        return false;
    }

    bool match = true;

    // Compare UID
    for (byte i = 0; i < 4; i++)
    {
        if (rfid.uid.uidByte[i] != allowedUID[i])
        {
            match = false;
        }
    }

    // Correct card
    if (match && amount>=fee)
    {
amount=amount-fee;
display.clearDisplay();
 display.setCursor(24,32);     
display.println("ACCESS GRANTED");
display.display();
Serial.println("Access Granted");

        rfid.PICC_HaltA();

        return true;
    
    }

    // Wrong card
    else
    {
      display.clearDisplay();
 display.setCursor(28,32);    
display.println("ACCESS DENIED");
display.display();
        Serial.println("Wrong Card");

        redlight();

        rfid.PICC_HaltA();

        return false;
    }
}

//Opening the gate
void openGate()
{
Serial.println("Gate Opening");

// Green ON, Red OFF
digitalWrite(Green_LED, HIGH);
digitalWrite(Red_LED, LOW);

// Open servo
gateServo.write(GATE_OPEN);

delay(5000); // time for car to pass

Serial.println("Gate Closing");

// Close servo
gateServo.write(GATE_CLOSED);

// Reset LEDs
digitalWrite(Green_LED, LOW);
digitalWrite(Red_LED, HIGH);

display.clearDisplay();
 display.setCursor(28,32);     // Start at top-left corner
    display.println("SYSTEM READY");
    display.display();
} 
   


//Simple red light flicker effect
void redlight()
{
        digitalWrite(Red_LED, HIGH);
        delay(200);
        digitalWrite(Red_LED, LOW);
        delay(200);
        digitalWrite(Red_LED, HIGH);


}



void setup() {
  Serial.begin(115200);
amount=100;
 SPI.begin(18, 19, 23, 5);
  rfid.PCD_Init();

  delay(50);
   pinMode(trig, OUTPUT);
   pinMode(echo, INPUT);
    pinMode(Red_LED , OUTPUT);
    pinMode(Green_LED , OUTPUT);

 gateServo.attach(servo);



    // Default System State
    gateServo.write(GATE_CLOSED);

    digitalWrite(Red_LED, HIGH);
    digitalWrite(Green_LED, LOW);

if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
 display.clearDisplay();
 display.setTextSize(1);      // Normal 1:1 pixel scale
 display.setTextColor(SSD1306_WHITE);//set colour
 display.setCursor(28,32);     // Start at top-left corner
 display.println("SYSTEM READY");
 display.display(); // Push the initial clear to the screen

    Serial.println("================================");
    Serial.println("  SYSTEM IS READY!!!!!!!!!!!");
    Serial.println("================================");

    delay(100);


}

void loop() {


distance=READINGdistance();

if (distance<0)
{
  return;
}



if (distance<=10 && waitingForCard==false)
{
  Serial.println("///TAP CARD TO ENTER////");
  waitingForCard=true;
  display.clearDisplay();
 display.setCursor(34,32);     
display.println("TAP CARD");
display.display();

 display.setCursor(0,0);     
display.println( amount);
display.display();
}

if (waitingForCard==true)
{
 if (detectCard())
 {
  openGate();

   waitingForCard = false;
 }
}

delay(200);

}

