#include <avr/pgmspace.h>
#include "tileConfig.h"
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>





// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = distributor1Node;

dataPayload payload;


void setup()
{
  
  Serial.begin(57600);
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
  
 // If it's time to send a message, send it!
  unsigned long now = millis();
  if ( now - last_sent >= interval  )
  {
    last_sent = now;


    payload = { TRUCK_MOTION, i };
    RF24NetworkHeader header(/*to node*/ factoryNode);
    bool ok = network.write(header,&payload,sizeof(payload));
    Serial.println("Sent");
    i=i+1;
    if(i>5)
    {
      i=0;
    } 
   
  }
  
}

void displayMessages(dataPayload lPayload)
{

     Serial.println(lPayload.command, DEC);
     Serial.println(lPayload.getMessageText());     
 
}

