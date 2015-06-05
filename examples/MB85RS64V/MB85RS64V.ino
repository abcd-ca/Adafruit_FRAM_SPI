#include <SPI.h>
#include "Adafruit_FRAM_SPI.h"

/* Example code for the Adafruit SPI FRAM breakout */
uint8_t FRAM_CS = 53; // Should be 10 for Uno, 53 for Mega
Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_CS);  // use hardware SPI

/*
// Or use software SPI, any pins!
uint8_t FRAM_SCK= 13;
uint8_t FRAM_MISO = 12;
uint8_t FRAM_MOSI = 11;
Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_SCK, FRAM_MISO, FRAM_MOSI, FRAM_CS);
*/

uint16_t          addr = 0;

void setup(void) {
  Serial.begin(9600);
  
  if (fram.begin()) {
    Serial.println("Found SPI FRAM");
  } else {
    Serial.println("No SPI FRAM found ... check your connections\r\n");
    while (1);
  }
  
  // Check the datasheet. If it says something like: The 1-Mbit serial F-RAM requires a 3-byte address then you need to specify 3, otherwise pass 2 (the default) or omit this line:
  fram.setAddressLengthInBytes(3);
  
  // Read the first byte
  uint8_t test = fram.read8(0x0);
  Serial.print("Incremented first byte "); Serial.print(test); Serial.println(" times");

  // Test write ++
  fram.writeEnable(true);
  Serial.print("Incrementing first byte ")
  fram.write8(0x0, test + 1);
  fram.writeEnable(false);
  
  fram.writeEnable(true);
  Serial.println("Writing test sequence");
  uint8_t sequenceToWrite[4] = { 0x22, 0x33, 0x44, 0x55 };
  int addressToStartWritingSequenceAt = 0x1;
  int lengthOfSequence = 4;
  fram.write(addressToStartWritingSequenceAt, sequenceToWrite, lengthOfSequence);
  fram.writeEnable(false);

  // dump first 100 bytes of memory
  uint8_t value;
  for (uint16_t a = 0; a < 100; a++) {
    value = fram.read8(a);
    if ((a % 32) == 0) {
      Serial.print("\n");
    }
    Serial.print("0x"); 
    if (value < 0x1) 
      Serial.print('0');
    Serial.print(value, HEX); Serial.print(" ");
  }
}

void loop(void) {

}