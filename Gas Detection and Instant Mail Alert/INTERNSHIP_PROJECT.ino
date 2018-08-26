int sensor=D7;
int gas_value;
int buzzerPin = D8; 
//float temp;
//int tempPin =A0;
int pos = 0;  
int i=0;
//void SendMessage();

//#include <SoftwareSerial.h>
#include <Servo.h>
#include<LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include "Gsender.h"

LiquidCrystal lcd(D6 ,D1 , D5, D4, D3, D2); //(RS-D6);(E-D1);(D4-D5);(D5-D4);(D6-D3);(D7-D2);
//SoftwareSerial mySerial(9, 13);
Servo myservo;  


#pragma region Globals
const char* ssid = "rcs";                           // WIFI network name
const char* password = "hogwarts";                       // WIFI network password
uint8_t connection_state = 0;                    // Connected to WIFI or not
uint16_t reconnect_interval = 10000;             // If not connected wait time to try again
#pragma endregion Globals

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
    static uint16_t attempt = 0;
    Serial.print("Connecting to ");
    if(nSSID) {
        WiFi.begin(nSSID, nPassword);  
        Serial.println(nSSID);
    } else {
        WiFi.begin(ssid, password);
        Serial.println(ssid);
    }

    uint8_t i = 0;
    while(WiFi.status()!= WL_CONNECTED && i++ < 50)
    {
        delay(200);
        Serial.print(".");
    }
    ++attempt;
    Serial.println("");
    if(i == 51) {
        Serial.print("Connection: TIMEOUT on attempt: ");
        Serial.println(attempt);
        if(attempt % 2 == 0)
            Serial.println("Check if access point available or SSID and Password\r\n");
        return false;
    }
    Serial.println("Connection: ESTABLISHED");
    Serial.print("Got IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}

void Awaits()
{
    uint32_t ts = millis();
    while(!connection_state)
    {
        delay(50);
        if(millis() > (ts + reconnect_interval) && !connection_state){
            connection_state = WiFiConnect();
            ts = millis();
        }
    }
}

void setup()
{
pinMode(buzzerPin, OUTPUT); 
pinMode(sensor,INPUT);
//pinMode(tempPin,INPUT);
myservo.attach(D0);
//mySerial.begin(9600);
Serial.begin(115200);
lcd.begin(16,2);
 // Serial.begin(9600);
    /*connection_state = WiFiConnect();
    if(!connection_state)  // if not connected to WIFI
        Awaits();          // constantly trying to connect

    Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
    String subject = "Subject is optional!";
    if(gsender->Subject(subject)->Send("rg73577@gmail.com", "ALERT!GAS LEAKAGE....")) {
        Serial.println("Message send.");
    } else {
        Serial.print("Error sending message: ");
        Serial.println(gsender->getError());
    }*/
delay(100);
}

void loop()
{
gas_value=digitalRead(sensor);
//Serial.println(gas_value);
delay(250);
//temp = analogRead(tempPin); 
//temp = temp * 0.48828125;
            if(gas_value==LOW)
            {
              Serial.println(gas_value);
             // Serial.print("TEMPERATURE = "); 
             // Serial.print(temp); 
             // Serial.print("*C"); 
              Serial.println();
              
              if(gas_value==LOW)
              {
                tone(buzzerPin,2000);
                for(i=0;i<4;i++)
                 {
                    lcd.setCursor(0,0);
                    lcd.print("ALERT");
                   delay(100);
                 }
                 lcd.setCursor(0,1);
                 lcd.print("GAS LEAKAGE");
                /* mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
                 delay(1000);  // Delay of 1000 milli seconds or 1 second
                 mySerial.println("AT+CMGS=\"+918618027278;\"\r"); // Replace x with mobile number
                 delay(1000);
                 mySerial.println("GAS LEAKAGE");// The SMS text you want to send*/
                 for (pos = 0; pos <= 180; pos += 1) 
                 { 
                   myservo.write(pos);             
                   delay(15);                     
                 }
                 
                 noTone(buzzerPin);
                  connection_state = WiFiConnect();
                  if(!connection_state)  // if not connected to WIFI
                  Awaits();          // constantly trying to connect

                  Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
                  String subject = "KITCHEN ALERT!";
                 if(gsender->Subject(subject)->Send("rg73577@gmail.com", "ALERT!!...GAS LEAKAGE....KNOB IS TURNED OFF....PLEASE CHECK")) {
                  Serial.println("Message send.");
                 } else {
                 Serial.print("Error sending message: ");
                 Serial.println(gsender->getError());
                 }
                 
                 delay(3000);
                 lcd.clear();
                 
               }
             /*  else if(temp>180)
               {
                 tone(buzzerPin,200);
                 delay(1000);
                 noTone(buzzerPin);
                  for(i=0;i<4;i++)
                 {
                    lcd.setCursor(0,0);
                    lcd.print("ALERT");
                   delay(100);
                 }
                 lcd.setCursor(0,1);
                 lcd.print("FIRE");
                 
               }*/
            }
            else if(gas_value==HIGH)
            {
               noTone(buzzerPin);
            }
           

}

/* void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918861631068\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("I am SMS from GSM Module");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}*/
