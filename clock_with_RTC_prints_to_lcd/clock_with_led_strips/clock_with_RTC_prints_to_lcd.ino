#include <LiquidCrystal.h>
#include <DS3231.h>
#include <Wire.h>
#include <FastLED.h>
// Init the DS3231 using the hardware interface
DS3231 rtc(SDA, SCL);

// Initialize the library by associating any needed LCD interface pin
// with the Arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pins in order of most to least significant bit
#define min_ pin = 6;
#define hour_pin = 7;
#define NUM_LEDS 6;
// Create a FastLED object
CRGB min_leds[NUM_LEDS];
CRGB hour_leds[NUM_LEDS];

void parseTimeString(const String &timeString, int &hour, int &minute);

int hour = 0;
int minute = 0;

void setup() {
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
    rtc.setDOW(TUESDAY);       // Set Day-of-Week to FRIDAY
    rtc.setTime(11, 10, 0);   // Set the time to 22:21:00 (24hr format)
    rtc.setDate(18, 6, 2024); // Set the date to May 17th, 2024

    // Initialize the LCD
    lcd.begin(16, 2);

    // Delay for initial setup
    delay(1000);
}

void loop() {
    // Get the current time as a string
    String timeString = rtc.getTimeStr();
    Serial.println(timeString);

    // Parse the hour and minute from the time string
    parseTimeString(timeString, hour, minute);

    // Display the parsed values
    Serial.print("Parsed hour: ");
    Serial.println(hour);
    Serial.print("Parsed minute: ");
    Serial.println(minute);

    // Display the binary values on LEDs
    displayBinary(hour, hour_leds);
    displayBinary(minute, hour_leds);

    // Display the time on the LCD
    lcd.setCursor(0, 0);
    lcd.print("Time:  ");
    lcd.print(timeString);

    lcd.setCursor(0, 1);
    lcd.print("Date: ");
    lcd.print(rtc.getDateStr());

   
}

void displayNumberInBinary(int number, leds)
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

// Function to parse time string and update hour and minute variables
void parseTimeString(const String &timeString, int &hour, int &minute) {
    // Extract hour and minute from the string
    hour = timeString.substring(0, 2).toInt();
    minute = timeString.substring(3, 5).toInt();
}
