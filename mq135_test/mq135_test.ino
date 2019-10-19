#define sensor 4


float RL = 20000;
float Ro = 776500; //valor obtenido en caracterización


//long RL = 10;
//long Ro = 76.63;





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

//float v = analogRead(sensor);

float v = 5.0 * analogRead(sensor) / 1024;

float Rs = RL * (5 - v) / v;

float ratio = Rs/Ro;


float ppm = 115.1 * pow(ratio,-2.924);



Serial.println(ppm);

delay(500);


}
