#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>



HardwareSerial MySerial(1);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  MySerial.begin(115200, SERIAL_8N1, 16, 17);
 
}

void loop() {
  // put your main code here, to run repeatedly:
    if(MySerial.available()){
    float data = MySerial.parseFloat();
    Serial.println(data);
    delay(500);
    }

}
