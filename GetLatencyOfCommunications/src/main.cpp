#include <Arduino.h>

#define D1 5
#define D5 14

#define INTERRUPT_PIN_1 D1 // External interrupt pin for first interrupt
#define INTERRUPT_PIN_2 D5 // External interrupt pin for second interrupt

unsigned long interrupt1Time = 0; // Variable to store millis value at first interrupt
unsigned long interrupt2Time = 0; // Variable to store millis value at second interrupt
unsigned long previousMillis = 0; // Variable to store the previous value of millis
const long interval = 1000;       // Interval at which to execute the loop function (1 second)
int averageTime;
int timesExecuted = 0;
boolean executed = false;

// ######################################################################################################################################################
void IRAM_ATTR interrupt1Handler();
void IRAM_ATTR interrupt2Handler();
// ######################################################################################################################################################

void setup()
{
  // Initialize serial communication for debugging purposes
  Serial.begin(115200);

  // Configure the two interrupt pins as input and enable the interrupts
  pinMode(INTERRUPT_PIN_1, INPUT);
  pinMode(INTERRUPT_PIN_2, INPUT);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_1), interrupt1Handler, HIGH);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_2), interrupt2Handler, HIGH);
  Serial.println();
  Serial.println("Latency Begin");
}

void loop()
{
  unsigned long currentMillis = millis(); // Get the current value of millis

  // Check if the interval has passed and execute the loop function if it has
  if (currentMillis - previousMillis >= interval)
  {
    // Calculate and print the difference between the two interrupt times
    if (executed)
    {
      Serial.println("Interrupt 1 triggered at: " + String(interrupt1Time)); // Debugging output
      Serial.println("Interrupt 2 triggered at: " + String(interrupt2Time)); // Debugging output
      int timeDiff = interrupt2Time - interrupt1Time;
      timesExecuted++;
      averageTime = (averageTime + timeDiff) / timesExecuted;
      Serial.println("Time difference between interrupts: " + String(timeDiff) + " milliseconds");
      Serial.println("Average difference: " + String(averageTime) + "ms of " + String(timesExecuted) + " times");
      executed = false;
    }

    previousMillis = currentMillis; // Update the previous value of millis
  }
}

// Interrupt handler for first interrupt
void IRAM_ATTR interrupt1Handler()
{
  interrupt1Time = millis(); // Store current millis value in variable
  executed = true;
}

// Interrupt handler for second interrupt
void IRAM_ATTR interrupt2Handler()
{
  interrupt2Time = millis(); // Store current millis value in variable
  executed = true;
}
