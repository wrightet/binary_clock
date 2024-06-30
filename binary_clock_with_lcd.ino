#include <LiquidCrystal.h>
#include <DS3231.h>
#include <Wire.h>

// Init the DS3231 using the hardware interface
DS3231 rtc(SDA, SCL);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#include <Arduino.h>
// pins in order of most to least significant bit
const int min_pins[] = {32, 30, 28, 26, 24, 22}; // Change these pin numbers according to your setup
const int hour_pins[] = {23, 25, 27, 29, 31};    // Change these pin numbers according to your setup

void parseTimeString(const String &timeString, int &hour, int &minute);
void setup()
{
    // Start the serial port
    Serial.begin(57600);

    // Initialize minute pins as OUTPUT
    for (int i = 0; i < 6; i++)
    {
        pinMode(min_pins[i], OUTPUT);
    }

    // Initialize hour pins as OUTPUT
    for (int i = 0; i < 5; i++)
    {
        pinMode(hour_pins[i], OUTPUT);
    }

    // Start the I2C interface
    Wire.begin();
    rtc.begin();

    // The following lines can be uncommented to set the date and time
    rtc.setDOW(FRIDAY);       // Set Day-of-Week to FRIDAY
    rtc.setTime(22, 21, 0);   // Set the time to 22:21:00 (24hr format)
    rtc.setDate(17, 5, 2024); // Set the date to May 17th, 2024

    // Initialize the LCD
    lcd.begin(16, 2);

    // Delay for initial setup
    delay(1000);
}

void loop()
{
   
    String timeString = rtc.getTimeStr();
    Serial.println(timeString);

    int hour = 0;
    int minute = 0;

    // Parse the hour and minute from the time string
    parseTimeString(timeString, hour, minute);
    displayBinary(hour,hour_pins);
    displayBinary(minute, min_pins);

    lcd.setCursor(0, 0);
    lcd.print("Time:  ");
    lcd.print(rtc.getTimeStr());

    lcd.setCursor(0, 1);
    lcd.print("Date: ");
    lcd.print(rtc.getDateStr());
    // count hours 0 - 23

    delay(1000);
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

// Function to parse time string and update hour and minute variables
void parseTimeString(const String &timeString, int &hour, int &minute)
{
    // Extract hour and minute from the string
    hour = timeString.substring(0, 2).toInt();
    minute = timeString.substring(3, 5).toInt();
}