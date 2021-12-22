void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

int val;

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(20);
  Serial.print("analog is: ");
  Serial.print(val*(3.3/1023.0));
  Serial.print("\n");
  delay(4000);
}
