// LED is active high
#define GPIO_LED    12 
#define GPIO_BUTTON 25

//Setup to create task

typedef struct 
{
  // data
  int freq1;
  int freq2;
}Data;


void setup()
{
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
  }
}

void led(void *parameters)
{
  for(;;)
  {
    // LED light up
  }
}