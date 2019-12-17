#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h> 


//******************************************** iPLANT MEASUREMENT SLAVE ************************************************
// BOARD USED: ARDUINO UNO
// MASTER: ESP-32
// IDE: PLATFORMIO
// A CODE MADE BY JUAN MANUEL MARIN AND MILLER CUBILLOS
//VARIABLES: CO2, SOIL MOISTURE, TEMPERATURE
//SENSORS: MQ135, YL100, DHT11


//Temperature sensor
DHT dht(2, DHT11); //Defining DHT11 


//CO2 sensor parameters
float RL = 20000; //According MQ135 datasheet
float Ro = 2993000; //Characterized value


//Tasks to be created
void CO2( void *pvParameters );
void Humidity( void *pvParameters );
void Temperature( void *pvParameters );


void setup () {

  //Inicialization
  Serial.begin(115200);
  dht.begin();

  //Just in case of an error
  while (!Serial){
    ;
  }
  
  //Task creator

    xTaskCreate(
    CO2
    ,  (const portCHAR *)"CO2"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

    xTaskCreate(
    Humidity
    ,  (const portCHAR *)"Humidity"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

    xTaskCreate(
    Temperature
    ,  (const portCHAR *)"Temperature"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

}

void loop()
{
  // Empty. Things are done in Tasks.
}


void CO2(void *pvParameters)  // MQ135 analog reading
{
  (void) pvParameters;

  float v1 = 5.0 * analogRead(A0) / 1023; //Obtaining the voltage input sent by the sensor
  float Rs = RL * (5.0 - v1) / v1; //Obtaining the sensor resistance
  float ratio = Rs/Ro; 
  
  float ppm = 115.1 * pow(ratio,-2.924); //Voltage into PPM transformation

  Serial.println(ppm);

  vTaskDelay(20000 / portTICK_PERIOD_MS);

}



void Humidity(void *pvParameters)  // Does nothing
{
  (void) pvParameters;

  float u2 = 5.0 - 5.0*analogRead(A1)/1023; //Obtaining the voltage input sent by the sensor
  float hum = (u2/5.0)*100; //Parametrization 

  //Serial.println(hum);
  vTaskDelay(20000 / portTICK_PERIOD_MS);

}


void Temperature(void *pvParameters)  // Does nothing
{
  (void) pvParameters;

  //built-in function
  float t = dht.readTemperature();
  //Serial.println(t);
  delay(20000);

}


