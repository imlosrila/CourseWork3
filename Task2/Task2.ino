#define in 23

unsigned long highTime;
int freq;


void setup() 
{
  pinMode(in, INPUT);
  int freq;
  Serial.begin(9600);

}

void loop() 
{
  
  highTime = pulseIn(in, HIGH, 1 );
  if (highTime < 3 )
  {
    Serial.println("Too low");
  }

  freq = 1/(highTime *2 * 0.000001);

  Serial.println(freq);
}
