#include <DS3231.h>
#include <Wire.h>
#include <FastLED.h>

// Init the DS3231 using the hardware interface
DS3231 rtc(SDA, SCL);

// Pins in order of most to least significant bit
const int min_pin = 6;
const int hour_pin = 7;
const int NUM_LEDS = 6;

// Create a FastLED object
CRGB min_leds[NUM_LEDS];
CRGB hour_leds[NUM_LEDS];

void parseTimeString(const String &timeString, int &hour, int &minute);

int manualHour = 0;
int manualMinute = 0;

const int buttonPins[] = {47, 49, 51, 53}; // Pins where buttons are connected

bool timeAdjusted = false;

void setup()
{
    // Start the serial port
    Serial.begin(57600);

    // Initialize the FastLED library
    FastLED.addLeds<WS2812B, min_pin, GRB>(min_leds, NUM_LEDS);
    FastLED.addLeds<WS2812B, hour_pin, GRB>(hour_leds, NUM_LEDS);
    FastLED.clear();

    // Set the brightness level (0-255)
    FastLED.setBrightness(50); // Adjust the brightness as desired

    // Start the I2C interface
    Wire.begin();
    rtc.begin();

    // The following lines can be uncommented to set the date and time
    rtc.setDOW(SATURDAY);     // Set Day-of-Week to TUESDAY
    rtc.setTime(10, 43, 55);  // Set the time to 10:43:00 (24hr format)
    rtc.setDate(24, 6, 2024); // Set the date to June 24th, 2024

    // Initialize button pins as inputs
    for (int i = 0; i < 4; i++)
    {
        pinMode(buttonPins[i], INPUT);
    }

    // Delay for initial setup
    delay(1000);

    // Initialize manual time with RTC time
    String timeString = rtc.getTimeStr();
    parseTimeString(timeString, manualHour, manualMinute);
}

void loop()
{
    // Get the current time as a string
    String timeString = rtc.getTimeStr();
    Serial.println(timeString);

    // Handle button presses
    if (digitalRead(buttonPins[0]) == HIGH)
    {                                            // Button pressed (HIGH due to pull-down resistor)
        manualHour = (manualHour - 1 + 24) % 24; // Wrap around 24-hour format
        delay(200);                              // Debounce delay
        timeAdjusted = true;
    }
    if (digitalRead(buttonPins[1]) == HIGH)
    {                                       // Button pressed (HIGH due to pull-down resistor)
        manualHour = (manualHour + 1) % 24; // Wrap around 24-hour format
        delay(200);                         // Debounce delay
        timeAdjusted = true;
    }
    if (digitalRead(buttonPins[2]) == HIGH)
    {                                                // Button pressed (HIGH due to pull-down resistor)
        manualMinute = (manualMinute - 1 + 60) % 60; // Wrap around 60-minute format
        delay(200);                                  // Debounce delay
        timeAdjusted = true;
    }
    if (digitalRead(buttonPins[3]) == HIGH)
    {                                           // Button pressed (HIGH due to pull-down resistor)
        manualMinute = (manualMinute + 1) % 60; // Wrap around 60-minute format
        delay(200);                             // Debounce delay
        timeAdjusted = true;
    }

    if (timeAdjusted)
    {
        rtc.setTime(manualHour, manualMinute, 0); // Update the RTC time
        timeAdjusted = false;                     // Reset the flag
    }

    // Parse the hour and minute from the RTC time
    int rtcHour, rtcMinute;
    parseTimeString(timeString, rtcHour, rtcMinute);

    // Display the parsed values
    Serial.print("RTC hour: ");
    Serial.println(rtcHour);
    Serial.print("RTC minute: ");
    Serial.println(rtcMinute);

    // Display the binary values on LEDs
    displayNumberInBinary(rtcHour, hour_leds, CRGB::Green);
    displayNumberInBinary(rtcMinute, min_leds, CRGB::Red);
}

void displayNumberInBinary(int number, CRGB *leds, CRGB color)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        // Check if the ith bit is set
        if (number & (1 << i))
        {
            leds[i] = color; // Turn LED to the specified color (1)
        }
        else
        {
            leds[i] = CRGB::Black; // Turn LED off (0)
        }
    }
    FastLED.show();
}

// Function to parse time string and update hour and minute variables
void parseTimeString(const String &timeString, int &hour, int &minute)
{
    // Extract hour and minute from the string
    hour = timeString.substring(0, 2).toInt();
    minute = timeString.substring(3, 5).toInt();
}
