//Swastik Khuntia///
//CET,BBSR/////////
////////////////////
#include <LiquidCrystal.h>
#include <Keypad.h>
#define NUM_SAMPLES 10

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

byte rowPins[ROWS] = {10, 9, 8, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {13, 12, 11}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


int sum = 0;
unsigned char sample_count = 0;
float voltage = 0.0;

LiquidCrystal lcd(A4, A5, 5, 4, 3, 2);
const int PT = A0;
const int CT = A1;
float Cur;
float Vol;
float Power;

//For unit calculation
int unit=1;

//For consumption
float pw=0.0;

const int load=6;// For load connection

void setup() {
  //For Debugging
//Serial.begin(9600);
// set up the LCD's number of columns and rows:
lcd.begin(20,4);
// Print a message to the LCD.
lcd.setCursor(6,1);
lcd.print("Prepaid");
lcd.setCursor(5,2);
lcd.print("Energy Meter");
delay(5000);
lcd.clear();

pinMode(load,OUTPUT);//Output for load

Constants();
}

void loop() {
while (unit==0)
{
 //Serial.println("In loop");//For Debugging Only
 digitalWrite(load,LOW); //Recharge Empty; Turn off load and ask to recharge
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("No Balance");
 lcd.setCursor(0,1);
 lcd.print("Recharge Now");
 //For recharge (keypad)
check_keypad();
// Serial.print(unit);//For Debugging
  }
 //Serial.print(unit);//For Debugging
digitalWrite(load,HIGH);
//lcd.setCursor(0, 2);
ShowVoltage(3, 0, PT);
Vol = voltage;
ShowVoltage(3, 1, CT);
Cur = voltage;
Power = Vol * Cur;
pw=pw+Power;

//Power and Unit Calculation
if (pw>10)   //Assumption: 1 Unit=10 W
{
  //Serial.println("Entered loop 2");//For Debugging Only
  unit=unit-1;
  pw=0;
  }
//remaining Unit printing
lcd.setCursor(14,0);
lcd.print("U:");
lcd.setCursor(16,0);
lcd.print(unit);
//Power consumption and cumilitive power
lcd.setCursor(3,2);
lcd.print(Power);
lcd.setCursor(3,3);
lcd.print(pw);
//Recharge before Null
check_keypad();
digitalWrite(load,HIGH);
}

void Constants()
{
lcd.setCursor(0,0);
lcd.print("V: ");
lcd.setCursor(0,1);
lcd.print("C: ");
lcd.setCursor(0,2);
lcd.print("P: ");
lcd.setCursor(0,3);
lcd.print("C: "); //Cumulative power
lcd.setCursor(8,0);
lcd.print("V");
lcd.setCursor(8,1);
lcd.print("A");
lcd.setCursor(8,2);
lcd.print("W");
lcd.setCursor(8,3);
lcd.print("W");
}

void ShowVoltage (int x,int y, unsigned int value)
{
while (sample_count < NUM_SAMPLES)
{
sum += analogRead(value);
sample_count++;
delay(10);
}

voltage = ((float)sum / (float)NUM_SAMPLES * 5.015) / 1024.0;
lcd.setCursor(x, y);
lcd.print(voltage);
sample_count = 0;
sum = 0;
}

void check_keypad()
{
  char key = keypad.getKey();
  Serial.println(key);//Debugging
  
  if (key)
  {
    if (key=='1')
      {
        //Serial.println("100");//Debugging
        unit=unit+10;
        lcd.clear();
        Constants();
      }
    else if (key=='2')
      {
        unit=unit+20;
        lcd.clear();
        Constants();
      }  
     else if (key=='3')
      {
        unit=unit+30;
        lcd.clear();
        Constants();
        }  
    else
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter a valid");
      lcd.setCursor(0,1);
      lcd.print("Recharge Amount");
      delay(2000);
    }
  }
    }
