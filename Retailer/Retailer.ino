#include <avr/pgmspace.h>
#include "tileConfig.h"
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If using software SPI (the default case):// If using software SPI (the default case):
#define OLED_CLK   A0
#define OLED_MOSI  A1
#define OLED_RESET A2
#define OLED_DC    A3
#define OLED_CS    A4

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = retailerWest;

dataPayload payload;


void setup()
{
  
  Serial.begin(19200);
  Serial.println("Retailer West Up");
 
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
  
 
   
  
  
}

void sendMessageToNode(int node, int command, int messageNumber)
{
        payload = { command, messageNumber }; 
        RF24NetworkHeader header1(/*to node*/ node);
        network.write(header1,&payload,sizeof(payload));
  
}

void showMessageOnLCD(char * message)
{
  
}

void displayMessages(dataPayload lPayload)

{
     Serial.print("Command: ");Serial.println(lPayload.command, DEC);
     Serial.print("Message Number: ");Serial.println(lPayload.messageNumber, DEC);
     Serial.print("Message Text: ");Serial.println(lPayload.getMessageText());
     
     switch(lPayload.messageNumber)
     {
      
       
       
     }
     

}


