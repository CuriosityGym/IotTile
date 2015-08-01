
#include "U8glib.h"


#include "tileConfig.h"
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
// If using software SPI (the default case):
#define OLED_CLK   A0
#define OLED_MOSI  A1
#define OLED_RESET A2
#define OLED_DC    A3
#define OLED_CS    A4
U8GLIB_SSD1306_ADAFRUIT_128X64 u8g(OLED_CLK, OLED_MOSI, OLED_CS, OLED_DC, OLED_RESET);	// SW SPI Com: SCK = A0, MOSI = 11, CS = 10, A0 = 9



// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = distributorEast;

dataPayload payload;


void setup()
{
  
  Serial.begin(19200);
  Serial.println("Distributor East Up");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  showMessageOnLCD("DE UP");
  
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
    displayMessages(payload,header);
    
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

  
   u8g.setFont(u8g_font_unifont);
   u8g.firstPage();  
  do {    
    u8g.drawStr(0, 28, message);
    
  } while( u8g.nextPage() );
  
  
  
}

void displayMessages(dataPayload lPayload, RF24NetworkHeader header)

{
     Serial.print("Command: ");Serial.println(lPayload.command, DEC);
     Serial.print("Message Number: ");Serial.println(lPayload.messageNumber, DEC);
     Serial.print("Message Text: ");Serial.println(lPayload.getMessageTextByIndex(lPayload.messageNumber));
     Serial.print("From Node: ");Serial.println(header.from_node);
     switch(lPayload.messageNumber)
     {
       case 1:
       showMessageOnLCD(lPayload.getMessageTextByIndex(2));        
       delay(3000);
       sendMessageToNode(factoryNode,OLED_MESSAGE,2);

       break;
       
       case 4:
       delay(3000); 
       showMessageOnLCD(lPayload.getMessageTextByIndex(5));// Dispatching..
       delay(3000);       
       sendMessageToNode(factoryNode,OLED_MESSAGE,5);

       delay(3000);
       showMessageOnLCD(lPayload.getMessageTextByIndex(7));// Dispatching..
       delay(3000);
       sendMessageToNode(factoryNode,OLED_MESSAGE,8); 
        showMessageOnLCD(lPayload.getMessageTextByIndex(8));// Dispatching..       
       break;
       
       
     }
     

}


