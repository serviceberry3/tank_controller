//define motor pins
int motor1pin1 = 5;
int motor1pin2 = 4;
int motor2pin1 = 7;
int motor2pin2 = 6;

void setup() {
  //motor +/- (spin direction ctrl) pins
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);

  //motor EN pins (PWM)
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT);

  //open up comms with the ESP-01
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    //read in one byte
    char data_rcvd = Serial.read();
    
    //ctrl motors appropriately

    //forward
    if (data_rcvd == '0')
      {
          //max speed 255, so set to low speed
          analogWrite(9, 100); 
          analogWrite(10, 100);

          //set motor direction
          digitalWrite(motor1pin1, LOW);
          digitalWrite(motor1pin2, HIGH);
          digitalWrite(motor2pin1, LOW);
          digitalWrite(motor2pin2, HIGH);
      }

      //back
      else if (data_rcvd == '1')
      {
          //set motor to low speed
          analogWrite(9, 100); 
          analogWrite(10, 100);

          //set motor direction
          digitalWrite(motor1pin1, HIGH);
          digitalWrite(motor1pin2, LOW);  
          digitalWrite(motor2pin1, HIGH);
          digitalWrite(motor2pin2, LOW);   
      }

      //left
      else if (data_rcvd == '2')
      {
          //set motor to low speed
          analogWrite(9, 100); 
          analogWrite(10, 100);
          
          //motor 1 fwd, motor 2 back
          digitalWrite(motor1pin1, HIGH);
          digitalWrite(motor1pin2, LOW); 
          digitalWrite(motor2pin1, LOW);
          digitalWrite(motor2pin2, HIGH);
      }

      //right
      else if (data_rcvd == '3')
      {
          //set motor to low speed
          analogWrite(9, 100); 
          analogWrite(10, 100);

          //motor 1 back, motor 2 fwd
          digitalWrite(motor1pin1, LOW);
          digitalWrite(motor1pin2, HIGH); 
          digitalWrite(motor2pin1, HIGH);
          digitalWrite(motor2pin2, LOW);   
      }

      //stop
      else {
        //all motors off
        digitalWrite(motor1pin1, LOW);
        digitalWrite(motor1pin2, LOW);
        digitalWrite(motor2pin1, LOW);
        digitalWrite(motor2pin2, LOW);
      }
  }
}
