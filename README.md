# BAM-navigator
Arduino code for navigator project using : Arduino uno, GPS-NEO-6M, OLED display and two buttons.  by: Ben Peled, Andres Godoy, Menashe Arad

our product helps the user to navigate back to last location even when cellular isn't available.(based on satellite communication)

Libraries and Dependencies:
 The code begins by importing necessary libraries such as TinyGPSPlus for handling GPS data, U8x8lib for driving an OLED display, Arduino.h for core Arduino functions, and SoftwareSerial for software-based serial communication.

Initialization:
Pins for software serial communication are defined (RXPin and TXPin) along with the baud rate (GPSBaud) for communication with the GPS device. Additionally, pins are defined for interrupt handling.

Setup Function: In the setup() function:
Serial communication with the GPS device (ss) is initiated.
Pin 2 is configured as an input for an interrupt from a pushbutton, and an interrupt service routine (ISR_pin2_SaveLocation) is attached to this pin.
Initialization of the OLED display (u8x8) is performed.

Loop Function: The loop() function:
Calculates the distance and angle to a saved location based on GPS coordinates.
Retrieves the number of functioning satellites and displays this information along with the distance and angle on the OLED display.
Waits for a specified duration before clearing the OLED display and checking for GPS data reception.

Helper Functions:
smartDelay(): Ensures that the GPS object is being updated properly by continuously processing incoming data while waiting for a specified duration.
ISR_pin2_SaveLocation(): An interrupt service routine triggered by a rising edge on pin 2 (configured as a pushbutton) that saves the current GPS location coordinates (latitude and longitude) to global variables saved_location_LAT and saved_location_LON.
