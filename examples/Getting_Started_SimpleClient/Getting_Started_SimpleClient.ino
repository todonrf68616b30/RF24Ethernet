/*
 * TMRh20 2014
 * 
 * RF24Ethernet simple web client example
 *
 * RF24Ethernet uses the fine uIP stack by Adam Dunkels <adam@sics.se>
 *
 *
 * This example connects to google and downloads the index page
 */


#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <printf.h>
#include <RF24Ethernet.h>

/*** Configure the radio CE & CS pins ***/
RF24 radio(7,8);
RF24Network network(radio);
RF24EthernetClass RF24Ethernet(radio,network);


EthernetClient client;

void setup() {
  
  Serial.begin(115200);
  printf_begin();
  
  // This initializes the radio with basic settings.
  // Needs to be called at the beginning of every sketch
  Ethernet.use_device();  
  
  // Tell RF24Ethernet which channel to use. This step is not that important, but it 
  // is important to set the channel this way, not directly via the radio
  //Ethernet.setChannel(97);
  
  // This step is very important. The address of the node needs to be set both
  // on the radio and in the UIP layer
  // This is the RF24Network address and needs to be configured accordingly if
  // using more than 4 nodes with the master node. Otherwise, 01-04 can be used.
  uint16_t myRF24NetworkAddress = 01;
  Ethernet.setMac(myRF24NetworkAddress);
  
  //Optional
  radio.setPALevel(RF24_PA_HIGH);
  radio.printDetails();
  
  // Set the IP address we'll be using.  Make sure this doesn't conflict with
  // any IP addresses or subnets on your LAN or you won't be able to connect to
  // either the Arduino or your LAN...
  IPAddress myIP(10,10,2,4);
  Ethernet.begin(myIP);
  
  // If you'll be making outgoing connections from the Arduino to the rest of
  // the world, you'll need a gateway set up.
  IPAddress gwIP(10,10,2,2);
  Ethernet.set_gateway(gwIP);  
}

uint32_t counter = 0;
uint32_t reqTimer = 0;

void loop() {

//  if(millis() - testTimer > 5000){
//     testTimer = millis();
//     Serial.println(F("alive")); 
//  }
  uint8_t c=0;
  size_t size;

if(size = client.available() > 0){
    char c = client.read();
    Serial.print(c);
    if(counter > 150){ Serial.println(""); counter=0;}
    counter++;
}

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println(F("Disconnect. Waiting for disconnect timeout"));
    client.stop();
  
    // Wait for serial input to begin
    //while (!Serial.available() && !client.connected()){}
    //Serial.read();
    reqTimer = millis();
    while(millis() - reqTimer < 5000 && !client.available() ){ }    
    connect();
  
  }
  // We can do other things in the loop, but be aware that the loop will
  // briefly pause while IP data is being processed.
}

void connect(){
    Serial.println(F("connecting"));
    IPAddress goog(74,125,224,87);
    IPAddress pizza(94,199,58,243);
    if (client.connect(goog, 80)) {
      Serial.println(F("connected"));
      // Make a HTTP request:
  
      uint8_t buffr[39] = {"GET / HTTP/1.1\n"};
      //uint8_t buffr[39] = {"GET /asciiart/pizza.txt HTTP/1.1\n"}; //Length is 33 bytes
      client.write(buffr,15);
      
      memcpy(buffr,"Host: www.google.ca\n",20);
      //memcpy(buffr,"Host: fiikus.net\n",17);
      client.write(buffr,20);
      
      memcpy(buffr,"Connection: close\n",18);
      client.write(buffr,18);
      client.println();    
    
    }else{
      // if you didn't get a connection to the server:
      Serial.println(F("connection failed"));
    }
}
