#define PROXIMITY_OUT 35


void setup_Proximity()
{
    pinMode(PROXIMITY_OUT, INPUT);
}

bool check_proximity()
{
    return digitalRead(PROXIMITY_OUT);
}