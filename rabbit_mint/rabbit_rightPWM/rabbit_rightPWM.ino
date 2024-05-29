  #include <Servo.h>
  #include <IBusBM.h>

  IBusBM ibus;
  Servo motor1;
  
  // Read the number of a given channel and convert to the range provided.
  // If the channel is off, return the default value
  int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
    uint16_t ch = ibus.readChannel(channelInput);
    if (ch < 100) return defaultValue;
    return map(ch, 1000, 2000, minLimit, maxLimit);
  }

  bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}
  int drive = 9;
  int rev = 4;
  int fast_speed = 2;
  int low_speed = 3;
  int ch3Val = 0;
  int ch5Val = 0;
  int ch6Val = 0;
  int pwm = 0;

  void setup() {
    // put setup code here, to run once:
    Serial.begin(115200);
    ibus.begin(Serial1);
    TCCR2B &= ~ _BV (CS22); // cancel pre-scaler of 64
    TCCR2B |= _BV (CS20);   // no pre-scaler

    pinMode(drive, OUTPUT);
    pinMode(rev, OUTPUT);
    pinMode(fast_speed,OUTPUT);
    pinMode(low_speed, OUTPUT);
  }

  void loop() {

    int ch5Val = readChannel(4, -100, 100, 0);
    int ch6Val = readChannel(5, -100, 100, 0);
    int ch3Val = readChannel(2, -100, 100, -100);
    int pwm = map(ch3Val, -100, 120, 0, 200);
    digitalWrite(fast_speed, 1);
    digitalWrite(low_speed,1);
      //clockwise
      if (ch5Val >= 0) {
        digitalWrite(rev, 0); //High is clockwise, Low is counter
        //normal speed
        if (ch6Val == 0){
        digitalWrite(fast_speed, 1);
        digitalWrite(low_speed,1);  
        
        analogWrite(drive, pwm);
        }
        //low speed
        else if (ch6Val < 0){
        digitalWrite(fast_speed, 1);
        digitalWrite(low_speed,0);  
        
        analogWrite(drive, pwm);
        }
        else if (ch6Val > 0){
        digitalWrite(fast_speed, 0);
        digitalWrite(low_speed,1);  
        
        analogWrite(drive, pwm); 
        }
      }
      //counter clockwise
      else if (ch5Val <0){
        digitalWrite(rev, 1); //High is clockwise, Low is counter
        //normal speed
        if (ch6Val == 0){
        digitalWrite(fast_speed, 1);
        digitalWrite(low_speed,1);  

        analogWrite(drive, pwm);
        }
        //low speed
        else if (ch6Val < 0){
        digitalWrite(fast_speed, 1);
        digitalWrite(low_speed,0);  

        analogWrite(drive, pwm);
        }
        else if (ch6Val > 0){
        digitalWrite(fast_speed, 0);
        digitalWrite(low_speed,1);  

        analogWrite(drive, pwm); 
        }
      }
    //delay(50);

    for (byte i = 0; i < 6; i++) {
      int value = readChannel(i, -100, 100, 0);
      Serial.print("Ch");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(value);
      Serial.print(" | "); 
    }
    Serial.println();
    delay(10); 
}