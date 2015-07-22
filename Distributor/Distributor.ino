#include <avr/pgmspace.h>
#include "tileConfig.h"
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
const uint16_t this_node = distributor1Node;

dataPayload payload;


void setup()
{
  
  Serial.begin(19200);
  Serial.println("Distributor 1 Up");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  
}

// How often to send 'hello world to the other unit
const unsigned long interval = 2000; //ms

// When did we last send?
unsigned long last_sent;
int i=0;
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
    displayMessages(payload);
    
  }
  
 // If it's time to send a message, send it!
  unsigned long now = millis();
  if ( now - last_sent >= interval  )
  {
    last_sent = now;


    payload = { TRUCK_MOTION, i };
    RF24NetworkHeader header(/*to node*/ factoryNode);
    bool ok = network.write(header,&payload,sizeof(payload));
    //Serial.println("Sent");
    i=i+1;
    if(i>5)
    {
      i=0;
    } 
   
  }
  
}

void displayMessages(dataPayload lPayload)

{
     Serial.print("Command: ");Serial.println(lPayload.command, DEC);
     Serial.print("Message Number: ");Serial.println(lPayload.messageNumber, DEC);
     Serial.print("Message Text: ");Serial.println(lPayload.getMessageText());
     //Serial.println(lPayload.getMessageText()); 
     //showMessageOnOLED(lPayload.getMessageText());    

}


