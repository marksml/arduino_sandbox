/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  long sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.print("sensorValue="); 
  Serial.println(sensorValue);

  long sensorValueMal100 = sensorValue * 100L;
  Serial.print("sensorValueMal100="); 
  Serial.println(sensorValueMal100);
  
  long volume = (sensorValue * 100L)/1023;
  Serial.print("volume="); 
  Serial.println(volume);
  Serial.println("---------------------------------------------");
  
  delay(1000);        // delay in between reads for stability
}
