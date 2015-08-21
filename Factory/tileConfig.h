#define TRUCK_MOTION 1
#define OLED_MESSAGE 2
#define ACTION_COMMAND 3;
#define MAX_STRING 60
char buffer[MAX_STRING];


int factoryNode=0;
int distributorWest=1;
int distributorEast=2;
int retailerWest=3;
int retailerEast=4;

int retailerThreshold=1;
int distributorThreshold=10;
int factoryTheshold=100;



const char str0[] PROGMEM = "Grains!Low!Alert";

//DW Notifications
const char str1[] PROGMEM = "Dispatch?";
const char str2[] PROGMEM = "Confirmed Dispatch  ";
const char str3[] PROGMEM = "Dispatching..";
const char str4[] PROGMEM = "Container Left!Factory";
const char str5[] PROGMEM = "Container!Arrived at!Distributor 1";
const char str6[] PROGMEM = "Truck Arrived  ";

const char str7[] PROGMEM = "Unloading";
const char str8[] PROGMEM = "Unloading Done"; //This is at D1, sent by D1 to Factory



//DE Notifications
const char str19[] PROGMEM = "Dispatch?";
const char str20[] PROGMEM = "Confirmed Dispatch";
const char str21[] PROGMEM = "Dispatching..";
const char str22[] PROGMEM = "Container Left Factory";
const char str23[] PROGMEM = "Container!Arrived at!Distributor 2";
const char str24[] PROGMEM = "Truck!Arrived";

const char str25[] PROGMEM = "Unloading";
const char str26[] PROGMEM = "Unloading Done"; //This is at 2, sent by D2 to Factory

//RW Notifications
const char str30[] PROGMEM = "Enough stock??";
const char str31[] PROGMEM = "Please Replenish";
const char str32[] PROGMEM = "Replenished";

//RE Notifications
const char str40[] PROGMEM = "Enough stock??";
const char str41[] PROGMEM = "Replenish";
const char str42[] PROGMEM = "Replenished";



PROGMEM const char* const tileMessages[]={str0,str1,str2,str3,str4,str5,str6,str7, str8, str19,str20,str21,str22,str23,str24,str25,str26};

struct dataPayload
{
 
 int command; //TRUCK_MOTION or OLED_MESSAGE
 int messageNumber;
 
 char* getMessageTextByIndex(int index)
 {
   //strcpy_P(buffer, (char*)pgm_read_word(&(tileMessages[this->messageNumber]))); // Necessary casts and dereferencing, just copy.
    strcpy_P(buffer, (char*)pgm_read_word(&(tileMessages[index])));
   //return getString(tileMessages[this->messageNumber]);
   return buffer;
 }
 
 
  
 
   
};

 char* getMessageTextByIndex(int index)
 {
   //strcpy_P(buffer, (char*)pgm_read_word(&(tileMessages[this->messageNumber]))); // Necessary casts and dereferencing, just copy.
    strcpy_P(buffer, (char*)pgm_read_word(&(tileMessages[index])));
   //return getString(tileMessages[this->messageNumber]);
   return buffer;
 }
 





