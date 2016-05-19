const int BUZZER=3;
const int GSR=A2;
int threshold=0;
int sensorValue;

void setup(){
  Serial.begin(9600);
}

void loop(){
  int temp;
  sensorValue=analogRead(GSR);
  Serial.println(sensorValue);
  delay(50);
  }

