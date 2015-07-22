#define TRUCK_MOTION 1
#define OLED_MESSAGE 2
#define MAX_STRING 60

int factoryNode=0;
char buffer[MAX_STRING];
int distributor1Node=1;
int distributor2Node=2;

int retailer1Node=3;
int retailer2Node=4;

const char str1[] PROGMEM = "Atta Low Alert";
const char str2[] PROGMEM = "Dispatching..";
const char str3[] PROGMEM = "Container Left Factory";
const char str4[] PROGMEM = "Container Arrived at Distributor";
const char str5[] PROGMEM = "Dispatching";
const char str6[] PROGMEM = "Dispatching";


PROGMEM const char* const tileMessages[]={str1,str2,str3,str4,str5,str6};

struct dataPayload
{
 
 int command; //TRUCK_MOTION or OLED_MESSAGE
 int messageNumber;
 
 char* getMessageText()
 {
   //strcpy_P(buffer, (char*)pgm_read_word(&(tileMessages[this->messageNumber]))); // Necessary casts and dereferencing, just copy.
    strcpy_P(buffer, (char*)pgm_read_word(&(tileMessages[this->messageNumber])));
   //return getString(tileMessages[this->messageNumber]);
   return buffer;
 }
 
  
 
   
};





