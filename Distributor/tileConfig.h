#define TRUCK_MOTION 1
#define OLED_MESSAGE 2
#define ACTION_COMMAND 3;
#define MAX_STRING 60

int factoryNode=0;
char buffer[MAX_STRING];
int distributor1Node=1;
int distributor2Node=2;

int retailer1Node=3;
int retailer2Node=4;


int factoryTheshold=100;
const char str0[] PROGMEM = "Atta Low Alert";
const char str1[] PROGMEM = "Dispatch?";
const char str2[] PROGMEM = "Confirmed Dispatch";
const char str3[] PROGMEM = "Dispatching..";
const char str4[] PROGMEM = "Container Left Factory";
const char str5[] PROGMEM = "Container Arrived at Distributor 1";
const char str6[] PROGMEM = "Truck Arrived";

const char str7[] PROGMEM = "Unloading";
const char str8[] PROGMEM = "Unloading Done"; //This is at D1, sent by D1 to Factory

const char str9[] PROGMEM = "Container Arrived at Distributor 2";

const char str10[] PROGMEM = "Unloading";

const char str11[] PROGMEM = "Unloading Done";
const char str12[] PROGMEM = "Container Arrived at Distributor 2";





PROGMEM const char* const tileMessages[]={str0,str1,str2,str3,str4,str5,str6,str7, str8, str9, str10};

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

char* getMessageTextbyIndex(int index)
 {
   //strcpy_P(buffer, (char*)pgm_read_word(&(tileMessages[this->messageNumber]))); // Necessary casts and dereferencing, just copy.
    strcpy_P(buffer, (char*)pgm_read_word(&(tileMessages[index])));
   //return getString(tileMessages[this->messageNumber]);
   return buffer;
 }
 





