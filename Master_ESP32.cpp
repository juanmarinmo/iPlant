#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <string.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>
#include <sstream>


HardwareSerial MySerial(1);

const int HPIN = 36;

QueueHandle_t queue; //Se declara la cola


void receiver( void *pvParameters ); //tarea 1
void fpgasender( void *pvParameters );   //tarea 2
void senderwifi( void *pvParameters ); //tarea 3


//Conectandose a la red

char* ssid = " "; //Definir nombre WIFI
char* password = " "; //Definir Clave

char serverAddress[] = "192.168.0.3";  // Dirección IP de Raspberry PI
int port = 8080; //Definir puerto


WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {
 
  Serial.begin(115200);
  MySerial.begin(115200, SERIAL_8N1, 16, 17);
  WiFi.begin(ssid, password); //Inicia WiFi
  
  //Salida FPGA
  pinMode (5, OUTPUT);

  //En caso de error
  while (!Serial){
    ;
  }
	
   queue = xQueueCreate(4, sizeof( float ) );
   

    if(queue == NULL){
    Serial.println("Error creating the queue"); //En caso de error en la cola
  }


  //Creador de tareas

    xTaskCreate(&receiver, "receiver", 1024, NULL, 2, NULL);
    xTaskCreate(&fpgasender, "fpgasender", 1024, NULL, 2, NULL);
    xTaskCreate(&senderwifi, "senderwifi", 1024, NULL, 2, NULL);
    
}

void loop(){
  
}

void receiver(void *pvParameters) //Recibe datos por UART
{
  (void) pvParameters;

  for(;;){
   
    if(MySerial.available()){
    
    //String data = MySerial.readStringUntil(' ');
    float data = MySerial.parseFloat();    
    
    //Serial.println(data);

    xQueueSend(queue, &data, portMAX_DELAY);

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    }else{
      Serial.println("error");
    }
  }
}
	 

void fpgasender(void *pvParameters)  // YL100 analog reading
{
  (void) pvParameters;
  
  for(;;){
  float u2 = 3.3-(3.3*analogRead(HPIN)/4095) ; //Obtaining the voltage input sent by the sensor
  float hum = (u2/3.3)*100; //Parametrization 
 
  if(hum<10){ 
  
   digitalWrite(5, HIGH);
   
  }if(hum>10){
   
   digitalWrite(5, LOW);
    
  }  

  xQueueSend(queue, &hum, portMAX_DELAY);
  vTaskDelay(2000/portTICK_PERIOD_MS);
  }
}

void senderwifi(void *pvParameters) //Por el momento solo recopila datos
{
  (void) pvParameters;

 for(;;){
     float Rx;
     String floatRX = (String)Rx;
      
     xQueueReceive(queue,&Rx, 1000/portTICK_RATE_MS);

    String contentType = "text/plain";
    
    client.post("/", "text/plain", floatRX);

    client.stop();

     vTaskDelay(500/portTICK_PERIOD_MS);


        
  }
  
}


