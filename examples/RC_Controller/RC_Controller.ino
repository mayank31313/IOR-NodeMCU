/*
 Name:		RC_Controller.ino
 Created:	5/30/2019 1:21:41 PM
 Author:	Mayank
*/

#include <Servo.h>
#include <LinkedList.h>
#include <ESP8266WiFi.h>
#include <IORESP8266.h>

#define SERVO_PIN D5
#define MOTOR_CONTROLLER D0
#define IN3 D1
#define IN4 D2

#define MIN_SERVO 1250
#define MAX_SERVO 1700

Servo steer;
WiFiClient wifi;
IOTClient client(wifi, 7896, 1456, "826f7556-6442-4c09-9e1e-76dbb462542c");

void onReceive(const String& message,LinkedList<SyncData*>* list) {	
	while (list->size() - 1) {
		SyncData* data = list->shift();
		int trigger = data->value.toInt();
		int key = data->key.charAt(0);
		if (key == 68)
		{
			if (trigger) {				
				Serial.println("Moving Backward Direction");
				digitalWrite(IN4, HIGH);
				digitalWrite(IN3, LOW);
			}
			else {
				Serial.println("Motors Idle");
				digitalWrite(IN3, LOW);
				digitalWrite(IN4, LOW);
			}
		}
		else if (key == 85) {
			if (trigger) {
				Serial.println("Moving Forward Direction");
				digitalWrite(IN3, HIGH);
				digitalWrite(IN4, LOW);
			}
			else {
				Serial.println("Motors Idle");
				digitalWrite(IN3, LOW);
				digitalWrite(IN4, LOW);
			}
		}
		else if (key == 76) {
			if (trigger) {
				Serial.println("Steering Left");
				steer.writeMicroseconds(MAX_SERVO);
			}
			else {
				steer.writeMicroseconds(1500);
			}
		}
		else if (key == 82) {
			if (trigger) {
				Serial.println("Steering Right");
				steer.writeMicroseconds(MIN_SERVO);
			}
			else {
				steer.writeMicroseconds(1500);
			}
		}
		else{
			Serial.println("Invalid Input");		
		}

		delete data;
	}
	
}

// the setup function runs once when you press reset or power the board
void setup() {
	WiFi.begin("JASPER", "DESKTOP3131312");
	Serial.begin(9600);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	client.setOnReceive(&onReceive);
	client.connect();

	pinMode(MOTOR_CONTROLLER, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);

	digitalWrite(IN3, LOW);
	digitalWrite(IN4, LOW);

	analogWrite(MOTOR_CONTROLLER, 950);

	steer.attach(SERVO_PIN, MIN_SERVO, MAX_SERVO);
	steer.writeMicroseconds(1500);
}

// the loop function runs over and over again until power down or reset
void loop() {
	client.wait4Data(true);
	wifi = client.getClient();
	while (wifi.available()) {
		Serial.print((char)wifi.read());
	}
}

