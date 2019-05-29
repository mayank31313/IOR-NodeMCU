# IOTClient-NodeMCU

IOT is taking over world, many electronics device connect together on a network and communicate to each other. I have build app that helps you to connect those microcontroller together. Below are the client details.

Currently it has been tested on: Arduino, Lego Mindstroms EV3 Brick and on a Raspberry PI 3 other tests are being done.

This is git repository for the Arduino client:
Before contuning verify that you have ArduinoJson in you arduino library.

Before starting make sure you have required libraries installed in your arduino library path

### Include Libraries
	
	#include <LinkedList.h>
	#include <IORESP8266Client.h>

### For WiFi Client
		
	#include <ESP8266WiFi.h>
	#include <SPI.h>

	WiFiClient client;
    
	//Inside a setup begin WiFi
	WiFi.begin(ssid,pass);

### Define some constants

	#define CODE \from\code
	#define TO \target\code
		
	const char* token = "\paste\subscription\key"

### Create IOTClient object

	IOTClient it_client(&client, CODE, TO, TOKEN);

### Setup Client

	it_client.connect(); //Connectes to the IOR Server, returns false if the connection is not made to server. else true 
	it_client.setOnReceive(&onReceive);

### Last but not the least

	it_client.wait4Data(); //Listens if there is a new data from server
	
##### Define On Receive Function

	void onReceive(const String& message) {
		Serial.println(message);
	}
	
##### For Sending Message with Metadata
    
For sending the message we will have 2 Cases:

##### Case 1: Send only Message
    it_client.sendMessage("your message here",NULL); //This will only send a Message to receivers ends
    
##### Case 2: Send message with metadata
          
    LinkedList>SyncData*>* list = new LinkedList<SyncData*>();//First create a LinkedList
    //Here SyncData is just like a key:value pair
    //So create a pointer object of SyncData   
    SyncData* data = new SyncData();
    data->key = "Here comes the Key";
    data->value = "Here comes the Value";
    //Add data to the linked list
    list->add(data);
    //Now send the data
    it_client.sendMessage("message",list);
    //Remember after sending the message you won't be able to fetch the data for LinkedList  
    
    
    