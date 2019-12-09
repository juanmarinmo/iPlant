
#include <WiFi.h>
#include <HTTPClient.h> 
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
    
    float data = MySerial.parseFloat();    
    String dataP = (String)data;
   
    float u2 = 3.3-(3.3*analogRead(HPIN)/4095) ; //Obtaining the voltage input sent by the sensor
    float hum = (u2/3.3)*100; //Parametrization
  
    String dataH = (String)hum;

    float t = dht.readTemperature();
    String dataT = (String)t;
    
    Serial.println(dataT);
    Serial.println(dataP);
    Serial.println(dataH);

    

    

  
   
   HTTPClient http;   
 
   http.begin("http://10.42.0.9:1880/post");   
   http.addHeader("Content-Type", "text/plain");             
 
   int httpResponseCode = http.POST(dataP);   //Send the actual POST request
   int httpR2 = http.POST(dataH);
   int httpR3 = http.POST(dataT);

      
   
   if(httpResponseCode>0 && httpR2 >0 && httpR3>0){
 
    
    Serial.println("Correct POST");
 
    }
   if(httpResponseCode<0 && httpR2<0 && httpR3<0){
    
    Serial.println("Error");
 
   }else{
    Serial.println("...");
   }
 
   http.end();  //Free resources
   vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    }
    
}


 
