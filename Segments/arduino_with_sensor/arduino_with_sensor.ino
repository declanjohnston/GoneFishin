const int tempPin = 0;

void setup() {
  Serial.begin(9600);
}
void loop(){
  float voltage, degreesC;
  voltage=getVoltage(tempPin);
  degreesC=(voltage)*100.0;
  Serial.print("voltage: ");
  Serial.print(voltage);
  Serial.print(", Deg C: ");
  Serial.println(degreesC);
  delay(1000);
}

float getVoltage(int pin){
  return (analogRead(pin)*0.004882814);
}

