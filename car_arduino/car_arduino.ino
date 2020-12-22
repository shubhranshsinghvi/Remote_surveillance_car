// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 3;
int in3 = 2;
int in4 = 4;
int D0 = 11; //D0 - nodeMCU
int D1 = 12; //D1 - nodeMCU
int D2 = 13; //D2 - nodeMCU (ENABLE)

void setup() {
  // Set all the motor control pins to outputs

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  pinMode(D0 , INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);

  Serial.begin(115200);

  // Turn off motors - Initial state
  turnoff();
}


void loop()
{
  
  bool a = digitalRead(D2); //ENABLE
  bool b = digitalRead(D0);
  bool c = digitalRead(D1);
 
 // Serial.println(a);
 // Serial.println(b);
 // Serial.println(c);
  if (a == HIGH)
  { 
    
    if (b == 1 && c == 1)
    {
    //  Serial.println("1");
      moveforward(5);
    }
    if (b == 0 && c == 0)
    {
      //Serial.println("2");
      movebackward(5);
      
    }
    if (b == 0 && c == 1)
    {
     // Serial.println("3");
      turnright(5);
    }
    if (b == 1 && c == 0)
    {
      //Serial.println("4");
      turnleft(5);
    }
   }
}

void turnoff() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(enA, LOW);
  digitalWrite(enB, LOW);
  
  
}

void moveforward(int tsec) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  
  delay(tsec * 1000);
  turnoff();
}

void movebackward(int tsec) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 255);
  analogWrite(enB, 255);
 
  delay(tsec * 1000);
  turnoff();
}


void turnleft(int tsec) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
 
  analogWrite(enA, 100);
  analogWrite(enB, 255);
  delay(tsec * 1000);
  turnoff();
}

void turnright(int tsec) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, 255);
  analogWrite(enB, 100);
  delay(tsec * 1000);
  turnoff();
}
