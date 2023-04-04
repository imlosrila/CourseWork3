int x;
int y;
int freqT2 = 999;
int freqT3 = 999;

char buffer1[3];
char buffer2[2];

void setup()
{
Serial.begin(9600);

}

void loop()
{
  // x = 100 * (freqT2/(1000-333));
  // y = 100 * (freqT2/(1000-555));
  
  // unsigned long t10 = micros();
  // sprintf(buffer,"%d,%d",x,y);
  // unsigned long t20 = micros();
  // // Serial.println(buffer);
  // Serial.print("99,99");

  Task5A();
}

void Task5A()
{
  x = map(freqT2, 333, 1000, 0, 99);
  y = map(freqT3, 500, 1000, 0, 99);

  sprintf(buffer1,"%d,",x);
  Serial.print(buffer1);

}

void Task5B()
{
  sprintf(buffer2,"%d,",y);
  Serial.print(buffer2);
}