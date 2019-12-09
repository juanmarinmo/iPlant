
#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <HardwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

HardwareSerial MySerial(1);

const int HPIN = 36;
#define DHTPIN 27  

//QueueHandle_t queue; //Se declara la cola
 
const char* ssid = "FibraETB915E";            //ESP32 y Raspberry conectadas a la misma red
const char* password = "FE36C3EF";

DHT dht(DHTPIN, DHT11); //Defining DHT11

#define HOST        "10.42.0.9"
#define PORT        1883
#define USERNAME    "username"
#define PASSWORD    "wilmerunal2016"

const int timeout = 200;
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client,HOST,PORT,USERNAME,PASSWORD);
Adafruit_MQTT_Publish temperatura = Adafruit_MQTT_Publish(&mqtt, "temperatura");
Adafruit_MQTT_Publish co2 = Adafruit_MQTT_Publish(&mqtt, "co2");
Adafruit_MQTT_Publish humedad = Adafruit_MQTT_Publish(&mqtt, "humedad");
void MQTT_connect();

void setup() {
 
  Serial.begin(115200);

  MySerial.begin(115200, SERIAL_8N1, 16, 17);
  pinMode (5, OUTPUT);
 
  delay(1000);

     //queue = xQueueCreate(4, sizeof( float ) );
   

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");

    MQTT_connect();

   
 
  xTaskCreate(
                    fpgasender,          /* Task function. */
                    "TaskOne",        /* String with name of task. */
                    10000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */

  xTaskCreate(
                    senderwifi,          /* Task function. */
                    "TaskTwo",        /* String with name of task. */
                    10000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */

}
 
void loop() {
 
}
 
void fpgasender( void * parameter)
{
 
    for( ;; ){
 
  float u2 = 3.3-(3.3*analogRead(HPIN)/4095) ; //Obtaining the voltage input sent by the sensor
  float hum = (u2/3.3)*100; //Parametrization 
 
  if(hum<10){ 
  
   digitalWrite(5, HIGH);
   
  }if(hum>10){
   
   digitalWrite(5, LOW);
    
  }  

  //xQueueSend(queue, &hum, portMAX_DELAY);
  vTaskDelay(2000/portTICK_PERIOD_MS);
  }
}
    


void senderwifi( void * parameter)
{
 
    for( ;; ){

    dht.begin();
    
    float dat = MySerial.parseFloat();    
    String dataP = (String)dat;
   
    float u2 = 3.3-(3.3*analogRead(HPIN)/4095) ; //Obtaining the voltage input sent by the sensor
    float hum = (u2/3.3)*100; //Parametrization
  
    String dataH = (String)hum;

    float t = dht.readTemperature();
    String dataT = (String)t;
    


  // Compute heat index in Fahrenheit (the default)
  // Compute heat index in Celsius (isFahreheit = false)
 

  temperatura.publish(t);
  delay(100);
  humedad.publish(hum);
  delay(100);
  co2.publish(dat);
  delay(100);
  
  
  Serial.print(F("Humedad: "));
  Serial.print(dataH);
  Serial.print(F("%  Temperatura: "));
  Serial.print(dataT);
  Serial.print(F("°C "));
  Serial.print(F("% CO2: "));
  Serial.print(dataP);
  Serial.print(F("% ppm"));

  
   
   vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    }
    
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 10;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 1 second...");
       mqtt.disconnect();
       delay(2000);
       retries--;
       if (retries == 0)
         Serial.println("No Conectado");
  }
  Serial.println("MQTT Connected!");
}
 
