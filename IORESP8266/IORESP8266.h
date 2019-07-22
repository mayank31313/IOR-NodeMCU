/*
 Name:		IORESP8266.h
 Created:	5/30/2019 1:10:11 PM
 Author:	Mayank
 Editor:	http://www.visualmicro.com
*/

#ifndef IORESP8266_h
#define IORESP8266_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <LinkedList.h>
#include <ESP8266WiFi.h>

#pragma once
struct SyncData {
	String key;
	String value;
};

class IOTClient
{
private:
	WiFiClient client;
	//const char* server = "www.iorcloud.ml";
	const char* server = "192.168.1.13";
	const char *token;
	int code, to;
	unsigned long time_delay;

	String readData();
	void(*onReceive)(const String&,LinkedList<SyncData*>*) = NULL;
public:
	boolean connect();
	IOTClient(WiFiClient c, int from, int to, const char* toToken) {
		this->client = c;
		this->code = from;
		this->token = toToken;
		this->to = to;
		time_delay = millis();

		this->client.setNoDelay(true);
	}
	bool sendMessage(const char* c, LinkedList<SyncData*>* s);
	boolean checkConnectivity();
	void wait4Data(boolean tunneling);
	void setOnReceive(void(*on_rec)(const String&, LinkedList<SyncData*>*));
	WiFiClient& getClient();
};

#endif

