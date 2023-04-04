// debounce.ino 
// MIT License (see file LICENSE)

// LED is active high
#define GPIO_LED    12 
#define GPIO_BUTTON 25

static QueueHandle_t queue;

//
// Button Debouncing task:
//
static void debounce_task(void *argp) {
  uint32_t level, state = 0, last = 0xFFFFFFFF;
  uint32_t mask = 0x7FFFFFFF;
  bool event;
  
  for (;;) {
    level = !!digitalRead(GPIO_BUTTON);
    state = (state << 1) | level;
    if ( (state & mask) == mask
      || (state & mask) == 0 ) {
      if ( level != last ) {
        event = !!level;
        if ( xQueueSendToBack(queue,&event,1) == pdPASS )
          last = level;
      }
    }
    taskYIELD();
  }
}

//
// LED queue receiving task
//
static void led_task(void *argp) {
  BaseType_t s;
  bool event, led = false;
  
  // Light LED initially
  digitalWrite(GPIO_LED,led);

  for (;;) {
    s = xQueueReceive(
      queue,
      &event,
      portMAX_DELAY
    );
    assert(s == pdPASS);
    if ( event ) {
      // Button press:
      // Toggle LED
      led ^= true;
      digitalWrite(GPIO_LED,led);
    }
  }
}

