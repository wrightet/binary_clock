#include <Arduino.h>
const int min_pins[] = {13, 14, 15, 16, 17, 18}; // Change these pin numbers according to your setup
const int hour_pins[] = {0, 2, 4, 5, 19, 23};     // Change these pin numbers according to your setup

int hour_start= 18;
int min_start = 45;

void setup()
{
    // Set the LED pins as outputs
    for (int i = 0; i < 6; i++)
    {
        pinMode(min_pins[i], OUTPUT);
        pinMode(hour_pins[i], OUTPUT);
    }
    //    Serial.begin(9600);
}

void loop()
{
    // count hours 0 - 23
    for (int hour = 0; hour < 24; hour++)
    {
        // Convert the number to binary and display it on the seconds LEDs
        displayBinary(hour, hour_pins);
        // Display hours
        //        Serial.print("hours: ");
        //        Serial.println(hour);

        // Count minutes from 0 to 59
        for (int min = 0; min < 60; min++)
        {
            // Convert the number to binary and display it on the minutes LEDs
            displayBinary(min, min_pins);
            // Display seconds
            //            Serial.print("seconds: ");
            //            Serial.println(sec);
            for (int sec =0; sec < 60; sec++){
                delay(1000);
            }
        }
    }
}

// Function to display a number in binary on the LEDs
void displayBinary(int num, const int *pins)
{
    for (int i = 0; i < 6; i++)
    {
        // Turn the LED on if the corresponding bit is 1, otherwise turn it off
        digitalWrite(pins[i], !bitRead(num, 5 - i));
    }
}
