#define LED_RED 15
#define LED_YELLOW 2
#define LED_GREEN 0

void setup_Led()
{
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
}

void set_red_led(bool state)
{
    if(state == true)
        digitalWrite(LED_RED, HIGH);
    else
        digitalWrite(LED_RED, LOW);
}

void set_yellow_led(bool state)
{
    if(state == true)
        digitalWrite(LED_YELLOW, HIGH);
    else
        digitalWrite(LED_YELLOW, LOW);
}

void set_green_led(bool state)
{
    if(state == true)
        digitalWrite(LED_GREEN, HIGH);
    else
        digitalWrite(LED_GREEN, LOW);
}