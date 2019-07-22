/*
 Name:		Tunneling.ino
 Created:	7/1/2019 8:35:31 PM
 Author:	Mayank
*/

//#include <ESP8266WiFi.h>
#include <Servo.h>
//#include <IORESP8266.h>

const char* ssid = "JASPER";
const char* password = "DESKTOP3131312";

struct PWMValues {
	int aileron;
	int elevator;
	int throttle;
	int rudder;
};
class DroneController {
private:
	int pins[4];
	PWMValues values;
	Servo outputs[4];
public:
	void setInputs(int aileron, int elevator, int throttle, int rudder) {
		pins[0] = aileron;
		pins[1] = elevator;
		pins[2] = throttle;
		pins[3] = rudder;

		for (int i = 0; i < 4; i++) {
			pinMode(pins[i], INPUT);
		}
	}
	void setOutputs(int aileron, int elevator, int throttle, int rudder) {
		outputs[0].attach(aileron);
		outputs[1].attach(elevator);
		outputs[2].attach(throttle);
		outputs[3].attach(rudder);
	}

	void writeMicroseconds(PWMValues out_values) {
		outputs[0].writeMicroseconds(out_values.aileron);
		outputs[1].writeMicroseconds(out_values.elevator);
		outputs[2].writeMicroseconds(out_values.throttle);
		outputs[3].writeMicroseconds(out_values.rudder);
	}
	PWMValues readPins() {
		values.aileron = pulseIn(pins[0], HIGH);
		values.elevator = pulseIn(pins[1], HIGH);
		values.throttle = pulseIn(pins[2], HIGH);
		values.rudder = pulseIn(pins[3], HIGH);

		return values;
	}
};

//WiFiClient wifi;
//IOTClient client(wifi, 7896, 1456, "826f7556-6442-4c09-9e1e-76dbb462542c");
//Servo controllers[4];

//int thrust_vars[4] = {500,500,88,500};
//const int pins[4] = { D6,D7,D8,D5};

DroneController controller;
//Servo throttleServo, elevatorServo, aileronServo,rudderServo;
//int throttle = 88, aileron = 500, elevator = 500, value = 500, rudder = 500;
//int value;
//unsigned long epoch = 0;
void setup() {
	/*
	throttleServo.attach(D1);
	elevatorServo.attach(D2);
	aileronServo.attach(D3);
	rudderServo.attach(D4);
	*/
	/*
	controllers[0].attach(D3);
	controllers[1].attach(D2);
	controllers[2].attach(D1);
	controllers[3].attach(D4);

	for (int i = 0; i < 4; i++) {
		controllers[i].writeMicroseconds(thrust_vars[i]);
	}
	*/
	Serial.begin(115200);
	delay(10);

	// We start by connecting to a WiFi network
	/*
	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	
	if (client.connect())
		Serial.println("Connected to Server");
	else
		Serial.println("Connection could not be established to server");

	pinMode(D0, INPUT);

	for (int i = 0; i < 4; i++) {
		pinMode(pins[i], INPUT);
	}

	epoch = millis();
	*/

	controller.setInputs(D1, D2, D3,D4);
	controller.setOutputs(D5, D6, D7, D8);
}
String value_;
boolean rc_controlled = false;
long start;
void loop() {
	/*
	start = millis();
	// put your main code here, to run repeatedly:
	int threshold = pulseIn(D0, HIGH,21000);
	rc_controlled = threshold > 1500;

	if (rc_controlled) {
		for (int i = 0; i < 4; i++) {
			thrust_vars[i] = pulseIn(pins[i], HIGH);
		}
	}
	else {
		client.wait4Data(true);
		wifi = client.getClient();
		while (wifi.available()) {
			value_ = wifi.readStringUntil('\n');
			value = value_.toInt();
			int index = value / 1000;
			thrust_vars[index] = (value % 1000) + 1000;
		}
	}

	for (int i = 0; i < 4; i++) {
		controllers[i].writeMicroseconds(thrust_vars[i]);
	}
	/*
	if (millis() - epoch > 1000 && wifi.connected()) {
		wifi.println(95);
		for (int i = 0; i < 4; i++) {
			wifi.println(thrust_vars[i] + 1000 * i);
		}
		wifi.flush();
		epoch = millis();
	}
	*/
	//Serial.println(millis() - start);

	PWMValues values = controller.readPins();
	Serial.print(values.aileron);
	Serial.print(" ");
	Serial.print(values.elevator);
	Serial.print(" ");
	Serial.print(values.throttle);
	Serial.print(" ");
	Serial.println(values.rudder);
	//controller.writeMicroseconds(values);
}