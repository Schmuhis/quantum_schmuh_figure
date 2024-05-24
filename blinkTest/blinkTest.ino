#define LED_BUILTIN 1

#include "Led.h"
#include "Servo.h"
#include "Proximity.h"

void setup()
{
  Serial.begin(9600);
  setup_Servo();
  setup_Led();
  setup_Proximity();
}

void loop()
{
  //tilt_Servo();
  // set_green_led(true);
  // delay(400);
  // set_yellow_led(true);
  // delay(400);
  // set_red_led(true);
  // delay(400);

  // set_green_led(false);
  // set_yellow_led(false);
  // set_red_led(false);

  if (check_proximity())
    Serial.println("You are close!");

  delay(100);

}