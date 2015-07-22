#include <SoftwareSerial.h>
#include <espduino.h>
#include <mqtt.h>


#include <avr/pgmspace.h>
#include "tileConfig.h"
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

SoftwareSerial debugPort(5, 6); // RX, TX
ESP esp(&Serial, &debugPort, 4);
MQTT mqtt(&esp);



// If using software SPI (the default case):
#define OLED_CLK   A0
#define OLED_MOSI  A1
#define OLED_RESET A2
#define OLED_DC    A3
#define OLED_CS    A4

//Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


boolean wifiConnected = false;

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = factoryNode;


dataPayload payload;


// Events for ESP and MQTT

void wifiCb(void* response)
{
  uint32_t status;
  RESPONSE res(response);

  if(res.getArgc() == 1) {
    res.popArgs((uint8_t*)&status, 4);
    if(status == STATION_GOT_IP) {
      debugPort.println("WIFI CONNECTED");
      mqtt.connect("iot.eclipse.org", 1883);
      wifiConnected = true;
      //or mqtt.connect("host", 1883); /*without security ssl*/
    } else {
      wifiConnected = false;
      mqtt.disconnect();
    }
    
  }
}

void mqttConnected(void* response)
{
  
    char* endpoints[]={"/CG/West/Alert", "/CG/East/Alert"};
    int sizeOfEndpointArray=2;
    debugPort.println("Connected");
    debugPort.print("Subscribing to ");
    debugPort.print(sizeOfEndpointArray);
    debugPort.println(" topics");
    for(int i=0;i<sizeOfEndpointArray;i++)
    {
     
     mqtt.subscribe(endpoints[i]); 
      
    } 
   // mqtt.subscribe(endpoints[0]);  
   // mqtt.subscribe(endpoints[1]);  
   // mqtt.publish(endpoints[0],0); 
   
    debugPort.print("Subscribed to ");
    debugPort.print(sizeOfEndpointArray);
    debugPort.println(" topics");
  

  
    
    

}

void mqttDisconnected(void* response)
{

}


void mqttData(void* response)
{
  RESPONSE res(response);

  debugPort.print("Received: topic=");
  String topic = res.popString();
  debugPort.println(topic);

  debugPort.print("data=");
  String data = res.popString();
  debugPort.println(data);
  
  if(topic=="/CG/West/Alert")
  {
    payload = { TRUCK_MOTION, 1 };
  }
  else  
  {
    payload = { TRUCK_MOTION, 2 };
  }  
  RF24NetworkHeader header(/*to node*/ distributor1Node);
  bool ok = network.write(header,&payload,sizeof(payload));
  
  //doAction(topic, data);
  

}
void mqttPublished(void* response)
{

}









void setup()
{
  
  
  
//*******************  MQTT SETUP **********************************
  Serial.begin(19200);
  debugPort.begin(19200);

  debugPort.println("Bringing up ESP!"); 
  esp.enable();
  delay(500);
  debugPort.println("Resetting up ESP!"); 
  esp.reset();
  delay(500);
  while(!esp.ready())
  {
   debugPort.println("ESP Not Ready!"); 
  } 

  debugPort.println("ARDUINO: setup mqtt client");
  
  if(!mqtt.begin("Arduino","uname","pwd", 120, 1)) {
    debugPort.println("ARDUINO: fail to setup mqtt");
    while(1);
  }


  debugPort.println("ARDUINO: setup mqtt lwt");
  mqtt.lwt("/lwt", "offline", 0, 0); //or mqtt.lwt("/lwt", "offline");

/*setup mqtt events */
  mqtt.connectedCb.attach(&mqttConnected);
  mqtt.disconnectedCb.attach(&mqttDisconnected);
  mqtt.publishedCb.attach(&mqttPublished);
  mqtt.dataCb.attach(&mqttData);

  /*setup wifi*/
  debugPort.println("ARDUINO: setup wifi");
  esp.wifiCb.attach(&wifiCb);
  esp.wifiConnect("OpenWrt-Bridge","#3Twinkle3#");
  debugPort.println("ARDUINO: system started");
  
  //*********************************************************************************************************
  
  
  
  //OLED
  
  //setupDisplay();
  
  //NRF Objects
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  //showMessageOnOLED("Factory UP");
  delay(1000);
}





void loop()
{
  
  // Pump the network regularly
  esp.process();

  if(wifiConnected) {
    
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
  
}



void displayMessages(dataPayload lPayload)

{
     debugPort.print("Command: ");debugPort.println(lPayload.command, DEC);
     debugPort.print("Message Number: ");debugPort.println(lPayload.messageNumber, DEC);
     debugPort.print("Message Text: ");debugPort.println(lPayload.getMessageText());
     //Serial.println(lPayload.getMessageText()); 
     //showMessageOnOLED(lPayload.getMessageText());    

}

void setupDisplay()
{
/*
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
*/
}

void showMessageOnOLED(char* message)
{
  /*
  Serial.println(message);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(message);
  display.display();
  */
 
}



