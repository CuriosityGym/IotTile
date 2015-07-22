#include <avr/pgmspace.h>
#include "tileConfig.h"
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <SoftwareSerial.h>
#include <espduino.h>
#include <mqtt.h>




// If using software SPI (the default case):
#define OLED_CLK   3
#define OLED_MOSI  4
#define OLED_RESET 5
#define OLED_DC    6
#define OLED_CS    7

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);



// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = factoryNode;


dataPayload payload;


void setup()
{
  
  Serial.begin(57600);
  Serial.println("Factory Up");
  
  setupDisplay();
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  showMessageOnOLED("Factory UP");
  delay(1000);
}





void loop()
{
  
  // Pump the network regularly

  network.update();

  // Is there anything ready for us?
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    network.read(header,&payload,sizeof(payload));
    //payload.printStruct();
    
    displayMessages(payload);
    
  }
  
}



void displayMessages(dataPayload lPayload)

{
     Serial.print("Command: ");Serial.println(lPayload.command, DEC);
     Serial.print("Message Number: ");Serial.println(lPayload.messageNumber, DEC);
     Serial.print("Message Text: ");Serial.println(lPayload.getMessageText());
     //Serial.println(lPayload.getMessageText()); 
     showMessageOnOLED(lPayload.getMessageText());    
 
}

void setupDisplay()
{

  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();

}

void showMessageOnOLED(char* message)
{
  Serial.println(message);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(message);
  display.display();
 
}




