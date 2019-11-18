#define pwmPin 10

unsigned long th, tl, tpwm, p1, p2 ,ppm, ppm2, ppm3 = 0;



void setup() {
  Serial.println("Initializing.");
  Serial.begin(9600); 

  pinMode(pwmPin, INPUT);
  Serial.println("Loop started.");
}

void loop() {

  //CO2 via pwm
  th = pulseIn(pwmPin, HIGH, 3000000); // use microseconds
  tl = pulseIn(pwmPin, LOW, 3000000);
  tpwm = th + tl; // actual pulse width
  Serial.print("PWM-time:\t");
  Serial.print(tpwm);
  Serial.print("\t");
  Serial.print(th);
  Serial.print("\t");
  Serial.print(tl);
  Serial.print("\t");
  th = pulseIn(pwmPin, HIGH, 3000000);
  ppm3 = 2000 * (th-2)/(tpwm-4);
  Serial.print("PPM: ");
  Serial.println(ppm3);
  //Serial.println("-----------");

  
  delay(2000);
}
