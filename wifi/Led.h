#define LED_RED 15
#define LED_YELLOW 2
#define LED_GREEN 0

void setup_Led()
{
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, HIGH);
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

void set_health_led(int health) {
    switch (health) {
        case 3:
            set_green_led(true);
            set_yellow_led(true);
            set_red_led(true);
            break;
        case 2:
            set_green_led(false);
            set_yellow_led(true);
            set_red_led(true);
            break;
        case 1:
            set_green_led(false);
            set_yellow_led(false);
            set_red_led(true);
            break;
        default:
            set_green_led(false);
            set_yellow_led(false);
            set_red_led(false);
            break;
    }
}
