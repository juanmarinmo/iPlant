int sensor = 36;







float RL = 20000;
float Ro = 776500; //valor obtenido en caracterización



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
//  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_0db);

}

void loop() {

//float v = 5.0 * analogRead(sensor) / 1024;


float v = 3.3 * analogRead(sensor) / 4095;

float Rs = RL * (3.3 - v) / v;

float ratio = Rs/Ro;


float ppm = 115.1 * pow(ratio,-2.924);



Serial.println(analogRead(sensor));

delay(1000);


}
