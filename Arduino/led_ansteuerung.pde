/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

void setup()
{                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(0, OUTPUT);     
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop()
{
  //Ascending();
  //Flashlight();
  Show();
  //KnightRiderx2();
  //KnightRiderx4();
  //TwoCircles();
  //Circle();
}

void Circle()
{
  int onTime = 50;
  int offTime = 1;
  
  OneOnOFF(1, onTime, offTime);
  OneOnOFF(2, onTime, offTime);
  OneOnOFF(3, onTime, offTime);
  OneOnOFF(4, onTime, offTime);
  OneOnOFF(8, onTime, offTime);
  OneOnOFF(12, onTime, offTime);
  OneOnOFF(16, onTime, offTime);
  OneOnOFF(15, onTime, offTime);
  OneOnOFF(14, onTime, offTime);
  OneOnOFF(13, onTime, offTime);
  OneOnOFF(9, onTime, offTime);
  OneOnOFF(5, onTime, offTime);  
}

void TwoCircles()
{
  int onTime = 50;
  int offTime = 1;

  TwoVirtuallyOn(1, 11, onTime);
  TwoVirtuallyOn(2, 7, onTime);
  TwoVirtuallyOn(3, 6, onTime);
  TwoVirtuallyOn(4, 10, onTime);
  TwoVirtuallyOn(8, 11, onTime);
  TwoVirtuallyOn(12, 7, onTime);  
  TwoVirtuallyOn(16, 6, onTime);
  TwoVirtuallyOn(15, 10, onTime);
  TwoVirtuallyOn(14, 11, onTime);
  TwoVirtuallyOn(13, 7, onTime);
  TwoVirtuallyOn(9, 6, onTime);
  TwoVirtuallyOn(5, 10, onTime);
  
  TwoVirtuallyOn(1, 11, onTime);
  TwoVirtuallyOn(2, 7, onTime);
  TwoVirtuallyOn(3, 6, onTime);
  TwoVirtuallyOn(4, 10, onTime);
  TwoVirtuallyOn(8, 11, onTime);
  TwoVirtuallyOn(12, 7, onTime);  
  TwoVirtuallyOn(16, 6, onTime);
  TwoVirtuallyOn(15, 10, onTime);
  TwoVirtuallyOn(14, 11, onTime);
  TwoVirtuallyOn(13, 7, onTime);
  TwoVirtuallyOn(9, 6, onTime);
  TwoVirtuallyOn(5, 10, onTime);
}

void TwoVirtuallyOn(int LED1, int LED2, int onTime)
{
  for(int i=0; i<onTime; i++)
  {
    OneOnOFF(LED1, 1, 0);
    OneOnOFF(LED2, 1, 0);
  }
}

void FourVirtuallyOn(int LED1, int LED2, int LED3, int LED4, int onTime)
{
  for(int i=0; i<onTime; i++)
  {
    OneOnOFF(LED1, 1, 0);
    OneOnOFF(LED2, 1, 0);
    OneOnOFF(LED3, 1, 0);
    OneOnOFF(LED4, 1, 0);
  }
}

void KnightRiderx4()
{
  int onTime = 100;
  int offTime = 1;
  
  FourVirtuallyOn(1, 8, 9, 16, onTime);
  FourVirtuallyOn(2, 7, 10, 15, onTime);
  FourVirtuallyOn(3, 6, 11, 14, onTime);
  FourVirtuallyOn(4, 5, 12, 13, onTime);  
  FourVirtuallyOn(3, 6, 11, 14, onTime);
  FourVirtuallyOn(2, 7, 10, 15, onTime);
}

void KnightRiderx2()
{
  int onTime = 200;
  int offTime = 1;
  
  TwoVirtuallyOn(1, 8, onTime);
  TwoVirtuallyOn(2, 7, onTime);
  TwoVirtuallyOn(3, 6, onTime);
  TwoVirtuallyOn(4, 5, onTime);  
  TwoVirtuallyOn(3, 6, onTime);
  TwoVirtuallyOn(2, 7, onTime);
}

void Show()
{
  int onTime = 50;
  int offTime = 1;
  
  //in
  OneOnOFF(1, onTime, offTime);
  OneOnOFF(2, onTime, offTime);
  OneOnOFF(3, onTime, offTime);
  OneOnOFF(4, onTime, offTime);
  OneOnOFF(8, onTime, offTime);
  OneOnOFF(12, onTime, offTime);
  OneOnOFF(16, onTime, offTime);
  OneOnOFF(15, onTime, offTime);
  OneOnOFF(14, onTime, offTime);
  OneOnOFF(13, onTime, offTime);
  OneOnOFF(9, onTime, offTime);
  OneOnOFF(5, onTime, offTime);
  OneOnOFF(6, onTime, offTime);
  OneOnOFF(7, onTime, offTime);
  OneOnOFF(11, onTime, offTime);
  OneOnOFF(10, onTime, offTime);
  
  //out
  OneOnOFF(6, onTime, offTime);
  OneOnOFF(7, onTime, offTime);
  OneOnOFF(8, onTime, offTime);  
  OneOnOFF(12, onTime, offTime);  
  OneOnOFF(16, onTime, offTime);
  OneOnOFF(15, onTime, offTime);
  OneOnOFF(14, onTime, offTime);
  OneOnOFF(13, onTime, offTime);
  OneOnOFF(9, onTime, offTime);
  OneOnOFF(5, onTime, offTime);  
}

void Flashlight()
{
  int onTime = 1;
  int offTime = 0;
  
  for(int i=1; i<=16; i++)
  {  
      OneOnOFF(i, onTime, offTime);
  }
}

void Ascending()
{
  int onTime = 250;
  int offTime = 100;
  
  for(int i=1; i<=16; i++)
  {  
      OneOnOFF(i, onTime, offTime);
  }
}

void OneOnOFF(int numLED, int onTime, int offTime)
{
   OneON(numLED, onTime);
   AllOFF(offTime);
}

void AllOFF(int offTime)
{
   //disable all!
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);    
  digitalWrite(2, HIGH);    
  digitalWrite(3, HIGH);
  
  delay(offTime);
}

void OneON(int numLED, int onTime)
{
  //==================================================================
  //first ROW
  //==================================================================
  if(numLED == 1)
  {
    //enable led 1
    digitalWrite(0, HIGH);   // set the LED on
    digitalWrite(1, LOW);    // set the LED off
    digitalWrite(2, LOW);    // set the LED off
    digitalWrite(3, LOW);    // set the LED off  
    
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    
    delay(onTime);    
    return;
  }
  
  if(numLED == 2)
  {
    //enable led 1
    digitalWrite(0, LOW);   // set the LED on
    digitalWrite(1, HIGH);    // set the LED off
    digitalWrite(2, LOW);    // set the LED off
    digitalWrite(3, LOW);    // set the LED off  
    
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    
    delay(onTime);    
    return;
  }
  
  if(numLED == 3)
  {
    //enable led 1
    digitalWrite(0, LOW);   // set the LED on
    digitalWrite(1, LOW);    // set the LED off
    digitalWrite(2, HIGH);    // set the LED off
    digitalWrite(3, LOW);    // set the LED off  
    
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    
    delay(onTime);    
    return;
  }
  
  if(numLED == 4)
  {
    //enable led 1
    digitalWrite(0, LOW);   // set the LED on
    digitalWrite(1, LOW);    // set the LED off
    digitalWrite(2, LOW);    // set the LED off
    digitalWrite(3, HIGH);    // set the LED off  
    
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    
    delay(onTime);    
    return;
  }
  
  //==================================================================
  //second ROW
  //==================================================================
    if(numLED == 5)
  {
    //enable led 1
    digitalWrite(0, HIGH);   // set the LED on
    digitalWrite(1, LOW);    // set the LED off
    digitalWrite(2, LOW);    // set the LED off
    digitalWrite(3, LOW);    // set the LED off  
    
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    
    delay(onTime);    
    return;
  }
  
  if(numLED == 6)
  {
    //enable led 1
    digitalWrite(0, LOW);   // set the LED on
    digitalWrite(1, HIGH);    // set the LED off
    digitalWrite(2, LOW);    // set the LED off
    digitalWrite(3, LOW);    // set the LED off  
    
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    
    delay(onTime);    
    return;
  }
  
  if(numLED == 7)
  {
    //enable led 1
    digitalWrite(0, LOW);   // set the LED on
    digitalWrite(1, LOW);    // set the LED off
    digitalWrite(2, HIGH);    // set the LED off
    digitalWrite(3, LOW);    // set the LED off  
    
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    
    delay(onTime);    
    return;
  }
  
  if(numLED == 8)
  {
    //enable led 1
    digitalWrite(0, LOW);   // set the LED on
    digitalWrite(1, LOW);    // set the LED off
    digitalWrite(2, LOW);    // set the LED off
    digitalWrite(3, HIGH);    // set the LED off  
    
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    
    delay(onTime);    
    return;
  }
  
  //==================================================================
  //third ROW
  //==================================================================
  
  if(numLED == 9)
  {
    //enable led 1
    digitalWrite(0, HIGH);
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW); 
    
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    
    delay(onTime);    
    return;
  }
  
  if(numLED == 10)
  {
    //enable led 1
    digitalWrite(0, LOW);
    digitalWrite(1, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);  
    
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    
    delay(onTime);    
    return;
  }
  
  if(numLED == 11)
  {
    //enable led 1
    digitalWrite(0, LOW);
    digitalWrite(1, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    
    delay(onTime);    
    return;
  }
  
  if(numLED == 12)
  {
    //enable led 1
    digitalWrite(0, LOW);
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH); 
    
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    
    delay(onTime);    
    return;
  }
  
  //==================================================================
  //fourth ROW
  //==================================================================
  
  if(numLED == 13)
  {
    //enable led 1
    digitalWrite(0, HIGH);
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW); 
    
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    
    delay(onTime);    
    return;
  }
  
  if(numLED == 14)
  {
    //enable led 1
    digitalWrite(0, LOW);
    digitalWrite(1, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);  
    
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    
    delay(onTime);    
    return;
  }
  
  if(numLED == 15)
  {
    //enable led 1
    digitalWrite(0, LOW);
    digitalWrite(1, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    
    delay(onTime);    
    return;
  }
  
  if(numLED == 16)
  {
    //enable led 1
    digitalWrite(0, LOW);
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH); 
    
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    
    delay(onTime);    
    return;
  }
}
