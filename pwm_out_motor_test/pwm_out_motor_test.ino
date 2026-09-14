#include <Arduino.h>

#define MOTOR_CTRL_PIN 26
#define BUTTON_PIN     27

#ifdef DEBUG_MODE
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif

const int PWM_MIN = 0;
const int PWM_MAX = 255;

// Button debounce time
const unsigned long DEBOUNCE_TIME_MS = 50;

// Current PWM state
bool motorEnabled = false;

// Button states
bool lastButtonReading = HIGH;
bool buttonState = HIGH;

// Time when button input last changed
unsigned long lastDebounceTime = 0;

void setPWM(int pwm)
{
  analogWrite(MOTOR_CTRL_PIN, pwm);
}

void setMotorState(bool enabled)
{
  motorEnabled = enabled;

  if (motorEnabled)
  {
    setPWM(PWM_MAX);

    Serial.print("Motor ON. PWM: ");
    Serial.println(PWM_MAX);
  }
  else
  {
    setPWM(PWM_MIN);

    Serial.print("Motor OFF. PWM: ");
    Serial.println(PWM_MIN);
  }
}

void processButton()
{
  // INPUT_PULLUP:
  // HIGH = button released
  // LOW  = button pressed
  bool reading = digitalRead(BUTTON_PIN);

  // Button state changed -> restart debounce timer
  if (reading != lastButtonReading)
  {
    lastDebounceTime = millis();
  }

  // Check if signal has been stable long enough
  if ((millis() - lastDebounceTime) >= DEBOUNCE_TIME_MS)
  {
    // Stable state changed
    if (reading != buttonState)
    {
      buttonState = reading;

      // React only when button is pressed
      if (buttonState == LOW)
      {
        setMotorState(!motorEnabled);
      }
    }
  }

  lastButtonReading = reading;
}

void setup()
{
  Serial.begin(115200);

  DEBUG_PRINTLN("");
  DEBUG_PRINTLN("Setup started");

  pinMode(MOTOR_CTRL_PIN, OUTPUT);

  // Internal pull-up resistor enabled
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Motor OFF at startup
  setMotorState(false);

  DEBUG_PRINTLN("Setup ended");
}

void loop()
{
  processButton();

  // Other non-blocking code can run here
}