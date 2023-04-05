// IO pins setup
#define ledPin 21
#define buttonPin 22

//Setup to create task

typedef struct 
{
  // data
  int freq1;
  int freq2;
}Data;

int buttonState = LOW; //this variable tracks the state of the button, low if not pressed, high if pressed
int ledState = -1; //this variable tracks the state of the LED, negative if off, positive if on

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers


void setup()
{
  //Pins setup
  pinMode(ledPin, LedOut);
  pinMode(buttonPin, INPUT);
  // Create tasks
  xTaskCreate(
    task1, // Function name
    "Task1", // Task name
    1000, // Stack size
    1, // Priority
    NULL // Task Handle
  );
  xTaskCreate(task2,"Task2",1000,1,NULL);
  xTaskCreate(task3,"Task3",1000,1,NULL);
  xTaskCreate(task4,"Task4",1000,1,NULL);
  xTaskCreate(task5,"Task5",1000,1,NULL);
  xTaskCreate(button,"CheckButton",1000,1,NULL);
  xTaskCreate(led,"Task1",1000,1,NULL);
}

void loop()
{

}

void task1(void *parameters)
{
  for(;;)
  {
    // Task1
  }
}

void task2(void *parameters)
{
  for(;;)
  {
    // Task2
  }
}

void task3(void *parameters)
{
  for(;;)
  {
    // Task3
  }
}

void task4(void *parameters)
{
  for(;;)
  {
    // Task4
  }
}

void task5(void *parameters)
{
  for(;;)
  {
    // Task5
  }
}

void button(void *parameters)
{
  for(;;)
  {
    // Check for button
    //sample the state of the button - is it pressed or not?
    buttonState = digitalRead(buttonPin);

    //filter out any noise by setting a time buffer
    if ( (millis() - lastDebounceTime) > debounceDelay) {

      //if the button has been pressed, lets toggle the LED from "off to on" or "on to off"
      if ( (buttonState == HIGH) && (ledState < 0) ) 
      {
        //Send q
        ledState = -ledState; //now the LED is on, we need to change the state
        lastDebounceTime = millis(); //set the current time
      }
      else if ( (buttonState == HIGH) && (ledState > 0) ) 
      {
        // Send q
        ledState = -ledState; //now the LED is off, we need to change the state
        lastDebounceTime = millis(); //set the current time
      }

    }
  }
}

void led(void *parameters)
{
  for(;;)
  {
    // LED light up
  }
}