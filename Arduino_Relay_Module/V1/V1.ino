// https://www.seeedstudio.com/Relay-Shield-v3-0.html
// https://wiki.seeedstudio.com/Relay_Shield_v3/

// Digital 4 – controls RELAY4’s COM4 pin (located in J4)
// Digital 5 – controls RELAY3’s COM3 pin (located in J3)
// Digital 6 – controls RELAY2’s COM2 pin (located in J2)
// Digital 7 – controls RELAY1’s COM1 pin (located in J1)

#define Relay1 7
#define Relay2 6
#define Relay3 5
#define Relay4 4
#define TestLED 13

void setup()
{
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(TestLED, OUTPUT);

  Serial.begin(9600);
}

// ascii values A 65, B 66, C67, D 68, E 69, F70, G71, H 72

void loop()
{
  // Each relay status is controlled by unique byte sent over serial
  // makes sure to remove newline characters when sending commands e.g. with println
  
  byte InputStatus = 0;
  if (Serial.available() > 0)
  {
    InputStatus = Serial.read(); // reads one byte of incoming data
  }

// Relay 1 control characters
if (InputStatus == 65) // A
{
  digitalWrite(Relay1, LOW);
  //digitalWrite(TestLED, LOW);
}
if (InputStatus == 66) // B
{
  digitalWrite(Relay1, HIGH);
  //digitalWrite(TestLED, HIGH);
}

// Relay 2 control characters
if (InputStatus == 67) // C
{
  digitalWrite(Relay2, LOW);
}
if (InputStatus == 68) // D
{
  digitalWrite(Relay2, HIGH);
}

// Relay 3 control characters
if (InputStatus == 69) // E
{
  digitalWrite(Relay3, LOW);
}
if (InputStatus == 70) // F
{
  digitalWrite(Relay3, HIGH);
}

// Relay 4 control characters
if (InputStatus == 71) // G
{
  digitalWrite(Relay4, LOW);
}
if (InputStatus == 72) // H
{
  digitalWrite(Relay4, HIGH);
}

  delay(200);

}
