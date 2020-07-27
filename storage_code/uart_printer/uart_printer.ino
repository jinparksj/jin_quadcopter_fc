String packet_serial1 = "";
String packet_serial = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  while (!Serial);
  while (!Serial1);

  Serial.println("UART Printer Ready");

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available() > 0) {
    packet_serial1 = Serial1.readString();
    Serial.print("Get From Serial1 : "); Serial.println(packet_serial1);
  }
  if (Serial.available() > 0) {
    packet_serial = Serial.readString();
    Serial.print("Get From Serial : "); Serial1.println(packet_serial);
  }
  delay(200);

}
