//code developed by Saiiavon
//follow me on twitter profile:saiiavon


#include <LiquidCrystal.h> //import LCD library
#include <SoftwareSerial.h> //import Serial library
#include <TinyGPS++.h>        //import gps library

TinyGPSPlus gps;// create gps object
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); //for lcd pin interface
SoftwareSerial gpsSerial(8,9);//connect tx pin of gps to arduino digital pin 8 and connect rx pin of gps to arduino digital pin 9


//Pin Declaration
#define key 10    // Digital pin for vehicle key
#define mot A2    // analog pin for Engine motor
#define Switch 12 // Digital pin for activating system
#define alrm 11   // Digital pin for Buzzer
#define tlit 13    // Digital pin for Motion

//variable declaration
int count=0;
/*if you want to use accelemeter in
 place of tlit sensor uncomment the Comment With accelemetor 
 and connect accelemeter to pin A0 and A1*/
//const int senx=A0; // for Accelemetor
//const int seny=A1; //for Accelemetor
String lattitude,longitude;
String message; //String variable for storing incomming message
int con=0;      //for Counting 
int i=0;        //for counting
boolean ext=true; // boolean value for exit from loop
int j; //for gps
int k; //for gps



void setup() //initillize
{
  pinMode(Switch,INPUT);
  pinMode(key,INPUT);
  pinMode(tlit,INPUT);
  pinMode(alrm, OUTPUT);
  pinMode(mot,OUTPUT);
  digitalWrite(key, LOW); //Set low important
  digitalWrite(tlit, LOW);
  
  lcd.begin(16,2);
  Serial.begin(9600); // for GSM
  delay(1000);
  gpsSerial.begin(9600); // for GPS
  delay(1000);
  
  
}

void loop()  //Main loop
{
  int temp; // variable for storing Switch value
  while(1)
  {
    temp=digitalRead(Switch);
    if(temp==HIGH)  //condition for Activate the System
    {
      lcd.clear();
      lcd.print("***Welcome***");
      delay(1000);
      lcd.clear();
      lcd.print("Vehicle Theft");
      lcd.setCursor(0,1);
      lcd.print("Detection System");
      delay(1000);
      lcd.clear();
      lcd.print("Security system");
      lcd.setCursor(0,1);
      lcd.print("   activated   ");
      delay(2000);
      lcd.clear();
      //int valx=analogRead(senx); //variable Declaration for accelerometer sensor
      //int valy=analogRead(seny); //For Accelemetor
      
      if(digitalRead(key)==HIGH) //Condition for Vechile key
      { 
        digitalWrite(mot,HIGH);
        while(ext){   //00000000000000000000000000000000000000000000000000000000000000000000000000000000
        
        
        lcd.setCursor(0, 0);
        lcd.print("Theft Detected  ");
        delay(1000);
        digitalWrite(alrm,HIGH);
        if(digitalRead(key)!=HIGH)
        {
          digitalWrite(mot,LOW);
        }
        else if(digitalRead(key)==HIGH)
        {
          digitalWrite(mot,HIGH);
        }

        
        if (count==0)  //test whether the period has elapsed
        {
          
          lcd.setCursor(0, 1);
          lcd.print("Sending SMS.....");
          get_gsm();
          delay(100); 
          if(digitalRead(key)!=HIGH){  //opopoppopopop
            digitalWrite(mot,LOW);
          }
          else if(digitalRead(key)==HIGH)
          {
            digitalWrite(mot,HIGH);
          }
          count=2; 
        }
        else if(count!=0)
        {
          delay(200);
          lcd.setCursor(0, 0);
          lcd.print(" Timer Activated ");
          delay(1000); 
          lcd.clear();
          count=count-1;
          digitalWrite(alrm,LOW);
          if(digitalRead(key)!=HIGH){      //opopopopopopop
            digitalWrite(mot,LOW);
          }
          else if(digitalRead(key)==HIGH)
          {
            digitalWrite(mot,HIGH);
          }
          delay(500);  
        }
        
        delay(500);
        Serial.println("AT+CNMI=2,2,0,0,0"); // command for receiving SMS
        lcd.setCursor(0,0);
        lcd.print("Waiting for SMS"); 
        delay(100);
        sms();                              //Function for receiving
        delay(100);  
        if(digitalRead(Switch)!=HIGH){
          ext=false;}       
      }
      } 
      //000000000000000000000000000000000000000000000000000000
      /* If you are using accelemeter uncomment first else if 
       *  and comment the second else if*/
      //else if((digitalRead(key)!=HIGH)&&(((valx>265 &&valx<280 )&&(valy>310 &&valy<340)) || ((valx>340 &&valx<395 )&&(valy>310 &&valy<350))))
                                           //condition for checking movement of sensor
      else if((digitalRead(key)!=HIGH)&& (digitalRead(tlit)==HIGH))
      {                                       //condition for checking movement of sensor
        lcd.setCursor(0, 0);
        lcd.print("Theft Detected  ");
        delay(1000);
        digitalWrite(alrm,HIGH);
        lcd.setCursor(0, 1);
        lcd.print("Sending SMS.....");
        momFunction();                       // Function for sending movement theft detected message  
      }
      else{
        digitalWrite(alrm,LOW); 
      }
      
    }                                            //end of temp loop
    else{
      if(digitalRead(key)==HIGH)
      {
        digitalWrite(mot,HIGH);
      }
      else{
        digitalWrite(mot,LOW);
      }
      lcd.setCursor(0, 0);
      lcd.print("Security System ");
      lcd.setCursor(0, 1);
      lcd.print("   Deactivated    ");
      
    }
  }
}
  
    
    

//---------------------------------------------------------------------------


void get_gsm() //function for reading GPS data and sending theft location through SMS
{
  for(k=0; k<50;k++){
    for(j=0; j<60;j++){
      gps.encode(gpsSerial.read());
      lattitude = String(gps.location.lat(),6);
      longitude = String(gps.location.lng(),6);
    }
    delay(10);
  }


  
  Serial.println("AT+CMGD=1");    //Delete privious sent SMS
  delay(1000);
  Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1 second
  Serial.println("AT+CMGS=\"+91809xxxxxxx\"\r"); // Replace x with mobile number
  delay(1000);
  Serial.print("Theft Detected: ");
  Serial.print("http://maps.google.com/maps?q=loc:");
  Serial.print(lattitude);
  Serial.print(",");
  Serial.print(longitude);
  delay(200);
  Serial.print("  ");
  
  Serial.print("If you want to stop the vehicle ");
  Serial.print("Send SMS STOP");
  delay(100);
  Serial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
  delay(3000);
  lcd.clear();
  lcd.print("Message Sent");
  digitalWrite(alrm,LOW);
  delay(3000);
  lcd.clear();
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
  delay(50);
   
}


//-----------------------------------------------------------
//For stoping vechile
void sms() {
  while(i<=25){
    if(Serial.available()>0)
    {
      message = Serial.readString();
      delay(10);
    }
    if(message.indexOf("STOP") > -1)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("**SMS received**");
      delay(10);
      Serial.println("AT+CMGD=1,4"); //command for deleting all messages
      delay(10);
      while(ext){
        digitalWrite(key, LOW);
        digitalWrite(mot, LOW);
        if(con==0){
          delay(10);
          upsms();           // function
          con=30;
        }
        else{
          lcd.setCursor(0,0);
          lcd.print("Updating GPS....");
          con=con-1;
          delay(10);
          
        }
        delay(500);
        if(digitalRead(Switch)!=HIGH){
          ext=false;
        }
      }
      
    }
    i++;
    delay(1000);
    if(digitalRead(key)!=HIGH)
    {  //opopoopopopopo
      digitalWrite(mot,LOW);
    }
    else if(digitalRead(key)==HIGH)
    {
      digitalWrite(mot,HIGH);
    }
  }
  i=0;
}

    
    
    
//=====================================================

void upsms(){   //Function for sending position of theft after turn off the engine

  for(k=0; k<50;k++){
    for(j=0; j<60;j++){
      gps.encode(gpsSerial.read());
      lattitude = String(gps.location.lat(),6);
      longitude = String(gps.location.lng(),6);
    }
    delay(10);
  }
  
  
  Serial.println("AT+CMGD=1");    //Delete privious sent SMS
  delay(1000);
  Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1 second
  Serial.println("AT+CMGS=\"+91809xxxxxxxx\"\r"); // Replace x with mobile number
  delay(1000);
  Serial.println("Engine Stopped ");
  Serial.print("Present location is ");
  Serial.print("http://maps.google.com/maps?q=loc:");
  Serial.print(lattitude);
  Serial.print(",");
  Serial.print(longitude);
  Serial.println(" hurry up!");
  delay(200);
  Serial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
  delay(3000);
  lcd.clear();
  lcd.print("Message Sent");
  delay(1000);
  lcd.clear();
  lcd.print("System Ready");
  delay(1000);
  lcd.clear();
  delay(500);
  
}



//============================================================

void momFunction()  //Function :- message data for moment alert
{

  for(k=0; k<50;k++){
    for(j=0; j<60;j++){
      gps.encode(gpsSerial.read());
      lattitude = String(gps.location.lat(),6);
      longitude = String(gps.location.lng(),6);
    }
    delay(10);
  }
  
  
  
  Serial.println("AT+CMGD=1");    //Delete privious sent SMS
  delay(1000);
  Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1 second
  Serial.println("AT+CMGS=\"+91809xxxxxxxx\"\r"); // Replace x with mobile number
  delay(1000);
  Serial.println("Vehicle moving theft detected ");
  Serial.print("location is ");
  Serial.print("http://maps.google.com/maps?q=loc:");
  Serial.print(lattitude);
  Serial.print(",");
  Serial.print(longitude);
  Serial.println(" check your vehicle!");
  delay(200);
  Serial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
  delay(3000);
  lcd.clear();
  lcd.print("Message Sent");
  delay(1000);
  lcd.clear();
  lcd.print("System Ready");
  delay(1000);
  lcd.clear();
  delay(500);
  digitalWrite(alrm,LOW);
}
