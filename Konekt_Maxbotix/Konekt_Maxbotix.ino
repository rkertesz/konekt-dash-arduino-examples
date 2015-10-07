/*
* Konekt MaxBotix Logger 
* Author: Ruben Kertesz <rubenk@gmail.com>
* Copyright Ruben Kertesz, 2015
* This software is licensed under LGPL, v3
*  
* Utilizes components from RFduinoMaxBotixLogger, included in child directory under LGPL references.
* Author: Ruben Kertesz <rubenk@gmail.com>
* Released under LGPL, v3
* 
* Utilizes components from Konekt Dash Hello World, included under MIT.
* Author: Patrick F. Wilbur <hello@konekt.io> <pdub@pdub.net>
* Released under the MIT License (MIT)
*
*
*/


void setup() {
  Serial2.begin(9600); /* Local Serial */
  // I would like to use a different serial but could not get it to work yet. Compile error.
  SerialCloud.begin(115200); /* Konekt Cloud */
  SerialCloud.println("Hello, World!"); /* one-time message */
  delay(30);

  String dataString = "";
  dataString += "Time,";
  dataString += ",";  
  dataString += "Dist(mm)"; 
  SerialCloud.println(dataString);
  // Output headers to cloud... this is sent separately but would be better to put character return '\r'?? as part of larger block of text
}

void loop() {
    // turn on ultrasonic sensor
  digitalWrite(1, HIGH); // not currently wired to this but the intent is to use this to charge a capacitor to operate the ultrasonic momentarily.
  // we do not want the ultrasonic ranging all the time. Not sure if the pin has enough current to power but it should (RFduino does). 
  // Alternatively, this pin can be used to run a PNP or high side switch (safer than direct power off of the pin).
  delay(100); //  milliseconds allows for ranging to start
  
  String dataString = "";
//  dataString += "Time,";
  dataString += millis(); // this should actually be sample time using RTC that is cal'd to internet time
  // Need help figuring out witch RTC to use, etc.
  // Also would be smart to add a running counter to perdiodically recalibrate RTC with the network periodically.
  // Simple method to periodically recal RTC time is to do it every time we upload to the web but
  // at the moment, we are uploading every 30 seconds. In future, we will sample every minute, store locally, and
  // upload every half-hour.

  dataString += ",";
  
//  dataString += "Dist(mm),";

  // read the distance sensor and append to the string:
  String Dist = EZread();
  if (Dist != NULL){
    digitalWrite(1, LOW); //turn ultrasonic sensor off
    dataString += (Dist); 
    SerialCloud.println(dataString);
  }
else
    SerialCloud.println("9999"); // not a proper response. Maximum ranging is 5000 mm on Maxbotix 7389

  delay(30000); // wait 30 seconds ///This should actually be a sleep mode to save power
}

String EZread() {
  String result;
  int index = 0;
  boolean startreading = false;
//  char in[4]={0};// = {0,0,0,0,0,0,0};
    Serial2.flush(); // gets rid of any bits that haven't been flushed

  //while(index < 4){
    if(Serial2.available()){
     while(Serial2.available()){
      byte incoming = Serial2.read();            
      if(incoming == 'R') {
        startreading = true; 
      } 
      else if(startreading != false) { // we got rid of the R previously
         if (incoming != '\r' && incoming != '\n'){
           result += char(incoming);
         }
         else{
           break;
         }  
      }
      delay(5); // keep from reading faster than sensor is transmitting
     }
    }
 //   result = in;
  return result;       
}

