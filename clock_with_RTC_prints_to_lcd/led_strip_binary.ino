#include <FastLED.h>

// Define the number of LEDs
#define NUM_LEDS 6

// Define the pin connected to the LED data line
#define DATA_PIN 6

// Create a FastLED object
CRGB leds[NUM_LEDS];

// Function to display a number in binary using the LEDs
void displayNumberInBinary(int number)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        // Check if the ith bit is set
        if (number & (1 << i))
        {
            leds[i] = CRGB::Red; // Turn LED red (1)
        }
        else
        {
            leds[i] = CRGB::Black; // Turn LED off (0)
        }
    }
    FastLED.show();
}

void setup()
{
    // Initialize the FastLED library
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.clear();

    // Set the brightness level (0-255)
    FastLED.setBrightness(50); // Adjust the brightness as desired
}

void loop()
{
    for (int i = 0; i < 60; i++)
    {
        displayNumberInBinary(i); // Display the current number in binary
        delay(1000);              // Wait for a second
    }
}
