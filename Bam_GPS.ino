


// importing the requried libraries

#include <TinyGPSPlus.h>      // GPS data accumulation
#include <Arduino.h>          // Arduino core library 
#include <U8x8lib.h>          // Library for OLED display (screen)
#include <SoftwareSerial.h>   // Library for software serial communication


// Define an OLED display object  name = U8X8, driver name = SH1106, resolution = 128X64 
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); // dont need any pins for disply reset


// Define the pins for software serial communication
static const int RXPin = 4, TXPin = 3; //rxgps->3 , txgps->4
static const uint32_t GPSBaud = 9600;  //defining baudrate

float saved_location_LAT, saved_location_LON; // Variables to store saved location

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  //Serial.begin(115200); // Start serial communication at 115200 baud rate between computer and arduino 
  ss.begin(GPSBaud);    // Start software serial communication with GPS device and board

  pinMode(2, INPUT);    // Set pin 2 as input for interrupt from pushbutton (saving location)

  // Attach interrupt to pin 2, call ISR_pin2_SaveLocation function on rising edge
  attachInterrupt(digitalPinToInterrupt(2), ISR_pin2_SaveLocation, RISING);

  // Initialize OLED display
  u8x8.begin();
  u8x8.setPowerSave(0); 

}

void loop() {
  // Calculate distance and angle to saved location
  unsigned long distanceKmToSavedLocation =
      (unsigned long)TinyGPSPlus::distanceBetween(
          gps.location.lat(),
          gps.location.lng(),
          saved_location_LAT, 
          saved_location_LON);

  // Calculate the angle to the saved location
  double coursetosavedlocation =
      TinyGPSPlus::courseTo(
          gps.location.lat(),
          gps.location.lng(),
          saved_location_LAT, 
          saved_location_LON);
  
  // calculate the corresponding letters that fit the direction
  const char *cardinaltosavedlocation = TinyGPSPlus::cardinal(coursetosavedlocation);

  // Display number of functioning satelites, distance and angle on OLED display
  //setting font size and matrix location
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  //printing number of working sats, distance and angle
  u8x8.print(" Sats:");
  if (gps.satellites.value() > 4)
  {
    u8x8.println("valid ");
    u8x8.print(" Num of Sats: ");
  }
  else
  {
    u8x8.println("NotValid ");
    u8x8.print(" Num of Sats: ");
  }
  u8x8.println(gps.satellites.value());
  u8x8.println();
  u8x8.print("Distance");
  u8x8.print("  ");
  u8x8.println("Angle");
  u8x8.println();
  u8x8.println(" ");
  u8x8.print(distanceKmToSavedLocation);
  u8x8.print("    ");
  u8x8.print(coursetosavedlocation);
  u8x8.print(cardinaltosavedlocation);

  // Wait for a while
  smartDelay(5000);

  // Clear OLED display
  u8x8.clearDisplay();

  // Check if GPS data is not received
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS data received: check wiring"));
  }
}

// This custom version of delay() ensures that the gps object
// is being "fed" properly.
static void smartDelay(unsigned long ms) {
  unsigned long start = millis(); // start =  currect time in milisec 
  do {
    while (ss.available()) { // search for data
      gps.encode(ss.read());
    }
  } while (millis() - start < ms); //  check if 5000 (milisec) have been passed
}

// Interrupt service routine to save location
void ISR_pin2_SaveLocation() {
  saved_location_LON = gps.location.lng();
  saved_location_LAT = gps.location.lat(); 
}






