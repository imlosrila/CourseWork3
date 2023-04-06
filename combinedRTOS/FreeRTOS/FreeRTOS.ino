// IO pins setup
//Queue handle

static QueueHandle_t qh;

// ===============Button/LED===============//

#define buttonPin 33
#define ledPin 25

int buttonState = LOW; //this variable tracks the state of the button, low if not pressed, high if pressed
int ledState = -1; //this variable tracks the state of the LED, negative if off, positive if on

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// ===============Task 1===============//
#define outT1 21                   // Pin 21 
int step = 0;                      // Checking wich step to use within the ifs
int done = 0;                      // Flag when all the tasks is done


unsigned long startTime = 0;      // These are the timings in microseconds
const long interval1 = 200;
const long interval2 = 50;
const long interval3 = 30; 


unsigned long timeNow = 0;        // Initialization for timestamps 
unsigned long timeNow2 = 0;
unsigned long timeNow3 = 0;


// ===============Task 2===============// 
#define inT2 22                    // pin 22 for Task 2

unsigned long highTimeT2;          // Half of wavelength
int freqT2;                        // Variable that holds the frequency 
bool stateT2;                      // Checking the state at which the analog read enters

// ===============Task 3===============//
#define inT3 23                    // pin 23 for task 3

unsigned long highTimeT3;          // Half og wavelength
int freqT3;                        // Variable that holds the frequency
bool stateT3;                      // Checking the state at which the analof read enters

// ===============Task 4===============//

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
#define potPin 34
#define led 27                            

// variable for storing the potentiometer value
int avgVal = 0;
int toVal = 0;
int potVal[4] = {0};
int count = 0;

// ===============Task 5===============//

// Variables for map of the frequency values
int x;
int y;


//Setup to create task

typedef struct 
{
  // data
  int freq1;
  int freq2;
}Data;

Data taskf;
SemaphoreHandle_t SMF; //Semaphore handle



void task1(void *parameters)
{
  TickType_t xLastWakeTime;
  const TickType_t Period1 = 4;

  // unsigned stack_hwm = 0, temp;

  // Initialise the xLastWakeTime variable with the current time.
  xLastWakeTime = xTaskGetTickCount();

  for(;;)
  {
    vTaskDelayUntil( &xLastWakeTime, Period1 );

    digitalWrite(outT1,HIGH);
    timeNow = micros();        // get the current time 
    done = 0;                  // setting done state to 0 meaning tasks are not done yet

    while(done == 0)
    {
      if((micros() >= (timeNow + interval1) ) && step == 0)      // Make signal low after 200us
      {
       digitalWrite(outT1,LOW);
        timeNow2 = micros();
        step = 1;
      }

      if((micros() >= (timeNow2 + interval2) ) && step == 1)    // make signal high again after 50us
      {
        digitalWrite(outT1,HIGH);
        timeNow3 = micros();
        step = 2;
      }

      if((micros() >= (timeNow3 + interval3) ) && step == 2)  // make signal low after 30us
      {
        digitalWrite(outT1,LOW);
        timeNow =  micros();
        step = 0;
        done = 1;    // done so that we can get out of this while loop
      }
    }
  }
  // temp = uxTaskGetStackHighWaterMark(NULL); 
  //   if (!stack_hwm || temp < stack_hwm)
  // {
  //   stack_hwm = temp;

  //   printf("%u",stack_hwm);
  //   // Serial.println(stack_hwm);
  // }
}

void task2(void *parameters)
{

  TickType_t xLastWakeTime;
  const TickType_t Period2 = 20;

  // Initialise the xLastWakeTime variable with the current time.
  xLastWakeTime = xTaskGetTickCount();

  for(;;)
  {

    vTaskDelayUntil( &xLastWakeTime, Period2 );
    if(xSemaphoreTake(SMF,portMAX_DELAY) == pdTRUE)
    {

    stateT2 = digitalRead(inT2);                     // checks the current state of signal ( high or low )


      stateT2 = digitalRead(inT2);                     // checks the current state of signal ( high or low )              
      // checks the current state of signal ( high or low )

      highTimeT2 = pulseIn(inT2, !stateT2,3000);       // measures the half wave length of either high or low signal

      if (freqT2 < 333)
      {
        // condition for too high or low which lets it to 0
        taskf.freq1 = 333;
      }

      else if (freqT2 > 1000)
      {
      taskf.freq1 = 1000;
      }

      else
      {
        taskf.freq1 = freqT2;
      }
        xSemaphoreGive(SMF);
    }
    
  }
}

void task3(void *parameters)
{
  // Data task3f;
  TickType_t xLastWakeTime;
  const TickType_t Period3 = 8;

  // Initialise the xLastWakeTime variable with the current time.
  xLastWakeTime = xTaskGetTickCount();

  for(;;)
  {
    vTaskDelayUntil( &xLastWakeTime, Period3 );

    if (xSemaphoreTake(SMF,portMAX_DELAY) == pdTRUE)
    {

      stateT3 = digitalRead(inT3);                   // checks the current state of signal ( high or low )

      highTimeT3 = pulseIn(inT3, !stateT3,2000);     // measures the half wavelength of either the high or low signal

      freqT3 = 1/(highTimeT3 *2 * 0.000001);         // calculating the frequency

       if (freqT3 < 500)
    {
     // condition for too high or low which lets it to 0
     taskf.freq2 = 500;
    }

    else if (freqT3 > 1000)
    {
     taskf.freq2 = 1000;
    }

    else 
    {
      taskf.freq2 = freqT3;
    }
    xSemaphoreGive(SMF);
    }
  }
}

void task4(void *parameters)
{
  TickType_t xLastWakeTime;
  const TickType_t Period4 = 100;

  // Initialise the xLastWakeTime variable with the current time.
  xLastWakeTime = xTaskGetTickCount();

  for(;;)
  {
    vTaskDelayUntil( &xLastWakeTime, Period4 );
    if (count < 4)                        // adds the value to the next array as counter goes up
    {                                      // as long as the last array is not filled
      potVal[count] = analogRead(potPin);
     count ++;
    }

    else
    {
      potVal[0] = analogRead(potPin);     // if last position of the array was used
      count = 1;                          // this goes back to add the reading to position 1
                                        // and increment count
    }

    for (int k = 0; k < 4; k++)           // calculating the sum of the numbers in the array
    {
      toVal += potVal[k];
    }

    avgVal = toVal / 4;                  // calculating the average 

    if (avgVal >= 2047)                  // checks if the pot have reached half of its value
    {
      digitalWrite(led, HIGH);
    }
    else
    {
     digitalWrite(led, LOW);
    }
    toVal = 0;                           // resets the value of total sum to 0
  }
}

void task5(void *parameters)
{
  TickType_t xLastWakeTime;
  const TickType_t Period5 = 100;

  // Initialise the xLastWakeTime variable with the current time.
  xLastWakeTime = xTaskGetTickCount();

  for(;;)
  {
    vTaskDelayUntil( &xLastWakeTime, Period5 );

   if (xSemaphoreTake(SMF,portMAX_DELAY) == pdTRUE)
  {
    x = map(taskf.freq1, 333, 1000, 0, 99);
    y = map(taskf.freq2, 500, 1000, 0, 99);
    int X = constrain(x, 0, 99);
    int Y = constrain(y, 0, 99);
    Serial.print(X);
    Serial.print(",");
    Serial.println(Y);

     xSemaphoreGive(SMF);
  }


  }
}

void button(void *parameters)
{
  uint32_t level, state = 0, last = 0xFFFFFFFF;
  uint32_t mask = 0x7FFFFFFF;
  bool event;
  
  for (;;) 
  {

    level = !!digitalRead(buttonPin);
    state = (state << 1) | level;
    if ( (state & mask) == mask|| (state & mask) == 0 )
       {
        if ( level != last ) 
        {
          event = !!level;
          if ( xQueueSendToBack(qh,&event,1) == pdPASS )
            last = level;
        }
      }
  }  
  taskYIELD();
}

void ledOut(void *parameters)
{

  // LED light up
  BaseType_t s;
  bool event, ledState = false;
  
  // Light LED initially
  digitalWrite(ledPin,ledState);

  for (;;) 
  {

    s = xQueueReceive(
      qh,
      &event,
      portMAX_DELAY
       );
      assert(s == pdPASS);
     if ( event ) 
    {
       // Button press:
      // Toggle LED
       ledState ^= true;
       digitalWrite(ledPin,ledState);
     }
   }
  
}

void loop()
{

}

void setup()
{
  BaseType_t rc;
  //creating a que
  // qh = xQueueCreate(5,sizeof(uint32_t));
  delay(2000);          // Allow USB to connect
  qh = xQueueCreate(40,sizeof(bool));
  assert(qh);
  //Semaphore initialisation
  SMF = xSemaphoreCreateMutex();
  //Pins setup

  Serial.begin(9600);
  //Task 1 initialization
  pinMode(outT1, OUTPUT);

  //Task 2 initialization
  pinMode(inT2, INPUT);
  int freqT2;
  
    
  //Task 3 initialization
  pinMode(inT3, INPUT);
  int freqT3;
  
  
  //Task 4 initialization
  pinMode(led, OUTPUT);

  //Task 5 initialization

  //Button/Led
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  

  // Create tasks

  xTaskCreatePinnedToCore(
    task1, // Function name
    "Task1", // Task name
    4096, // Stack size
    NULL, // Param
    1, // Priority
    NULL, // Task Handle
    1
  );
  
  rc = xTaskCreatePinnedToCore(task2,"Task2",4096,NULL,3,NULL,1);
  assert(rc == pdPASS);

  rc = xTaskCreatePinnedToCore(task3,"Task3",4096,NULL,3,NULL,1);
  assert(rc == pdPASS);

  rc = xTaskCreatePinnedToCore(task4,"Task4",4096,NULL,1,NULL,1);
  assert(rc == pdPASS);

  rc = xTaskCreatePinnedToCore(task5,"Task5",4096,NULL,2,NULL,1);
  assert(rc == pdPASS);

  rc = xTaskCreatePinnedToCore(button,"CheckButton",4096,NULL,1,NULL,1);
  assert(rc == pdPASS);

  rc = xTaskCreatePinnedToCore(ledOut,"LED",4096,NULL,1,NULL,1);
  assert(rc == pdPASS);

 
}