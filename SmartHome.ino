
#include<Keypad.h> // KEYPAD LIBRARY FOR KEYPAD INTERFACING 
#include<LiquidCrystal.h> // LIBRARY FOR LCD INTERFACING
#include<Servo.h>// LIBRARY FOR SERVO MOTOR
//#include<String.h>
#define buzzer 7// DEFINING PIN FOR BUZZER.
//ROOM DOOR
#define ENTRY 14
#define EXIT 15
#define DoorLamp 12
#define IRjewellery 8 
Servo servoTrash;
int const trigPin = 51;
int const echoPin = 50;
int sensorPin = A0;
int servoPin  = 49;

int sensorValue = 0;
int servoGrad = 90;
int tolerance = 40;

Servo servoSunflower;



LiquidCrystal lcd(19, 35, 39, 31, 30, 26); // PINS FOR LCD

char keys[4][3] = { // LAYOUT OF KEYPAD
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPin[4] = {6, 5, 4, 3}; // ROW PINS OF KEYPAD
byte colPin[3] = {2, 1, 0}; // COLUMN PINS OF KEYPAD

Servo servodoor;
String password = "357";  // SETTING DEFAULT PASSWORD.
int position = 0; // VARIABLE FOR DETERMINING THE POSITION.

int wrong = 0; // VARIABLE FOR CALCULATING THE WRONG INPUT.

int redPin = 41; // DEFINING PIN FOR RED LED
int greenPin = 53; // DEFINING PIN FOR GREEN LED

Keypad keypad = Keypad(makeKeymap(keys), rowPin, colPin, 4, 3);
// MAPPING THE KEYPAD.


int total = 0; // VARIABLE TO DETERMINE THE NUMBER OF WRONG ATTEMPTS.

int rainSensorValue = 0;
Servo windowServo; 

void setup()
{
  //DOOR
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  lcd.begin(16, 2);
  servodoor.attach(45);
  setLocked(true);
  delay(1000);

  //SUNFLOWER AND TRASH
  //pinMode(buzzer, OUTPUT); pinMode( sensorPin, INPUT);
  servoSunflower.attach( servoPin );
  servoSunflower.write( servoGrad );
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servoTrash.attach(48);

  //ROOM DOOR
  //Serial.begin(9600);
  pinMode(ENTRY, INPUT);
  pinMode(EXIT, INPUT);
  pinMode(DoorLamp, OUTPUT);
  pinMode(buzzer, OUTPUT);

   pinMode(IRjewellery, INPUT);
}
int count = 0;

int blinds=0;

void loop()
{

  lcd.clear();
  lcd.print("Enter Password:");
  delay(100);

  char pressed = keypad.getKey(); // TAKING THE INPUT FROM KEYPAD
  String key[3];

  if (pressed) // IF THE KEY IS PRESSED
  {
    lcd.clear();
    lcd.print("Enter Password:");
    lcd.setCursor(position, 2);
    lcd.print(pressed);
    delay(500);
    if (pressed == '*' || pressed == '#')
    {
      position = 0;
      setLocked(true);
      lcd.clear();
    }

    else if (pressed == password[position])
    {
      key[position] = pressed;
      position++;
    }

    else if (pressed != password[position] )
    { // IN CASE OF WRONG INPUT INCREMENT BOTH WRONG AND POSITION.
      wrong++;
      position ++;
    }

    if (position == 3)
    { // WHEN POSITION == 3 THEN CHECK THE FOLLOWING
      if ( wrong > 0) // IF ANY WRONG INPUT IF GIVEN THEN INCREMENT TOTAL AND
        // SET WRONG AND POSITION TO ZERO.
      {
        total++;
        wrong = 0;
        position = 0;
        lcd.clear();
        lcd.print("WRONG");
        lcd.setCursor(5, 2);
        lcd.print("PASSWORD");
        delay(1000);
        setLocked(true);
      }

      else if (position == 3 && wrong == 0)
      { // IF NO WRONG VALUE IS GIVEN THEN DISPLAY THE ACCEPTED PASSWORD AND
        // MOVE THE SERVO MOTOR.
        position = 0;
        wrong = 0;
        lcd.clear();
        lcd.print("PASSWORD");
        lcd.setCursor(6, 2);
        lcd.print("ACCEPTED");
        delay(2000);
        lcd.clear();
        lcd.print("Door Open");
        delay(2000);
        lcd.clear();
        lcd.setCursor(16, 0);
        lcd.print("Welcome!");
        lcd.setCursor(16, 1);
        delay(2000);
        setLocked(false);
      }

      if (total == 3) // IF TOTAL OF 3 ATTEMPTS ARE DONE BY ENTERING WRONG PASS
        //WORD THEN SOUND A BUZZER AND SET TOTAL TO 0.
      {
        total = 0;
        buzzer_beep();
        delay(500);
      }

    }

  }

  //SUNFLOWER AND TRASH
  sensorValue = analogRead(sensorPin);
  if ( sensorValue < (512 - tolerance) )
  {
    if (servoGrad < 180) servoGrad++;
  }

  if ( sensorValue > (512 + tolerance) )
  {
    if (servoGrad > 0) servoGrad--;
  }

  servoSunflower.write( servoGrad );
  int duration, distance;
  digitalWrite(trigPin, HIGH);
  delay(1);
  digitalWrite(trigPin, LOW);
  // Measure the pulse input in echo pin
  duration = pulseIn(echoPin, HIGH);
  // Distance is half the duration devided by 29.1 (from datasheet)
  distance = (duration / 2) / 29.1;
  // if distance less than 0.5 meter and more than 0 (0 or less means over range)
  if (distance <= 3 && distance >= 0) {
    servoTrash.write(50);
    delay(3000);
  } else {

    servoTrash.write(160);
  }
  // Waiting 60 ms won't hurt any one
  delay(60);

  delay(100);

//ROOM DOOR

if (digitalRead(ENTRY) == LOW) //&& digitalRead(EXIT)==HIGH)
  {

    digitalWrite(DoorLamp, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(500);
    
    if (count >= 0)
      {count++;
      Serial.print(count);}
    digitalWrite(buzzer, LOW);

  }
  else if (digitalRead(EXIT) == LOW) // && digitalRead(ENTRY)==HIGH)
  {
    if (count > 0) {
      count--;
      digitalWrite(buzzer, HIGH);
    }

    if (count == 0) {
      digitalWrite(DoorLamp, LOW);
      digitalWrite(buzzer, HIGH);
    }

    delay(500);
    digitalWrite(buzzer, LOW);

  }


 if (digitalRead(IRjewellery) == HIGH)
  {
    digitalWrite(buzzer, HIGH);
  }
  else
      {digitalWrite(buzzer, LOW);}



rainSensorValue = analogRead(A15);
  if(rainSensorValue <800 && rainSensorValue > 400)
  {
  
    if(blinds==0){
    windowServo.attach(42);
    windowServo.write(10);
   delay(1000);
          windowServo.detach();
          blinds=1;}
  }else{
 
  if(blinds==1){
  windowServo.attach(42);
          windowServo.write(90);
         delay(1000);
          windowServo.detach();
          blinds=0;}
  }
  Serial.println(rainSensorValue);
  //delay(3000);
}






void setLocked(int locked)// FUNCTION TO CHANGE STATUS OF SERVO MOTOR.
{
  if (locked)
  {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    delay(1000);
    servodoor.attach(45);
    servodoor.write(10);
    delay(1000);
    servodoor.detach();
  }
  else
  {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    delay(1000);
    servodoor.attach(45);
    servodoor.write(90);
    delay(1000);
    servodoor.detach();
  }
}
void buzzer_beep()// FUNCTION TO BEEP THE BUZZER.
{
  digitalWrite(buzzer, HIGH);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("ALARM!");
  delay(5000);
  digitalWrite(buzzer, LOW);
  delay(1000);

  delay(2000);
  // while (1)
  //{
  // lcd.scrollDisplayLeft();
  //delay(5000);
  //}
}
