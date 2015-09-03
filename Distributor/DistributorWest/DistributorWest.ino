
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

#define M1A        A5
#define M1B        A6
#define IRSensor   5

U8GLIB_SSD1306_ADAFRUIT_128X64 u8g(OLED_CLK, OLED_MOSI, OLED_CS, OLED_DC, OLED_RESET);	// SW SPI Com: SCK = A0, MOSI = 11, CS = 10, A0 = 9



// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = distributorWest;

dataPayload payload;
boolean truckArrivedOnce=false;
boolean truckInTransit=false;
void setup()
{
  
  Serial.begin(19200);
  Serial.println("Distributor West Up");
  u8g.setFont(u8g_font_courB10);  // select font
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(IRSensor, INPUT);
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  showMessageOnLCD("Distributor(W)!Up");
 // Serial.println("Message Shown");
  
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
  
   if(isTruckDetected() && truckInTransit)
  {
      //Serial.println("SensorRead");
      if(!truckArrivedOnce)
      {
        showMessageOnLCD(getMessageTextByIndex(5));
        delay(1000);
        sendMessageToNode(factoryNode,OLED_MESSAGE,5);

        truckArrivedOnce=true;
         delay(3000);
        showMessageOnLCD(getMessageTextByIndex(7));
       delay(3000);
       
       sendMessageToNode(factoryNode,OLED_MESSAGE,8); 
       showMessageOnLCD(getMessageTextByIndex(8));
       delay(3000);
       

      } 
   
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

 
  //Serial.println(message);
  int messageIndex=0;  
  char* command = strtok(message, "!");
  //Serial.println(command);
  int startYIndex=20;
  int fontGap=10;;
  int yIndex=0;
  
  char * lmessages[4];
  

  while (command != NULL)
  {
    

    yIndex=startYIndex+messageIndex*fontGap;
    //
    lmessages[messageIndex]=command;
    command = strtok (NULL, "!");
    messageIndex=messageIndex+1;
    //delay(5000);
  }
  
  drawText(lmessages, messageIndex);

  
  
  
}

void drawText(char *messages[], int index)
{
  

   u8g.firstPage();  
  do {  
    for(int i=0;i<index;i++)
    {  
      u8g.drawStr(0, (i+1)*14, messages[i]);
    } 
    
  } while( u8g.nextPage() );
  
  
  
}


void displayMessages(dataPayload lPayload)

{
     Serial.print("Command: ");Serial.println(lPayload.command, DEC);
     Serial.print("Message Number: ");Serial.println(lPayload.messageNumber, DEC);
     Serial.print("Message Text: ");Serial.println(lPayload.getMessageTextByIndex(lPayload.messageNumber));
     
     switch(lPayload.messageNumber)
     {
       case 1:
       
       sendMessageToNode(factoryNode,OLED_MESSAGE,2);
       showMessageOnLCD(lPayload.getMessageTextByIndex(2));
       break;
       
       case 4:
       truckInTransit=true;
       delay(1000);
       showMessageOnLCD(lPayload.getMessageTextByIndex(9));//truck in transit       
       
       
     }
     

}

boolean isTruckDetected()
{
   // Serial.println(digitalRead(IRSensor));
    if(digitalRead(IRSensor)==LOW)
    {
      
      return false;
    }
    
    else return true;

}


