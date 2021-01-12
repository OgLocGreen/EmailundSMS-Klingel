#include <Arduino.h>
#include <SoftwareSerial.h> //Source: plerup/EspSoftwareSerial@^6.11.2



void updateSerial();
void sendingSms(); //To Send a sms to a other Phonenummber please change in this Funktion
void print_wakeup_reason(); //Print wakeup



//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(17, 16); //SIM800L Tx & Rx is connected to ESP32 #3 & #2

bool klingel_taster = false;
#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex Define Wakeuppin


void setup()
{
  Serial.write("Staring Setup");
  //Begin serial communication with ESP32 and ESP32 IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with ESP32 and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  delay(1000);
  Serial.print('\n'); 
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  mySerial.println("ATI"); // Get the module name and revision
  updateSerial();
  mySerial.println("AT+COPS?"); //Check that you’re connected to the network, in this case BSNL
  updateSerial();
  mySerial.println("AT+COPS=?"); //Return the list of operators present in the network.
  updateSerial();
    mySerial.write(26); 

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  //Configure which Pin to use as wake up
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,0); //1 = High, 0 = Low
  sendingSms();

}



// Loop ist empty cause we want it only to be once send
void loop(){
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

void sendingSms()
{
  Serial.begin(9600);
    //Begin serial communication with Esp32 and SIM800L
    mySerial.begin(9600);

    Serial.println("Initializing..."); 
    delay(1000);
    Serial.print('\n'); 
    mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
    Serial.write("Sending SMS");
    mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
    updateSerial();
    mySerial.println("AT+CMGS=\"+4917642646298\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
    updateSerial();
    mySerial.print("Last Minute Engineers | lastminuteengineers.com"); //text content
    Serial.write("Finished!!!");
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}