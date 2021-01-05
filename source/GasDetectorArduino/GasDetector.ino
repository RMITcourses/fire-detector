//import neccessary libraries of Ethernet Shield
#include <SPI.h>
#include <Ethernet.h>

//This program is a gas detector system

//set ports for components
int buzzer = 4;
int led = 5;
int fan = 6;
int sensor = A0;

//MAC address of the Ethernet Shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IP address of RMIT
byte ip[] = { 10, 247, 219, 99 }; 
byte gateway[] = { 10, 247, 219, 1 };
byte subnet[]  = { 255, 255, 254, 0 };

//IP address of the server deployed on Amazon 
byte server[] = { 54, 88, 228, 147 };

// Initialize the Ethernet client library
EthernetClient client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip, gateway, subnet);
  }
  //set fan, buzzer, led as output
  pinMode(fan, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  //set sensor as input of the system
  pinMode(sensor, INPUT);
  
  delay(1000);
  //connect to server through port 8080
  client.connect(server, 8080);
  Serial.println("Connecting");
}

void loop() {
  //read analod value
  int analogSensor = analogRead(sensor);
  Serial.print("Pin A0: ");
  Serial.println(analogSensor); // print the gas value on Serial Monitor

  //set data in JSON type to send to the server
  String data = "{\"gasSensor\":";
  data += analogSensor;
  data += "}";
  Serial.println(data);  

  if (!client.connected()) //if the server is not connected, try to connect it
    {
      client.connect(server, 8080);
    }
  if(client.connected()){
      //header of the POST request
     Serial.println("Connecting to Server: ");  
     client.println("POST / HTTP/1.1");
     client.println("Host: http://ec2-54-88-228-147.compute-1.amazonaws.com/");
     client.println("Connection: close");
     client.println("Content-Type: application/json"); //send data as json 
     client.print("Content-Length: ");
     client.println(data.length());
     
     client.println();
     client.println(data); //body of the POST request
  }
  else {
     Serial.println("Cannot connect to Server");
  }
  
  // If the value > 200, turn on fan and led only
  // If the value > 350, activate the buzzer until the gas value decrease to <= 200
  if(analogSensor > 200){
    digitalWrite(fan, HIGH);
    digitalWrite(led, HIGH);
     if(analogSensor > 350) {
      digitalWrite(buzzer, HIGH);
      tone(buzzer, 1000, 200);
     }
    else {
       digitalWrite(buzzer, LOW);
       noTone(buzzer);
     }
   }
   //if the gas <= 200, turn off the fan and the LED
  else {
    digitalWrite(fan, LOW);
    digitalWrite(led, LOW);
   }
  
   
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  delay(1000);
}

