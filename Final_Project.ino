#include <Servo.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10);
String command;
Servo myServo;
int pos = 0;
int r=0;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;
String IDname;

void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  myServo.write(0);
  delay(1000);
  //Fingerprint sensor module setup
  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  }
  else {
    Serial.println("Did not find fingerprint sensor 😞");
    while (1) {
      delay(1);
    }
  }
}


void P180(){
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);
  }
}

void P0(){

  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void loopV() {
  while (Serial1.available()) {
    delay(5);

    char c = Serial1.read();

    if (c == '#')
    {

      break;

    } //Exit the loop when the # is detected after the word


    command += c; //Shorthand for voice = voice + c

  }

  if(command.length() > 0) {

    Serial.println(command);

     if (command == "*lock")
    {
      P180();
      delay(15);
    }


    else if (command == "*unlock")
    {
      P0();                     // waits 15ms for the servo to reach the position
      delay(15);
    }
  }

  command = "";
}


void loopF() {
  //displayMainScreen();
  fingerprintID = getFingerprintIDez();
  delay(50);
  if (fingerprintID == 1 || fingerprintID == 2) {
         r=1;
    }
    Serial.println("");
    Serial.println(" ok ");

  }

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  return finger.fingerID;
}



void loop() {
  loopF();
  if(r==1)
  {
    loopV();
    r=0;
  }
}
