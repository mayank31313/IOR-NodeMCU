/*
 Name:		IORESP8266.cpp
 Created:	5/30/2019 1:10:11 PM
 Author:	Mayank
 Editor:	http://www.visualmicro.com
*/

#include "IORESP8266.h"


boolean IOTClient::connect() {

	if (client.connect(server, 80)) {
		String s("POST /IOT/dashboard/socket/subscribe/");
		s += String(token);
		s += "/";
		s += String(code);
		s += "/";
		s += String(to);
		s += " HTTP/1.1";

		Serial.println("Connected to Server");
		client.println(s);
		client.print("Host: ");
		client.println(String(server));
		client.println("Connection: close");
		client.println();
		client.flush();
		Serial.println("Waiting for response...");

		int index = client.find("HTTP/1.1 201");
		if (index == 1)
			Serial.println("Authenticated Successfully");
		else {
			Serial.println("Unauthenticated Request");
			return false;
		}
		if (!client.find("\r\n\r\n")) {
			client.stop();
			Serial.println("Invalid response");
			return false;
		}
		else {
			s = client.readStringUntil('\n');
			Serial.println(s);
			//client.find("\n");
			Serial.println("Receiving Response");
		}
		client.stop();

		if (client.connect(server, 8000)) {
			Serial.println("connected");
			client.println(s);
			client.flush();
			time_delay = millis();
			sendMessage("<HEARTBEAT>", NULL);
			return true;
		}
		else {
			Serial.println("connection failed");
		}
		return false;
	}
	return false;
}

bool IOTClient::sendMessage(const char* message, LinkedList<SyncData*>* metadata) {
	if (client.connected()) {
		String mes(message);
		client.println(mes);
		if (metadata != NULL) {
			do {
				SyncData* data = metadata->shift();
				client.print(data->key);
				client.print(" ");
				client.println(data->value);
				delete data;
			} while (metadata->size() > 0);
		}
		client.println();
		client.flush();
		return true;
	}
	else
		Serial.println("Client not Connected");
	return false;
}


boolean IOTClient::checkConnectivity() {
	if (!client.connected()) {
		Serial.println();
		Serial.println("disconnecting.");
		client.stop();
		return false;
	}
	return true;
}
String IOTClient::readData() {
	if (client.available() && client.connected()) {
		String data = "";
		while (client.available() > 0) {
			data += (char)(client.read());
		}
		//Serial.println(data);
		return data;
	}
	return "";
}

void IOTClient::wait4Data(boolean tunneling) {
	if (!checkConnectivity()) {
		Serial.println("Sending reconnect request");		
		delay(1000);
		this->connect();
		return;
	}

	if (!tunneling) {
		if (millis() - time_delay > 60000) {
			Serial.println("Sending Heartbeat");
			sendMessage("<HEARTBEAT>", NULL);
			time_delay = millis();
		}
		String receivedData = readData();
		if (receivedData.length() > 0) {
			sendMessage("ack", NULL);
			int start = 0, end = receivedData.indexOf("\n");
			String data = receivedData.substring(start, end);
			LinkedList<SyncData*>* list = new LinkedList<SyncData*>();
			start = receivedData.indexOf("\n", end) + 1;
			while (start > 0) {
				SyncData* sync = new SyncData();
				int splitter = receivedData.indexOf(" ", start);
				sync->key = receivedData.substring(start, splitter);
				end = receivedData.indexOf("\n", splitter);
				sync->value = receivedData.substring(splitter, end);
				list->add(sync);
				start = receivedData.indexOf("\n", end) + 1;
			}
			if (this->onReceive == NULL)
				Serial.println("WARNING NO RECEIVE FUNCTION");
			else
				(*this->onReceive)(data, list);


			while (list->size()) {
				delete list->pop();
			}
			list->clear();
			delete list;

		}
	}
}

void IOTClient::setOnReceive(void(*on_rec)(const String&, LinkedList<SyncData*>*))
{
	this->onReceive = on_rec;
}

WiFiClient& IOTClient::getClient() {
	return client;
}

