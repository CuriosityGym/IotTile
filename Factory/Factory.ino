#include <SoftwareSerial.h>
#include <espduino.h>
#include <mqtt.h>
#include "U8glib.h"


#include "tileConfig.h"
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>





SoftwareSerial debugPort(5, 6); // RX, TX
ESP esp(&Serial, &debugPort, 4);
MQTT mqtt(&esp);

char* endpoints[]={"/CG/West/Alert", "/CG/East/Alert"};
int sizeOfEndpointArray=2;

int alertCounters[]={0,0};

#define OLED_CLK   A0
#define OLED_MOSI  A1
#define OLED_RESET A2
#define OLED_DC    A3
#define OLED_CS    A4
U8GLIB_SSD1306_ADAFRUIT_128X64 u8g(OLED_CLK, OLED_MOSI, OLED_CS, OLED_DC, OLED_RESET);	// SW SPI Com: SCK = A0, MOSI = 11, CS = 10, A0 = 9

boolean ESPEnable=false;
boolean wifiConnected = false;
boolean test=true;
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
  
   
    debugPort.println("Connected");
    debugPort.print("Subscribing to ");
    debugPort.print(sizeOfEndpointArray);
    debugPort.println(" topics");
    for(int i=0;i<sizeOfEndpointArray;i++)
    {
     
     mqtt.subscribe(endpoints[i]); 
      
    } 
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
  
  doAction(topic, data);
  sendActionAlerts();
  

}
void mqttPublished(void* response)
{

}



void sendActionAlerts()
{
  
  for(int i=0;i<sizeOfEndpointArray;i++)
    {
     delay(1000);//wait for a few moments before sending consecutive sends
     if(i==0) //West Alert
     {
       
        if(alertCounters[i]> factoryTheshold)
        {
            sendMessageToNode(factoryNode,TRUCK_MOTION,1); 
                     
        
        } 
        
        else if(alertCounters[i]> distributorThreshold)
        {
            sendMessageToNode(distributorWest,TRUCK_MOTION,1);        
            
        } 
     
        else if(alertCounters[i]> retailerThreshold)
        {
            sendMessageToNode(retailerWest,TRUCK_MOTION,1);        
            
        }      
      
      
     }
     
      if(i==1) //East Alert
     {
       
        if(alertCounters[i]> factoryTheshold)
        {
            sendMessageToNode(factoryNode,TRUCK_MOTION,1); 
                  
        } 
        
        else if(alertCounters[i]> distributorThreshold)
        {
            sendMessageToNode(distributorEast,TRUCK_MOTION,1); 
            
        } 
     
        else if(alertCounters[i]> retailerThreshold)
        {
            sendMessageToNode(retailerEast,TRUCK_MOTION,1);        
            
        }      
      
      
     }
     
     
     
    } 
  
}









void setup()
{
  
  
  
//*******************  MQTT SETUP **********************************
  Serial.begin(19200);
  debugPort.begin(19200);
  if(ESPEnable)
  {
    setupMQTT();
  } 
  
  
  
  //OLED
  u8g.setFont(u8g_font_unifont);  // select font
  
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
  if(test)
  {
    sendMessageToNode(distributorWest,TRUCK_MOTION,1);    
    test=false;
  } 
    
  // Pump the network regularly
   
  network.update();
  //debugPort.print(network.available());
  while ( network.available() )
  {
    debugPort.println("Yes");
    // If so, grab it and print it out
    RF24NetworkHeader header;
    network.read(header,&payload,sizeof(payload));    
    processMessages(payload);
    
  }
  if(ESPEnable)
  {
    esp.process();
  } 
  
  u8g.firstPage();
  do {
    draw();
  } while( u8g.nextPage() );

  
  
}

void draw()
{
     // u8g.drawBitmapP( 0, 14, 16, 52, CGLOGO); //Just logo
      u8g.drawStr(0, 28, "Rupin");
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



void processMessages(dataPayload lPayload)

{
     debugPort.print("Command: ");debugPort.println(lPayload.command, DEC);
     debugPort.print("Message Number: ");debugPort.println(lPayload.messageNumber, DEC);
     debugPort.print("Message Text: ");debugPort.println(lPayload.getMessageTextByIndex(lPayload.messageNumber));
     
     switch(lPayload.messageNumber)
     {
        case 0: //Atta Low Alert, not sent by NRF, sent over wifi
        // Message to Dispatch sent from NRF, Factory Ackowledges it.
        break;       

        
        case 2: //Confirmed Dispatch, response from Distributors when asked if factory can dispatch
          //Reciever :Factory
          //Case when Distributor confirms space available to stock
          sendMessageToNode(distributorWest,OLED_MESSAGE,3);
          delay(1000);
          sendMessageToNode(distributorEast,OLED_MESSAGE,3);
          
          delay(3000);
          showMessageOnLCD(lPayload.getMessageTextByIndex(3));// Dispatching..
          delay(2000);
          setTruckInMotion();// Happens till Distributor 1 confirms truck arriveds
          delay(2000);
          showMessageOnLCD(lPayload.getMessageTextByIndex(4));// Truck Left Factory
          sendMessageToNode(distributorWest,OLED_MESSAGE,4); //Notify the Distributors of Truck leaving factory
          
  
        break;
        
        
        
        
      
        case 5: //Container Arrived at Distributor 1
        //Sent by Distributor, accepted at Factory, to confirm distrbutor recieved the stock
        //sensed by IR sensor
        
          //delay(3000);
          showMessageOnLCD(lPayload.getMessageTextByIndex(5));// Container Arrived at D1
          stopTruck();
          //sendMessageToNode(distributorWest,ACTION_COMMAND,6); //Truck has halted, start unloading process
          // Action happens at Distributor 1, until it sends a message that unloaded successfully.
          
        break;
        
        
        
        case 8: // Unloading at Distributor 1 Complete
        //Sent by Distributor, accepted at Factory,
        delay(1000);
        setTruckInMotion();// Happens till Distributor 2 confirms truck arrives
        break;
        
        case 11: // Unloading at Distributor 1 Complete
        //Sent by Distributor, accepted at Factory,
        showMessageOnLCD(lPayload.getMessageTextByIndex(9));// Container Arrived at D2
        stopTruck();// Happens till Distributor 2 confirms truck arrives
        break;
        
        
        
        
      
       
       
     }
        

}

void stopTruck()
{
  
}

void setTruckInMotion()
{
  
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





boolean doAction(String location, String data)
{
    data.toLowerCase();

    if(data=="")
    {
       data="0"; 
    }  
   
   
    for(int i=0;i<sizeOfEndpointArray;i++)
    {
     //debugPort.println(light+ " : " +endpoints[i]);
      if(location==endpoints[i])
      {
         alertCounters[i]=alertCounters[i]+data.toInt();
      } 
      

      
    } 
    
     for(int i=0;i<sizeOfEndpointArray;i++)
    {
     debugPort.print("Recieved ");
     debugPort.print(alertCounters[i], DEC);     
     debugPort.print(" alerts from ");
     debugPort.println(endpoints[i]);      
    } 
      

}

void setupMQTT()
{
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
   
}
