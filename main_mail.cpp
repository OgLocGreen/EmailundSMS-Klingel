
#include "ESP32_MailClient.h" //Source: mobizt/ESP32 Mail Client@^2.1.6

#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex Define Wakeuppin

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "FRITZ!Box 7590 VL";
const char* password = "56616967766283031728";

// To send Email using Gmail use port 465 (SSL) and SMTP Server smtp.gmail.com
// YOU MUST ENABLE less secure app option https://myaccount.google.com/lesssecureapps?pli=1
#define emailSenderAccount    "mechatronikwelt@gmail.com"    
#define emailSenderPassword   "mechatronikHSKA2020"
#define emailRecipient        "OgLocGreen@protonmail.com"  //PLs enter here the Mail Reciver email-adress
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32 Test"


// The Email Sending data object contains config and data to send
SMTPData smtpData;

// Callback function to get the Email sending status
// Callback function to get the Email sending status
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}



/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
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

void setup(){
  Serial.begin(9600);
  delay(1000); //Take some time to open up the Serial Monitor

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  //Configure which Pin to use as wake up
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,0); //1 = High, 0 = Low


  // Logging Into the Email Account
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
  smtpData.setSender("ESP32", emailSenderAccount);

  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  // Set the subject
  smtpData.setSubject(emailSubject);

  // Set the message with HTML format
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Klingel! Jemand ist an der Haustüre!</h1><p>- Sent from ESP32 board</p></div>", true);

  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  smtpData.setSendCallback(sendCallback);

  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //Clear all data from Email object to free memory
  smtpData.empty();

  while(GPIO_NUM_33!=0)
  {
  esp_deep_sleep_start();
  }
}

// Loop ist empty cause we want it only to be once send
void loop(){

}