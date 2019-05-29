

/*
 Name:		IOR_NodeMCU.ino
 Created:	5/29/2019 4:22:36 PM
 Author:	Mayank
*/
#include "IORESP8266Client.h"
// the setup function runs once when you press reset or power the board

WiFiClient wifi;
IOTClient client(wifi,789,1234,"1db93bcd-15a9-48be-8a35-f7f805cee03b");

void onReceive(const String);

void setup() {
	WiFi.begin("JASPER", "DESKTOP3131312");
	Serial.begin(9600);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");

	client.connect();
	client.setOnReceive(&onReceive);
}

// the loop function runs over and over again until power down or reset
void loop() {
	client.wait4Data();
}

void onReceive(const String& message) {
	Serial.println(message);
}
